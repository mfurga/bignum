/*
 * Arbitrary precision arithmetic implementation.
 */

#include "bignum.h"

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

static void bignum_resize(bignum *a, int sz);
static void bignum_lshift(bignum *a, int shift);
static bignum *bignum_normalize(bignum *a);
static int bignum_is_zero(const bignum *a);
static int bignum_bit_length(word x);
static void bignum_set_sign(bignum *a, int sign);

/* Arithmetic. */
static bignum *bignum_add_a(const bignum *a, const bignum *b);
static bignum *bignum_sub_a(const bignum *a, const bignum *b);
static bignum *bignum_mul_a(const bignum *a, const bignum *b);
static bignum *bignum_div_a(const bignum *a, const bignum *b);
static bignum *bignum_div_a1(const bignum *a, const bignum *b);
static bignum *bignum_div_a2(const bignum *a, const bignum *b);

static bignum *bignum_bitwise_op(bignum *a, bignum *b, char op);

/*
 * Create a new bignum object initialized to 0.
 */
bignum *
bignum_new(void)
{
  bignum *a = malloc(sizeof(bignum));
  if (a == NULL) {
    return NULL;
  }
  a->sign = BIGNUM_POSITIVE;
  a->size = 1;
  a->digit = malloc(sizeof(word));
  if (a->digit == NULL) {
    free(a);
    return NULL;
  }
  a->digit[0] = 0;
  return a;
}

void
bignum_free(bignum *a)
{
  assert(a != NULL);
  assert(a->digit != NULL);
  free(a->digit);
  free(a);
}

static void
bignum_resize(bignum *a, int sz)
{
  int size, i;
  word *digit;

  assert(a != NULL);
  assert(a->digit != NULL);
  assert(sz > 0);

  size = a->size;
  digit = a->digit;

  a->size = sz;
  a->digit = malloc(sizeof(word) * sz);
  if (a->digit == NULL) {
    /* todo: Error. */
    return;
  }

  for (i = 0; i < MIN(sz, size); i++) {
    a->digit[i] = digit[i];
  }

  /* a is not normalize. */
  for (; i < sz; i++) {
    a->digit[i] = a->sign == BIGNUM_NEGATIVE_COMPLEMENT ? BIGNUM_MASK : 0;
  }

  free(digit);
}

void
bignum_assign(bignum *a, const bignum *b)
{
  int size;

  assert(a != NULL);
  assert(b != NULL);

  a->sign = b->sign;
  bignum_resize(a, b->size);

  size = b->size;
  while (--size >= 0) {
    a->digit[size] = b->digit[size];
  }
}

void
bignum_assign_int(bignum *a, int b)
{
  int sign;
  int size;
  unsigned abs_b;

  assert(a != NULL);

  if (b < 0) {
    /* Because undefined behaviour when b is INT_MIN. */
    sign = BIGNUM_NEGATIVE;
    abs_b = 0U - (unsigned)b;
  } else {
    sign = BIGNUM_POSITIVE;
    abs_b = (unsigned)b;
  }

  a->sign = sign;

#if BIGNUM_SHIFT == 16
  size = 2;
  bignum_resize(a, size);

  a->size = size;
  a->digit[0] = abs_b & BIGNUM_MASK;
  a->digit[1] = (abs_b >> BIGNUM_SHIFT) & BIGNUM_MASK;
#elif BIGNUM_SHIFT == 32
  size = 1;
  bignum_resize(a, size);

  a->size = size;
  a->digit[0] = abs_b & BIGNUM_MASK;
#endif

  bignum_normalize(a);
}

void
bignum_assign_str(bignum *a, char *b)
{
  assert(a != NULL);
  assert(b != NULL);

  int size_a, size_b, sign, i, d;

  if (*b == '-') {
    sign = BIGNUM_NEGATIVE;
    b++;
  } else if (*b == '+') {
    sign = BIGNUM_POSITIVE;
    b++;
  } else {
    sign = BIGNUM_POSITIVE;
  }

  size_b = strlen(b);

  /*
   * Calculate the length the BIGNUM_BASE representation based on the following
   * approximation:
   * 2^x = 10^(size_b) => x = size_b*lg(10) <= size_b*3.33 <= size_b * 4
   *
   * size_a = (x / BIGNUM_BITS_IN_DITGIT) + 1
   */

  bignum_assign_int(a, 0);
  size_a = ((long long)size_b * 4) / BIGNUM_BITS_IN_DITGIT + 1;
  bignum_resize(a, size_a);

#define ADD(a, b) do {                                                          \
    dword carry = (dword)(a)->digit[0] + (dword)(b);                            \
    (a)->digit[0] = carry & BIGNUM_MASK;                                        \
    carry >>= BIGNUM_SHIFT;                                                     \
    for (int i = 1; i < (a)->size && carry > 0; i++) {                          \
      carry += (a)->digit[i];                                                   \
      (a)->digit[i] = carry & BIGNUM_MASK;                                      \
      carry >>= BIGNUM_SHIFT;                                                   \
    }                                                                           \
    assert(carry == 0);                                                         \
  } while (0)

