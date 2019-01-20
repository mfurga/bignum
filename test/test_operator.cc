//
// Copyright (c) 2019 Mateusz Furga <matfurga@gmail.com>
// This software is released under the MIT license (see LICENSE).
// 
// Unit tests for operators.
// ================================================================

#include <gtest/gtest.h>
#include "../src/bignum.h"

using namespace bignum;

TEST(BignumOperator, OperatorGreater)
{
  Bignum number = 123981;

  EXPECT_TRUE(number > 0);
  EXPECT_TRUE(number > Bignum(-1));
  EXPECT_TRUE(number > 123980);
  EXPECT_TRUE(number > "123881");
  EXPECT_FALSE(number > "123981");
  EXPECT_FALSE(number > "123982");
  EXPECT_FALSE(number > "8977985769209667086665608083912137176498"
                        "8403223910475617226466455607362045261905");

  number = "-3336503050879707279381497";

  EXPECT_TRUE(number > "-3336503050879707279381498");
  EXPECT_TRUE(number > "-3336503050879708279381497");
  EXPECT_FALSE(number > 0);
  EXPECT_FALSE(number > 1);
  EXPECT_FALSE(number > "-3336503050879707279381497");
  EXPECT_FALSE(number > "3336503050879707279381496");
}

TEST(BignumOperator, OperatorGreaterOrEqual)
{
  Bignum number = "81497808982872143213";

  EXPECT_TRUE(number >= 0);
  EXPECT_TRUE(number >= -1);
  EXPECT_TRUE(number >= 2143213);
  EXPECT_TRUE(number >= -780891);
  EXPECT_TRUE(number >= "81497808982872143212");
  EXPECT_TRUE(number >= "81497808982872143213");

  EXPECT_FALSE(number >= "81497808982872143214");
  EXPECT_FALSE(number >= "81497808982872143215");
  EXPECT_FALSE(number >= (Bignum(1) + "81497808982872143213"));
}

TEST(BignumOperator, OperatorLess)
{
  Bignum number = -91283;

  EXPECT_TRUE(number < 0);
  EXPECT_TRUE(number < -1);
  EXPECT_TRUE(number < 1);
  EXPECT_TRUE(number < 91282);
  EXPECT_TRUE(number < -91282);
  EXPECT_TRUE(number < -91183);

  EXPECT_FALSE(number < -91283);
  EXPECT_FALSE(number < -91284);
}

TEST(BignumOperator, OperatorLessOrEqual)
{
  Bignum number = 0;

  EXPECT_TRUE(number <= 0);
  EXPECT_TRUE(number <= 1);
  EXPECT_TRUE(number <= "192941902392840129");

  EXPECT_FALSE(number <= -1);
  EXPECT_FALSE(number <= -9);

  number = "198298412093012";

  EXPECT_TRUE(number <= "198298412093013");
  EXPECT_TRUE(number <= "198298412093012");

  EXPECT_FALSE(number <= 0);
  EXPECT_FALSE(number <= -1);
  EXPECT_FALSE(number <= "-198298412093012");
}
