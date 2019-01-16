//
// Copyright (c) 2019 Mateusz Furga <matfurga@gmail.com>
// This software is released under the MIT license (see LICENSE).
// 
// Unit tests for number initialization.
// ================================================================

#include <gtest/gtest.h>
#include "../src/bignum.h"

#define VU32(...) (std::vector<uint32_t>({__VA_ARGS__}))

using namespace bignum;

TEST(BignumInit, InitStartsEmptyNumber)
{
  Bignum number;
  EXPECT_EQ(number.repr(), VU32());
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 0);
}

TEST(BignumInit, InitStartsNotEmptyNumberAsInt)
{
  Bignum number = 4123;

  EXPECT_TRUE(number == 4123);
  EXPECT_EQ(number.repr(), VU32(4123));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 4);

  Bignum number_positive = 39578489007577149;

  EXPECT_TRUE(number_positive == "39578489007577149");
  EXPECT_EQ(number_positive.size, 17);
  EXPECT_EQ(number_positive.sign, POSITIVE);
  EXPECT_EQ(number_positive.repr(), VU32(7577149, 39578489));

  Bignum number_positive_zero = 0;

  EXPECT_TRUE(number_positive_zero == 0);
  EXPECT_EQ(number_positive_zero.repr(), VU32(0));
  EXPECT_EQ(number_positive_zero.sign, POSITIVE);
  EXPECT_EQ(number_positive_zero.size, 1);

  Bignum number_negative = -855950164990988300;

  EXPECT_TRUE(number_negative == -855950164990988300);
  EXPECT_EQ(number_negative.repr(), VU32(990988300, 855950164));
  EXPECT_EQ(number_negative.sign, NEGATIVE);
  EXPECT_EQ(number_negative.size, 18);

  Bignum number_negative_one = -1;

  EXPECT_TRUE(number_negative_one == -1);
  EXPECT_EQ(number_negative_one.repr(), VU32(1));
  EXPECT_EQ(number_negative_one.sign, NEGATIVE);
  EXPECT_EQ(number_negative_one.size, 1);

  Bignum number_positive_one = 1;

  EXPECT_TRUE(number_positive_one == 1);
  EXPECT_EQ(number_positive_one.size, 1);
  EXPECT_EQ(number_positive_one.sign, POSITIVE);
  EXPECT_EQ(number_positive_one.repr(), VU32(1));
}

TEST(BignumInit, InitStartsNotEmptyNumberAsCString)
{
  Bignum number_positive = "65432787129128912839128329831938195831";

  EXPECT_TRUE(number_positive == "65432787129128912839128329831938195831");
  EXPECT_EQ(number_positive.size, 38);
  EXPECT_EQ(number_positive.sign, POSITIVE);
  EXPECT_EQ(number_positive.repr(), VU32(
    938195831, 128329831, 128912839, 432787129, 65));

  Bignum number_positive_big = "198423740130123037023010001072301723817310010"
                               "823710370000001723182371231012901820840128312"
                               "012901820840128312309123871238712837123101230"
                               "827301287301301237018273011298129182981298129"
                               "010019201295238103183899181923103012837124012";

  EXPECT_EQ(number_positive_big.repr(), VU32(
    837124012, 923103012, 183899181, 295238103,  10019201,
    981298129, 298129182,  18273011, 301301237, 827301287,
    123101230, 238712837, 309123871, 840128312,  12901820,
    840128312,  12901820, 182371231,      1723, 823710370,
    817310010,  72301723,  23010001, 130123037, 198423740
  ));
  EXPECT_EQ(number_positive_big.sign, POSITIVE);
  EXPECT_EQ(number_positive_big.size, 45 * 5);

  Bignum number_negative = "-2938198349834000839100002283900000312983";

  EXPECT_TRUE(number_negative == "-2938198349834000839100002283900000312983");
  EXPECT_EQ(number_negative.repr(), VU32(
    312983, 2283900, 839100, 198349834, 2938));
  EXPECT_EQ(number_negative.sign, NEGATIVE);
  EXPECT_EQ(number_negative.size, 40);

  Bignum number_negative_big = "-874817217283718728172817281271497127192793917"
                                "485323981230100401004010239120452060340247340"
                                "000001231230000000000004981349123912839859170"
                                "219381293010230000003452411039819491491491419"
                                "000000000000000000000000000000000000000000000";

  EXPECT_EQ(number_negative_big.repr(), VU32(
            0,         0,         0,         0,         0,
    491491419,  39819491,   3452411,  10230000, 219381293,
    839859170, 349123912,      4981, 230000000,      1231,
    340247340, 120452060,   4010239, 230100401, 485323981,
    192793917, 271497127, 172817281, 283718728, 874817217
  ));
  EXPECT_EQ(number_negative_big.sign, NEGATIVE);
  EXPECT_EQ(number_negative_big.size, 45 * 5);

  Bignum number_positive_zero = "0";

  EXPECT_TRUE(number_positive_zero == "0");
  EXPECT_EQ(number_positive_zero.repr(), VU32(0));
  EXPECT_EQ(number_positive_zero.sign, POSITIVE);
  EXPECT_EQ(number_positive_zero.size, 1);

  Bignum number_negative_one = "-1";

  EXPECT_TRUE(number_negative_one == "-1");
  EXPECT_EQ(number_negative_one.repr(), VU32(1));
  EXPECT_EQ(number_negative_one.sign, NEGATIVE);
  EXPECT_EQ(number_negative_one.size, 1);

  Bignum number_positive_one = "1";

  EXPECT_TRUE(number_positive_one == "1");
  EXPECT_EQ(number_positive_one.repr(), VU32(1));
  EXPECT_EQ(number_positive_one.sign, POSITIVE);
  EXPECT_EQ(number_positive_one.size, 1);
}

