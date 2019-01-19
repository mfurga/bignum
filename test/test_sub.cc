//
// Copyright (c) 2019 Mateusz Furga <matfurga@gmail.com>
// This software is released under the MIT license (see LICENSE).
// 
// Unit tests for subtraction.
// ================================================================

#include <gtest/gtest.h>
#include "../src/bignum.h"

#define VU32(...) (std::vector<uint32_t>({__VA_ARGS__}))

using namespace bignum;

TEST(BignumSub, SubtractTwoZeros)
{
  Bignum number = 0;

  number.sub(0);
  EXPECT_TRUE(number == 0);
  EXPECT_EQ(number.repr(), VU32(0));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 1);
}

TEST(BignumSub, SubtractNumbersWithPositiveSign)
{
  Bignum number = 8000123;

  number.sub(1000999);
  EXPECT_TRUE(number == 6999124);
  EXPECT_EQ(number.repr(), VU32(6999124));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 7);

  Bignum number2 = "4344892027605525392106078926436593913846";

  number2.sub("8627525041266639822166859196189930534297");
  EXPECT_TRUE(number2 == "-4282633013661114430060780269753336620451");
  EXPECT_EQ(number2.repr(), VU32(
    336620451, 780269753, 114430060, 633013661, 4282));
  EXPECT_EQ(number2.sign, NEGATIVE);
  EXPECT_EQ(number2.size, 40);

  Bignum number3 = "5253921000000000000000000000000123";
  number3.sub("210000000001000000999");
  EXPECT_TRUE(number3 == "5253920999999789999999998999999124");
  EXPECT_EQ(number3.repr(), VU32(999999124, 999999998, 999999789, 5253920));
  EXPECT_EQ(number3.sign, POSITIVE);
  EXPECT_EQ(number3.size, 34);
}

TEST(BignumSub, SubtractNumbersWithPositiveSignBig)
{
  Bignum number = "899725105223393249272042979447335401830409825"
                  "759806064809646388191700531544294703248025715"
                  "514708554908467479369364907126134149304708133"
                  "110584726329448375924275993027649209271459132"
                  "900651636779230279714584383190497275317384781";

  number.sub("906827349072844065005645764444581591578340402"
             "004308576941002041501310149821459993761775102"
             "219929664959191306892643427387187023815552618"
             "992855521919760958891083538200080661700600633"
             "703206926106740285290976264590660786721017148");

  EXPECT_TRUE(number == "-7102243849450815733602784997246189747930576"
                        "244502512131355653309609618277165290513749386"
                        "705221110050723827523278520261052874510844485"
                        "882270795590312582966807545172431452429141500"
                        "802555289327510005576391881400163511403632367");
  EXPECT_EQ(number.repr(), VU32(
    403632367, 400163511, 576391881, 327510005, 802555289,
    429141500, 172431452, 966807545, 590312582, 882270795,
    510844485, 261052874, 523278520,  50723827, 705221110,
    513749386, 277165290, 309609618, 131355653, 244502512,
    747930576, 997246189, 733602784, 849450815, 7102243
  ));
  EXPECT_EQ(number.sign, NEGATIVE);
  EXPECT_EQ(number.size, 5 * 45 - 2);
}

TEST(BignumSub, SubtractNumbersWithPositiveSignReverse)
{
  Bignum zero = 0;

  zero.sub(0, true);
  EXPECT_TRUE(zero == 0);
  EXPECT_EQ(zero.repr(), VU32(0));
  EXPECT_EQ(zero.sign, POSITIVE);
  EXPECT_EQ(zero.size, 1);

  Bignum number = 98810233;

  number.sub(100000999, true);
  EXPECT_TRUE(number == 1190766);
  EXPECT_EQ(number.repr(), VU32(1190766));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 7);

  Bignum number2 = "10000000000000000";

  number2.sub("10000000000000001", true);
  EXPECT_TRUE(number2 == 1);
  EXPECT_EQ(number2.repr(), VU32(1));
  EXPECT_EQ(number2.sign, POSITIVE);
  EXPECT_EQ(number2.size, 1);
}

TEST(BignumSub, SubtractNumbersWithNegativeSign)
{
  Bignum number = -23415;

  number.sub("-283902019371913");
  EXPECT_TRUE(number == "283902019348498");
  EXPECT_EQ(number.repr(), VU32(19348498, 283902));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 15);

  number.sub("283902019348499");
  EXPECT_TRUE(number == -1);
  EXPECT_EQ(number.repr(), VU32(1));
  EXPECT_EQ(number.sign, NEGATIVE);
  EXPECT_EQ(number.size, 1);

  number.sub(-98100000000000);
  EXPECT_TRUE(number == 98099999999999);
  EXPECT_EQ(number.repr(), VU32(999999999, 98099));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 14);
}

TEST(BignumSub, SubtractNumbersWithDifferentSign)
{
  Bignum number = "921758123691574896375";

  number.sub("-991259845559966245");
  EXPECT_TRUE(number == "922749383537134862620");
  EXPECT_EQ(number.repr(), VU32(134862620, 749383537, 922));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 21);

  number.sub("1298391824541257454542");
  EXPECT_TRUE(number == "-375642441004122591922");
  EXPECT_EQ(number.repr(), VU32(122591922, 642441004, 375));
  EXPECT_EQ(number.sign, NEGATIVE);
  EXPECT_EQ(number.size, 21);
}

TEST(BignumSub, SubtractTwoZerosOperator)
{
  Bignum number = 0;

  number -= "0";
  number -= std::string("0");
  number -= 0;
  number -= Bignum(0);

  EXPECT_TRUE(number == 0);
  EXPECT_EQ(number.repr(), VU32(0));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 1);
}

TEST(BignumSub, SubtractNumbersWithPositiveSignOperator)
{
  Bignum number = 98123141;
  number -= 98123141;

  EXPECT_TRUE(number == 0);
  EXPECT_EQ(number.repr(), VU32(0));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 1);

  number -= "276207306607787999875528331";

  EXPECT_TRUE(number == "-276207306607787999875528331");
  EXPECT_EQ(number.repr(), VU32(875528331, 607787999, 276207306));
  EXPECT_EQ(number.sign, NEGATIVE);
  EXPECT_EQ(number.size, 27);
}

TEST(BignumSub, SubtractNumbersWithNegativeSignOperator)
{
  Bignum number = -10000000000000000;
  number -= -999999999999999;

  EXPECT_TRUE(number == "-9000000000000001");
  EXPECT_EQ(number.repr(), VU32(1, 9000000));
  EXPECT_EQ(number.sign, NEGATIVE);
  EXPECT_EQ(number.size, 16);

  number -= "670605669143345208873360365879859";

  EXPECT_TRUE(number == "-670605669143345217873360365879860");
  EXPECT_EQ(number.repr(), VU32(365879860, 217873360, 669143345, 670605));
  EXPECT_EQ(number.sign, NEGATIVE);
  EXPECT_EQ(number.size, 33);
}

TEST(BignumSub, SubtractNumbersWithDifferentSignOperator)
{
  Bignum number = "4578212646929253762498755526";

  number -= "-53132247305338359881";

  EXPECT_TRUE(number == "4578212700061501067837115407");
  EXPECT_EQ(number.repr(), VU32(837115407,  61501067, 578212700, 4));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 28);

  number -= -1;

  EXPECT_TRUE(number == "4578212700061501067837115408");
  EXPECT_EQ(number.repr(), VU32(837115408,  61501067, 578212700, 4));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 28);
}