#define MULTIPLY(a, b) do {                                                     \
    dword carry = 0;                                                            \
    for (int i = 0; i < (a)->size; i++) {                                       \
      carry += (dword)(a)->digit[i] * (dword)b;                                 \
      (a)->digit[i] = carry & BIGNUM_MASK;                                      \
      carry >>= BIGNUM_SHIFT;                                                   \
    }                                                                           \
    assert(carry == 0);                                                         \
  } while (0)

  d = 0;
  for (i = 0; i < size_b % BIGNUM_DECIMAL_DIGITS; i++) {
    d *= 10;
    d += b[i] - '0';
  }

  ADD(a, d);

  d = 0;
  for (int j = 1; i < size_b; i++, j++) {
    d *= 10;
    d += b[i] - '0';
    if (j == BIGNUM_DECIMAL_DIGITS) {
      MULTIPLY(a, BIGNUM_DECIMAL_BASE);
      ADD(a, d);
      d = 0; j = 0;
    }
  }

#undef ADD
#undef MULTIPLY

  bignum_set_sign(a, sign);
  bignum_normalize(a);
}

/*
 * Ignores overflows.
 */
int
bignum_to_int(bignum *a)
{
  assert(a != NULL);

  int b = 0;

  b |= a->digit[0];

#if BIGNUM_SHIFT == 16
  if (a->size > 1) {
    b |= a->digit[1] << BIGNUM_SHIFT;
  }
#endif

  return a->sign == BIGNUM_POSITIVE ? (b) : -(b);
}

/*
 * Return the decimal representation of the number. Caller should free the memory
 * allocated by this function. Return NULL on error.
 */
char *
bignum_to_str(bignum *a)
{
  int size_a, size_b, size_r, digits, i;
  bignum *b;
  char *r, *p;
  dword carry = 0;

  assert(a != NULL);

  size_a = a->size;

  /*
   * Calculate the length of the decimal representation based on the following
   * approximation:
   *
   *  2^(size * word_size) = 10^x => x = log(2) * (size * word_size) <=
   *  <= 0.31 * (size * word_size) <= floor((31 * size * word_size) / 100) + 1.
   */
  digits = (31LL * (long long)size_a * BIGNUM_BITS_IN_DITGIT) / 100 + 1;

  b = bignum_new();
  if (b == NULL) {
    return NULL;
  }
  bignum_resize(b, digits / BIGNUM_DECIMAL_DIGITS + 1);

  /* Radix conversion according to TAOCP vol. 2 (3rd ed.), section 4.4, Method 1b. */
  size_b = 0;
  for (int i = size_a - 1; i >= 0; i--) {
    carry = a->digit[i];

    for (int j = 0; j < size_b; j++) {
      carry = (dword)b->digit[j] << BIGNUM_SHIFT | carry;
      b->digit[j] = carry % BIGNUM_DECIMAL_BASE;
      carry /= BIGNUM_DECIMAL_BASE;
    }

    while (carry > 0) {
      b->digit[size_b++] = carry % BIGNUM_DECIMAL_BASE;
      carry /= BIGNUM_DECIMAL_BASE;
    }
  }
  b = bignum_normalize(b);

  size_r = (long long)(b->size - 1) * BIGNUM_DECIMAL_DIGITS;

  /* Count digits for the MSW. */
  {
    word d = b->digit[b->size - 1];
    if (d == 0) {
      size_r++;
    }
    while (d > 0) {
      size_r++;
      d /= 10;
    }
  }

  if (a->sign == BIGNUM_NEGATIVE) {
    size_r++;  /* +1 for the '-' character. */
  }

  size_r++;  /* +1 for the nul byte. */
  r = malloc(size_r * sizeof(char));
  if (r == NULL) {
    bignum_free(b);
    return NULL;
  }

  p = r + (size_r - 1);
  *p-- = '\0';

  for (i = 0; i < b->size - 1; i++) {
    for (int j = 0; j < BIGNUM_DECIMAL_DIGITS; j++) {
      *p-- = '0' + (char)(b->digit[i] % 10);
      b->digit[i] /= 10;
    }
  }

  if (b->digit[i] == 0) {
    *p-- = '0';
  }

  while (b->digit[i] > 0) {
    *p-- = '0' + (char)(b->digit[i] % 10);
    b->digit[i] /= 10;
  }

  if (a->sign == BIGNUM_NEGATIVE) {
    *p-- = '-';
  }

  assert(r == p + 1);

  bignum_free(b);
  return r;
}

