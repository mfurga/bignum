#include "bignum.h"

#include <stdio.h>
#include <assert.h>
#include <limits.h>

void
bignum_number_equal(bignum *a, int b)
{
  int sign;
  unsigned abs_b;

  if (b >= 0) {
    sign = BIGNUM_POSITIVE;
    abs_b = (unsigned)b;
  } else {
    sign = BIGNUM_NEGATIVE;
    abs_b = 0U - (unsigned)b;
  }

  assert(a->sign == sign);

#if BIGNUM_BITS_IN_DITGIT == 32
  assert(a->size == 1);
  assert(a->digit[0] == abs_b);
#elif BIGNUM_BITS_IN_DITGIT == 16
  if (abs_b >= 1U << 16) {
    assert(a->size == 2);
    assert(a->digit[0] == (abs_b & 0xffff));
    assert(a->digit[1] == ((abs_b >> 16) & 0xffff));
  } else {
    assert(a->size == 1);
    assert(a->digit[0] == abs_b);
  }
#endif
}

void
bignum_new_tests()
{
  bignum *a = bignum_new();

  assert(a->size == 1);
  assert(a->sign == BIGNUM_POSITIVE);
  assert(a->digit[0] == 0);

  bignum_free(a);
}

void
bignum_assign_int_tests()
{
  bignum *a = bignum_new();

  bignum_assign_int(a, -1);
  bignum_number_equal(a, -1);

  bignum_assign_int(a, 0xffff);
  bignum_number_equal(a, 0xffff);

  bignum_assign_int(a, 0xffff + 1);
  bignum_number_equal(a, 0xffff + 1);

  bignum_assign_int(a, -0xffff - 1);
  bignum_number_equal(a, -0xffff - 1);

  bignum_assign_int(a, INT_MAX);
  bignum_number_equal(a, INT_MAX);

  bignum_assign_int(a, INT_MIN);
  bignum_number_equal(a, INT_MIN);

  bignum_free(a);
}

/* Only for int size. */
void
bignum_assign_str_tests()
{
  bignum *a = bignum_new();

  bignum_assign_str(a, "0");
  bignum_number_equal(a, 0);

  bignum_assign_str(a, "+0");
  bignum_number_equal(a, 0);

  bignum_assign_str(a, "-0");
  bignum_number_equal(a, 0);

  bignum_assign_str(a, "+1");
  bignum_number_equal(a, 1);

  bignum_assign_str(a, "32767"); /* 2^15 - 1 */
  bignum_number_equal(a, 32767);

  bignum_assign_str(a, "-32768"); /* -2^15 */
  bignum_number_equal(a, -32768);

  bignum_assign_str(a, "2147483647"); /* INT_MAX: 2^31 - 1 */
  bignum_number_equal(a, 2147483647);

  bignum_assign_str(a, "-2147483648"); /* INT_MIN: -2^31 */
  bignum_number_equal(a, -2147483648);

  bignum_free(a);
}


int main(void)
{
  bignum_new_tests();
  bignum_assign_int_tests();
  bignum_assign_str_tests();

  printf("OK\n");
  return 0;
}

