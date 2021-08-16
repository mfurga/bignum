#include "bignum.h"
#include "unit.h"

#include <stdio.h>
#include <limits.h>

#if BIGNUM_BITS_IN_DITGIT == 32

#define BIGNUM_CMP_WITH_INT(a, b) do {                                                 \
    int sign;                                                                          \
    unsigned abs_b;                                                                    \
    if ((b) >= 0) {                                                                    \
      sign = BIGNUM_POSITIVE;                                                          \
      abs_b = (unsigned)(b);                                                           \
    } else {                                                                           \
      sign = BIGNUM_NEGATIVE;                                                          \
      abs_b = 0U - (unsigned)(b);                                                      \
    }                                                                                  \
    ASSERT_EQUAL_INT((a)->sign, sign);                                                 \
    ASSERT_EQUAL_INT((a)->size, 1);                                                    \
    ASSERT_EQUAL_UINT((a)->digit[0], abs_b);                                           \
  } while (0)

#elif BIGNUM_BITS_IN_DITGIT == 16

#define BIGNUM_CMP_WITH_INT(a, b) do {                                                 \
    int sign;                                                                          \
    unsigned abs_b;                                                                    \
    if ((b) >= 0) {                                                                    \
      sign = BIGNUM_POSITIVE;                                                          \
      abs_b = (unsigned)(b);                                                           \
    } else {                                                                           \
      sign = BIGNUM_NEGATIVE;                                                          \
      abs_b = 0U - (unsigned)(b);                                                      \
    }                                                                                  \
    ASSERT_EQUAL_INT(a->sign, sign);                                                   \
    if (abs_b >= 1U << 16) {                                                           \
      ASSERT_EQUAL_INT((a)->size, 2);                                                  \
      ASSERT_EQUAL_UINT((a)->digit[0], (abs_b & 0xffff));                              \
      ASSERT_EQUAL_UINT((a)->digit[1], ((abs_b >> 16) & 0xffff));                      \
    } else {                                                                           \
      ASSERT_EQUAL_INT((a)->size, 1);                                                  \
      ASSERT_EQUAL_UINT((a)->digit[0], abs_b);                                         \
    }                                                                                  \
  } while (0)

#endif

void
bignum_new_tests()
{
  bignum *a = bignum_new();

  BIGNUM_CMP_WITH_INT(a, 0);

  bignum_free(a);
}

void
bignum_assign_int_tests()
{
  bignum *a = bignum_new();

  bignum_assign_int(a, -1);
  BIGNUM_CMP_WITH_INT(a, -1);

  bignum_assign_int(a, 0xffff);
  BIGNUM_CMP_WITH_INT(a, 0xffff);

  bignum_assign_int(a, 0xffff + 1);
  BIGNUM_CMP_WITH_INT(a, 0xffff + 1);

  bignum_assign_int(a, -0xffff - 1);
  BIGNUM_CMP_WITH_INT(a, -0xffff - 1);

  bignum_assign_int(a, INT_MAX);
  BIGNUM_CMP_WITH_INT(a, INT_MAX);

  bignum_assign_int(a, INT_MIN);
  BIGNUM_CMP_WITH_INT(a, INT_MIN);

  bignum_free(a);
}

/* Only for int size. */
void
bignum_assign_str_tests()
{
  bignum *a = bignum_new();

  bignum_assign_str(a, "0");
  BIGNUM_CMP_WITH_INT(a, 0);

  bignum_assign_str(a, "+0");
  BIGNUM_CMP_WITH_INT(a, 0);

  bignum_assign_str(a, "-0");
  BIGNUM_CMP_WITH_INT(a, 0);

  bignum_assign_str(a, "+1");
  BIGNUM_CMP_WITH_INT(a, 1);

  bignum_assign_str(a, "32767"); /* 2^15 - 1 */
  BIGNUM_CMP_WITH_INT(a, 32767);

  bignum_assign_str(a, "-32768"); /* -2^15 */
  BIGNUM_CMP_WITH_INT(a, -32768);

  bignum_assign_str(a, "2147483647"); /* INT_MAX: 2^31 - 1 */
  BIGNUM_CMP_WITH_INT(a, 2147483647);

  bignum_assign_str(a, "-2147483648"); /* INT_MIN: -2^31 */
  BIGNUM_CMP_WITH_INT(a, -2147483648);

  bignum_free(a);
}

void
bignum_neg_tests()
{
  bignum *a = bignum_new();
  bignum *b = bignum_new();

  bignum_assign_int(a, -1);
  bignum_neg(a, b);
  BIGNUM_CMP_WITH_INT(a, -1);
  BIGNUM_CMP_WITH_INT(b, 0);

  bignum_assign_int(a, 0);
  bignum_neg(a, b);
  BIGNUM_CMP_WITH_INT(a, 0);
  BIGNUM_CMP_WITH_INT(b, -1);

  bignum_assign_int(a, INT_MAX);
  bignum_neg(a, b);
  BIGNUM_CMP_WITH_INT(a, INT_MAX);
  BIGNUM_CMP_WITH_INT(b, INT_MIN);

  bignum_assign_int(a, INT_MIN);
  bignum_neg(a, b);
  BIGNUM_CMP_WITH_INT(a, INT_MIN);
  BIGNUM_CMP_WITH_INT(b, INT_MAX);

  bignum_assign_int(a, 65535);  /* 2^16 - 1 */
  bignum_neg(a, b);
  BIGNUM_CMP_WITH_INT(a, 65535);
  BIGNUM_CMP_WITH_INT(b, -65536);

  bignum_assign_int(a, -65535);  /* -2^16 + 1 */
  bignum_neg(a, b);
  BIGNUM_CMP_WITH_INT(a, -65535);
  BIGNUM_CMP_WITH_INT(b, 65534);

  bignum_free(a);
  bignum_free(b);
}

int main(void)
{
  bignum_new_tests();
  bignum_assign_int_tests();
  bignum_assign_str_tests();

  bignum_neg_tests();

  UNIT_STATUS_AND_EXIT;

  return 0;
}