void
bignum_add(bignum *a, bignum *b, bignum *c)
{
  bignum *r;

  assert(a != NULL && b != NULL && c != NULL);

  if (a->sign == BIGNUM_NEGATIVE) {
    if (b->sign == BIGNUM_NEGATIVE) {
      r = bignum_add_a(a, b);
      bignum_set_sign(r, BIGNUM_NEGATIVE);
    } else {
      r = bignum_sub_a(b, a);
    }
  } else {
    if (b->sign == BIGNUM_NEGATIVE) {
      r = bignum_sub_a(a, b);
    } else {
      r = bignum_add_a(a, b);
    }
  }

  bignum_assign(c, r);
  bignum_free(r);
}

void
bignum_sub(bignum *a, bignum *b, bignum *c)
{
  bignum *r;

  assert(a != NULL && b != NULL && c != NULL);

  if (a->sign == BIGNUM_NEGATIVE) {
    if (b->sign == BIGNUM_NEGATIVE) {
      r = bignum_sub_a(b, a);
    } else {
      r = bignum_add_a(a, b);
      bignum_set_sign(r, BIGNUM_NEGATIVE);
    }
  } else {
    if (b->sign == BIGNUM_NEGATIVE) {
      r = bignum_add_a(a, b);
    } else {
      r = bignum_sub_a(a, b);
    }
  }

  bignum_assign(c, r);
  bignum_free(r);
}

void
bignum_mul(bignum *a, bignum *b, bignum *c)
{
  assert(a != NULL && b != NULL && c != NULL);

  bignum *r;

  if (a->sign == BIGNUM_NEGATIVE) {
    if (b->sign == BIGNUM_NEGATIVE) {
      r = bignum_mul_a(a, b);
    } else {
      r = bignum_mul_a(a, b);
      bignum_set_sign(r, BIGNUM_NEGATIVE);
    }
  } else {
    if (b->sign == BIGNUM_NEGATIVE) {
      r = bignum_mul_a(a, b);
      bignum_set_sign(r, BIGNUM_NEGATIVE);
    } else {
      r = bignum_mul_a(a, b);
    }
  }

  bignum_assign(c, r);
  bignum_free(r);
}

void
bignum_div(bignum *a, bignum *b, bignum *c)
{
  assert(a != NULL && b != NULL && c != NULL);
  assert(bignum_is_zero(b) == 0);

  bignum *r;

  if (a->sign == BIGNUM_NEGATIVE) {
    if (b->sign == BIGNUM_NEGATIVE) {
      r = bignum_div_a(a, b);
    } else {
      r = bignum_div_a(a, b);
      bignum_set_sign(r, BIGNUM_NEGATIVE);
    }
  } else {
    if (b->sign == BIGNUM_NEGATIVE) {
      r = bignum_div_a(a, b);
      bignum_set_sign(r, BIGNUM_NEGATIVE);
    } else {
      r = bignum_div_a(a, b);
    }
  }

  bignum_assign(c, r);
  bignum_free(r);
}

static bignum *
bignum_add_a(const bignum *a, const bignum *b)
{
  int size_a, size_b, i;
  dword carry = 0;
  bignum *c;

  size_a = a->size;
  size_b = b->size;

  if (size_a < size_b) {
    { const bignum *tmp = a; a = b; b = tmp; }
    { int tmp = size_a; size_a = size_b; size_b = tmp; }
  }

  c = bignum_new();
  bignum_resize(c, size_a + 1);

  for (i = 0; i < size_b; i++) {
    carry += (dword)a->digit[i] + (dword)b->digit[i];
    c->digit[i] = carry & BIGNUM_MASK;
    carry >>= BIGNUM_SHIFT;
  }

  for (; i < size_a; i++) {
    carry += (dword)a->digit[i];
    c->digit[i] = carry & BIGNUM_MASK;
    carry >>= BIGNUM_SHIFT;
  }

  c->digit[i] = carry;
  return bignum_normalize(c);
}

