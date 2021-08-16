/*
 * Arbitrary precision arithmetic implementation.
 */

#ifndef _BIGNUM_H_INCLUDED_
#define _BIGNUM_H_INCLUDED_

#include <stdint.h>

#define BIGNUM_BITS_IN_DITGIT 16

#if BIGNUM_BITS_IN_DITGIT == 32

typedef uint32_t word;
typedef int32_t sword;
typedef uint64_t dword;
typedef int64_t sdword;

#define BIGNUM_SHIFT 32
#define BIGNUM_DECIMAL_DIGITS 9
#define BIGNUM_DECIMAL_BASE 1000000000

#elif BIGNUM_BITS_IN_DITGIT == 16

typedef uint16_t word;
typedef int16_t sword;
typedef uint32_t dword;
typedef int32_t sdword;

#define BIGNUM_SHIFT 16
#define BIGNUM_DECIMAL_DIGITS 4
#define BIGNUM_DECIMAL_BASE 10000

#else
#  error "BIGNUM_BITS_IN_DITGIT must be defined to 16 or 32."
#endif

#define BIGNUM_BASE ((dword)1 << BIGNUM_SHIFT)
#define BIGNUM_MASK (BIGNUM_BASE - 1)

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define BIGNUM_POSITIVE 10
#define BIGNUM_NEGATIVE 11

#define BIGNUM_POSITIVE_COMPLEMENT 0
#define BIGNUM_NEGATIVE_COMPLEMENT 1

typedef struct bignum {
  int sign;
  int size;
  word *digit;
} bignum;

bignum *bignum_new(void);

void bignum_free(bignum *a);

void bignum_assign(bignum *a, const bignum *b);

void bignum_assign_int(bignum *a, int b);

void bignum_assign_str(bignum *a, char *b);

int bignum_to_int(bignum *a);

char *bignum_to_str(bignum *a);

void bignum_add(bignum *a, bignum *b, bignum *c);

void bignum_sub(bignum *a, bignum *b, bignum *c);

void bignum_mul(bignum *a, bignum *b, bignum *c);

void bignum_div(bignum *a, bignum *b, bignum *c);

/* Bitwise */

void bignum_neg(bignum *a, bignum *b);

void bignum_or(bignum *a, bignum *b, bignum *c);

void bignum_xor(bignum *a, bignum *b, bignum *c);

void bignum_and(bignum *a, bignum *b, bignum *c);

#endif  // _BIGNUM_H_INCLUDED_