TEST(BignumInit, InitStartsNotEmptyNumberAsStdString)
{
  Bignum number_positive = std::string("90776684760217149920");

  EXPECT_TRUE(number_positive == "90776684760217149920");
  EXPECT_EQ(number_positive.repr(), VU32(217149920, 776684760, 90));
  EXPECT_EQ(number_positive.sign, POSITIVE);
  EXPECT_EQ(number_positive.size, 20);

  Bignum number_negative = std::string("-8185292020046263238612");

  EXPECT_TRUE(number_negative == "-8185292020046263238612");
  EXPECT_EQ(number_negative.repr(), VU32(263238612, 292020046, 8185));
  EXPECT_EQ(number_negative.sign, NEGATIVE);
  EXPECT_EQ(number_negative.size, 22);

  Bignum number_positive_zero = std::string("0");

  EXPECT_TRUE(number_positive_zero == "0");
  EXPECT_EQ(number_positive_zero.repr(), VU32(0));
  EXPECT_EQ(number_positive_zero.sign, POSITIVE);
  EXPECT_EQ(number_positive_zero.size, 1);

  Bignum number_positive_one = std::string("1");

  EXPECT_TRUE(number_positive_one == "1");
  EXPECT_EQ(number_positive_one.repr(), VU32(1));
  EXPECT_EQ(number_positive_one.sign, POSITIVE);
  EXPECT_EQ(number_positive_one.size, 1);

  Bignum number_negative_one = std::string("-1");

  EXPECT_TRUE(number_negative_one == "-1");
  EXPECT_EQ(number_negative_one.repr(), VU32(1));
  EXPECT_EQ(number_negative_one.sign, NEGATIVE);
  EXPECT_EQ(number_negative_one.size, 1);
}

TEST(BignumInit, InitStartsEmptyNumberMaxInt)
{
  Bignum number_positive_max = 0x7fffffffffffffff;

  EXPECT_TRUE(number_positive_max == "9223372036854775807");
  EXPECT_EQ(number_positive_max.repr(), VU32(854775807, 223372036, 9));
  EXPECT_EQ(number_positive_max.sign, POSITIVE);
  EXPECT_EQ(number_positive_max.size, 19);

  Bignum number_negative = -0x7fffffffffffffff;

  EXPECT_TRUE(number_negative == "-9223372036854775807");
  EXPECT_EQ(number_negative.repr(), VU32(854775807, 223372036, 9));
  EXPECT_EQ(number_negative.sign, NEGATIVE);
  EXPECT_EQ(number_negative.size, 19);

  Bignum number_negative_max = static_cast<int64_t>(-0x8000000000000000);

  EXPECT_TRUE(number_negative_max == "-9223372036854775808");
  EXPECT_EQ(number_negative_max.repr(), VU32(854775808, 223372036, 9));
  EXPECT_EQ(number_negative_max.sign, NEGATIVE);
  EXPECT_EQ(number_negative_max.size, 19);
}