static bignum *
bignum_sub_a(const bignum *a, const bignum *b)
{
  int size_a, size_b, sign, i;
  dword borrow = 0;
  bignum *c;

  sign = BIGNUM_POSITIVE;
  size_a = a->size;
  size_b = b->size;

  if (size_a < size_b) {
    { const bignum *tmp = a; a = b; b = tmp; }
    { int tmp = size_a; size_a = size_b; size_b = tmp; }
    sign = BIGNUM_NEGATIVE;
  } else if (size_a == size_b) {
    i = size_a - 1;

    while (i >= 0 && a->digit[i] == b->digit[i]) {
      i--;
    }

    if (i < 0) {
      return bignum_new();  /* Numbers are equal. Return 0. */
    }

    if (a->digit[i] < b->digit[i]) {
      { const bignum *tmp = a; a = b; b = tmp; }
      sign = BIGNUM_NEGATIVE;
    }

    size_a = size_b = i + 1;
  }

  c = bignum_new();
  c->sign = sign;
  bignum_resize(c, size_a);

  for (i = 0; i < size_b; i++) {
    borrow = BIGNUM_BASE + (dword)a->digit[i] - (dword)b->digit[i] - borrow;
    c->digit[i] = borrow & BIGNUM_MASK;
    borrow = borrow < BIGNUM_BASE;
  }

  for (; i < size_a; i++) {
    borrow = BIGNUM_BASE + (dword)a->digit[i] - borrow;
    c->digit[i] = borrow & BIGNUM_MASK;
    borrow = borrow < BIGNUM_BASE;
  }

  assert(borrow == 0);
  return bignum_normalize(c);
}

/*
 * Primary school multiplication.
 */
static bignum *
bignum_mul_a(const bignum *a, const bignum *b)
{
  int size_a, size_b;
  dword carry = 0;
  bignum *c;

  size_a = a->size;
  size_b = b->size;

  c = bignum_new();
  bignum_resize(c, size_a + size_b);

  for (int i = 0; i < size_a; i++) {
    for (int j = 0; j < size_b; j++) {
      carry += (dword)c->digit[i + j] + (dword)a->digit[i] * (dword)b->digit[j];
      c->digit[i + j] = carry & BIGNUM_MASK;
      carry >>= BIGNUM_SHIFT;
    }
    c->digit[i + size_b] += carry & BIGNUM_MASK;
    carry >>= BIGNUM_SHIFT;

    assert(carry == 0);
  }

  return bignum_normalize(c);
}

static bignum *
bignum_div_a(const bignum *a, const bignum *b)
{
  if (b->size == 1) {
    return bignum_div_a1(a, b);
  }

  if (a->size < b->size) {
    return bignum_new();  /* Return 0. */
  }

  return bignum_div_a2(a, b);
}

/*
 * Linear time division. Assuming length of b is 1.
 */
static bignum *
bignum_div_a1(const bignum *a, const bignum *b)
{
  assert(b->size == 1);

  word digit = b->digit[0];
  dword rem = 0;
  bignum *r;

  r = bignum_new();
  bignum_resize(r, a->size);

  for (int i = a->size - 1; i >= 0; i--) {
    rem = rem << BIGNUM_SHIFT | a->digit[i];
    r->digit[i] = (word)(rem / digit);
    rem = rem % digit;
  }

  return bignum_normalize(r);
}

/*
 * Division based on Knuth's algorithm in TAOCP vol. 2 (3rd ed.), section 4.3.1, Algorithm D.
 */
