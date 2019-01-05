#include <gtest/gtest.h>

#include "../src/bignum.h"

using namespace bignum;

TEST(Bignum, InitStartsEmptyNumber)
{
  Bignum number;
  EXPECT_EQ(number.size, 0);
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.get_number_repr(), std::vector<uint32_t>());
}

TEST(Bignum, InitStartsNotEmptyNumberAsInt)
{
  Bignum number_positive = 391283092831231;
  EXPECT_EQ(number_positive.size, 15);
  EXPECT_EQ(number_positive.sign, POSITIVE);
  EXPECT_EQ(number_positive.get_number_repr(), std::vector<uint32_t>({
    92831231, 391283
  }));

  Bignum number_positive_zero = 0;
  EXPECT_EQ(number_positive_zero.size, 1);
  EXPECT_EQ(number_positive_zero.sign, POSITIVE);
  EXPECT_EQ(number_positive_zero.get_number_repr(), std::vector<uint32_t>({
    0
  }));

  Bignum number_negative = -1028192891009847129;
  EXPECT_EQ(number_negative.size, 19);
  EXPECT_EQ(number_negative.sign, NEGATIVE);
  EXPECT_EQ(number_negative.get_number_repr(), std::vector<uint32_t>({
    9847129, 28192891, 1
  }));

  Bignum number_negative_one = -1;
  EXPECT_EQ(number_negative_one.size, 1);
  EXPECT_EQ(number_negative_one.sign, NEGATIVE);
  EXPECT_EQ(number_negative_one.get_number_repr(), std::vector<uint32_t>({
    1
  }));

  Bignum number_positive_one = 1;
  EXPECT_EQ(number_positive_one.size, 1);
  EXPECT_EQ(number_positive_one.sign, POSITIVE);
  EXPECT_EQ(number_positive_one.get_number_repr(), std::vector<uint32_t>({
    1
  }));
}

TEST(Bignum, InitStartsNotEmptyNumberAsCString)
{
  Bignum number_positive = "65432787129128912839128329831938195831";
  EXPECT_EQ(number_positive.size, 38);
  EXPECT_EQ(number_positive.sign, POSITIVE);
  EXPECT_EQ(number_positive.get_number_repr(), std::vector<uint32_t>({
    938195831, 128329831, 128912839, 432787129, 65
  }));

  Bignum number_positive_big = ("198423740130123037023010001072301723817310010"
                                "823710370000001723182371231012901820840128312"
                                "012901820840128312309123871238712837123101230"
                                "827301287301301237018273011298129182981298129"
                                "010019201295238103183899181923103012837124012");
  EXPECT_EQ(number_positive_big.size, 45 * 5);
  EXPECT_EQ(number_positive_big.sign, POSITIVE);
  EXPECT_EQ(number_positive_big.get_number_repr(), std::vector<uint32_t>({
    837124012, 923103012, 183899181, 295238103,  10019201,
    981298129, 298129182,  18273011, 301301237, 827301287,
    123101230, 238712837, 309123871, 840128312,  12901820,
    840128312,  12901820, 182371231,      1723, 823710370,
    817310010,  72301723,  23010001, 130123037, 198423740
  }));

  Bignum number_negative = "-2938198349834000839100002283900000312983";
  EXPECT_EQ(number_negative.size, 40);
  EXPECT_EQ(number_negative.sign, NEGATIVE);
  EXPECT_EQ(number_negative.get_number_repr(), std::vector<uint32_t>({
    312983, 2283900, 839100, 198349834, 2938
  }));

  Bignum number_negative_big = ("-874817217283718728172817281271497127192793917"
                                 "485323981230100401004010239120452060340247340"
                                 "000001231230000000000004981349123912839859170"
                                 "219381293010230000003452411039819491491491419"
                                 "000000000000000000000000000000000000000000000");
  EXPECT_EQ(number_negative_big.size, 45 * 5);
  EXPECT_EQ(number_negative_big.sign, NEGATIVE);
  EXPECT_EQ(number_negative_big.get_number_repr(), std::vector<uint32_t>({
            0,         0,         0,         0,         0,
    491491419,  39819491,   3452411,  10230000, 219381293,
    839859170, 349123912,      4981, 230000000,      1231,
    340247340, 120452060,   4010239, 230100401, 485323981,
    192793917, 271497127, 172817281, 283718728, 874817217
  }));

  Bignum number_positive_zero = "0";
  EXPECT_EQ(number_positive_zero.size, 1);
  EXPECT_EQ(number_positive_zero.sign, POSITIVE);
  EXPECT_EQ(number_positive_zero.get_number_repr(), std::vector<uint32_t>({
    0
  }));

  Bignum number_negative_one = "-1";
  EXPECT_EQ(number_negative_one.size, 1);
  EXPECT_EQ(number_negative_one.sign, NEGATIVE);
  EXPECT_EQ(number_negative_one.get_number_repr(), std::vector<uint32_t>({
    1
  }));

  Bignum number_positive_one = "1";
  EXPECT_EQ(number_positive_one.size, 1);
  EXPECT_EQ(number_positive_one.sign, POSITIVE);
  EXPECT_EQ(number_positive_one.get_number_repr(), std::vector<uint32_t>({
    1
  }));
}

TEST(Bignum, InitStartsNotEmptyNumberAsStdString)
{
  Bignum number_positive = std::string("9182981300000198219");
  EXPECT_EQ(number_positive.size, 19);
  EXPECT_EQ(number_positive.sign, POSITIVE);
  EXPECT_EQ(number_positive.get_number_repr(), std::vector<uint32_t>({
    198219, 182981300, 9,
  }));

  Bignum number_negative = std::string("-1283192192891829182912");
  EXPECT_EQ(number_negative.size, 22);
  EXPECT_EQ(number_negative.sign, NEGATIVE);
  EXPECT_EQ(number_negative.get_number_repr(), std::vector<uint32_t>({
    829182912, 192192891, 1283
  }));

  Bignum number_positive_zero = std::string("0");
  EXPECT_EQ(number_positive_zero.size, 1);
  EXPECT_EQ(number_positive_zero.sign, POSITIVE);
  EXPECT_EQ(number_positive_zero.get_number_repr(), std::vector<uint32_t>({
    0
  }));

  Bignum number_positive_one = std::string("1");
  EXPECT_EQ(number_positive_one.size, 1);
  EXPECT_EQ(number_positive_one.sign, POSITIVE);
  EXPECT_EQ(number_positive_one.get_number_repr(), std::vector<uint32_t>({
    1
  }));

  Bignum number_negative_one = std::string("-1");
  EXPECT_EQ(number_negative_one.size, 1);
  EXPECT_EQ(number_negative_one.sign, NEGATIVE);
  EXPECT_EQ(number_negative_one.get_number_repr(), std::vector<uint32_t>({
    1
  }));
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}