static bignum *
bignum_div_a2(const bignum *a, const bignum *b)
{
  bignum *u, *v, *res, *qv;
  int n, m, d, neg;
  dword q, r, uu, carry, borrow;

  assert(a->size >= b->size && b->size >= 2);

  n = b->size;
  m = a->size - b->size;

  u = bignum_new();
  bignum_assign(u, a);

  v = bignum_new();
  bignum_assign(v, b);

  qv = bignum_new();
  bignum_assign(qv, v);
  bignum_resize(qv, b->size + 1);

  res = bignum_new();
  bignum_resize(res, m + 1);

  /* Normalization step. Make sure that the MSD of v >= BIGNUM_BASE/2. */
  d = BIGNUM_SHIFT - bignum_bit_length(v->digit[v->size - 1]);
  bignum_lshift(u, d);
  bignum_lshift(v, d);
  if (u->size < a->size + 1) {
    bignum_resize(u, a->size + 1);
  }
  assert(v->size == b->size && v->digit[v->size - 1] >= BIGNUM_BASE / 2);

  for (int j = m; j >= 0; j--) {
    uu = ((dword)u->digit[j + n] << BIGNUM_SHIFT | (dword)u->digit[j + n - 1]);

    q = uu / (dword)v->digit[n - 1];
    r = uu % (dword)v->digit[n - 1];

    while (q * (dword)v->digit[n - 2] > ((r << BIGNUM_SHIFT) | (dword)u->digit[j + n - 2]) ||
        q == BIGNUM_BASE) {
      q--;
      r += v->digit[n - 1];
      if (r >= BIGNUM_BASE) {
        break;
      }
    }
    assert(q < BIGNUM_BASE);

  /* Check if a - b is negative. a is shifted by j. */
#define IS_NEGATIVE(a, b, r) do {                                                          \
    (r) = 0;                                                                               \
    for (int i = n; i >= 0; i--) {                                                         \
      if ((b)->digit[i] > (a)->digit[j + i]) {                                             \
        (r) = 1; break;                                                                    \
      }                                                                                    \
      if ((b)->digit[i] < (a)->digit[j + i]) {                                             \
        break;                                                                             \
      }                                                                                    \
    }                                                                                      \
  } while (0)

    /* Multiply v times q. */
    carry = 0;
    for (int i = 0; i < n; i++) {
      carry += (dword)q * (dword)v->digit[i];
      qv->digit[i] = carry & BIGNUM_MASK;
      carry >>= BIGNUM_SHIFT;
    }
    qv->digit[n] = carry;

    IS_NEGATIVE(u, qv, neg);

    /* This branch is taken with probability ~ 2/BIGNUM_BASE. */
    if (neg) {
      q--;
      borrow = 0;
      for (int i = 0; i < n; i++) {
        borrow = BIGNUM_BASE + (dword)qv->digit[i] - (dword)v->digit[i] - borrow;
        qv->digit[i] = borrow & BIGNUM_MASK;
        borrow = borrow < BIGNUM_BASE;
      }
      qv->digit[n] -= borrow;

      IS_NEGATIVE(u, qv, neg);
      assert(neg == 0);
    }

    borrow = 0;
    for (int i = 0; i < n + 1; i++) {
      borrow = BIGNUM_BASE + (dword)u->digit[j + i] - (dword)qv->digit[i] - borrow;
      u->digit[j + i] = borrow & BIGNUM_MASK;
      borrow = borrow < BIGNUM_BASE;
    }
    assert(borrow == 0);

    res->digit[j] = q;

#undef IS_NEGATIVE
  }

  bignum_free(u);
  bignum_free(v);
  bignum_free(qv);

  return bignum_normalize(res);
}

/*
 * Convert the bignum object to the two's complement representation.
 * After that bignum->sign is treated as sign bit in the two's complement.
 */
static void
bignum_to_complement(bignum *a)
{
  dword carry = 1;

  /* Positive number already have a good two's complement representation. */
  if (a->sign == BIGNUM_POSITIVE) {
    a->sign = BIGNUM_POSITIVE_COMPLEMENT;
    return;
  }

  a->sign = BIGNUM_NEGATIVE_COMPLEMENT;

  for (int i = 0; i < a->size; i++) {
    carry += ~(a->digit[i]) & BIGNUM_MASK;
    a->digit[i] = carry & BIGNUM_MASK;
    carry >>= BIGNUM_SHIFT;
  }

  a->sign = (a->sign + carry) & 1;
}

/*
 * Convert the two's complement representation to the correct bignum object.
 */
static void
bignum_from_complement(bignum *a)
{
  dword borrow = 1;

  /* Positive number already have a good bignum representation. */
  if (a->sign == BIGNUM_POSITIVE_COMPLEMENT) {
    a->sign = BIGNUM_POSITIVE;
    return;
  }

  a->sign = BIGNUM_NEGATIVE;

  for (int i = 0; i < a->size; i++) {
    borrow = BIGNUM_BASE + (dword)a->digit[i] - borrow;
    a->digit[i] = ~(borrow & BIGNUM_MASK);
    borrow = borrow < BIGNUM_BASE;
  }

  assert(borrow == 0);  /* overflow. */
}

void
bignum_neg(bignum *a, bignum *b)
{
  assert(a != NULL && b != NULL);

  bignum_assign(b, a);
  /* +1 to make sure that the final two's complement representation doesn't overflow. */
  bignum_resize(b, a->size + 1);

  bignum_to_complement(b);

  b->sign = 1 - b->sign;
  for (int i = 0; i < b->size; i++) {
    b->digit[i] = ~b->digit[i];
  }

  bignum_from_complement(b);
  bignum_normalize(b);
}

void
bignum_or(bignum *a, bignum *b, bignum *c)
{
  assert(a != NULL && b != NULL && c != NULL);

  bignum *r = bignum_bitwise_op(a, b, '|');

  bignum_assign(c, r);
  bignum_free(r);
}

void
bignum_xor(bignum *a, bignum *b, bignum *c)
{
  assert(a != NULL && b != NULL && c != NULL);

  bignum *r = bignum_bitwise_op(a, b, '^');

  bignum_assign(c, r);
  bignum_free(r);
}

void
bignum_and(bignum *a, bignum *b, bignum *c)
{
  assert(a != NULL && b != NULL && c != NULL);

  bignum *r = bignum_bitwise_op(a, b, '&');

  bignum_assign(c, r);
  bignum_free(r);
}

/* Perform bitwise OR XOR AND. */
static bignum *
bignum_bitwise_op(bignum *a, bignum *b, char op)
{
  bignum *r;
  int size_a, size_b, i;
  word pad;

  size_a = a->size;
  size_b = b->size;

  if (size_a < size_b) {
    { bignum *tmp = a; a = b; b = tmp; }
    { int tmp = size_a; size_a = size_b; size_b = tmp; }
  }

  bignum_to_complement(a);
  bignum_to_complement(b);

  if (b->sign == BIGNUM_POSITIVE_COMPLEMENT) {
    pad = 0;
  } else {
    pad = BIGNUM_MASK;  /* 1111... */
  }

  r = bignum_new();

  switch (op) {
    case '|':
      r->sign = a->sign | b->sign;
    break;
    case '^':
      r->sign = a->sign ^ b->sign;
    break;
    case '&':
      r->sign = a->sign & b->sign;
    break;
  }

  /* +1 if result is negative to make sure that the final two's complement
     representation doesn't overflow. */
  bignum_resize(r, size_a + r->sign);

  switch (op) {
    case '|':
      for (i = 0; i < size_b; i++) {
        r->digit[i] = a->digit[i] | b->digit[i];
      }
      for (; i < size_a; i++) {
        r->digit[i] = a->digit[i] | pad;
      }
    break;
    case '^':
      for (i = 0; i < size_b; i++) {
        r->digit[i] = a->digit[i] ^ b->digit[i];
      }
      for (; i < size_a; i++) {
        r->digit[i] = a->digit[i] ^ pad;
      }
    break;
    case '&':
      for (i = 0; i < size_b; i++) {
        r->digit[i] = a->digit[i] & b->digit[i];
      }
      for (; i < size_a; i++) {
        r->digit[i] = a->digit[i] & pad;
      }
    break;
  }

  bignum_from_complement(a);
  bignum_from_complement(b);
  bignum_from_complement(r);

  return bignum_normalize(r);
}

static int
bignum_is_zero(const bignum *a)
{
  return a->size == 1 && a->digit[0] == 0;
}

/*
 * Shift digits s bits left. 0 <= s < BIGNUM_SHIFT.
 */
static void
bignum_lshift(bignum *a, int s)
{
  assert(a != NULL);
  assert(s < BIGNUM_SHIFT);

  dword acc;
  word carry = 0;

  bignum_resize(a, a->size + 1);

  for (int i = 0; i < a->size; i++) {
    acc = ((dword)a->digit[i] << s) | carry;
    a->digit[i] = (word)(acc & BIGNUM_MASK);
    carry = (word)(acc >> BIGNUM_SHIFT);
  }

  assert(carry == 0);
  bignum_normalize(a);
}

static void
bignum_set_sign(bignum *a, int sign)
{
  a->sign = bignum_is_zero(a) == 1 ? BIGNUM_POSITIVE : sign;
}

static int
bignum_bit_length(word x)
{
  int length = 0;
  while (x > 0) {
    length++;
    x >>= 1;  /* Right shift with zero-extend. */
  }
  return length;
}

static bignum *
bignum_normalize(bignum *a)
{
  while (a->size - 1 > 0 && a->digit[a->size - 1] == 0) {
    a->size--;
  }
  return a;
}

