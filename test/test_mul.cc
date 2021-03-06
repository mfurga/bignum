//
// Copyright (c) 2019 Mateusz Furga <matfurga@gmail.com>
// This software is released under the MIT license (see LICENSE).
// 
// Unit tests for multiplication.
// ================================================================

#include <gtest/gtest.h>
#include "../src/bignum.h"

#define VU32(...) (std::vector<uint32_t>({__VA_ARGS__}))

using namespace bignum;

TEST(BignumMul, MultiplyTwoZeros)
{
  Bignum number = 0;
  number.mul(0);

  EXPECT_TRUE(number == 0);
  EXPECT_EQ(number.repr(), VU32(0));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 1);
}

TEST(BignumMul, MultiplyNumberWithZero)
{
  Bignum number = "920698931235922310";
  number.mul(0);

  EXPECT_TRUE(number == 0);
  EXPECT_EQ(number.repr(), VU32(0));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 1);

  number = "-625364305637147871048749723451853567";
  number.mul(0);

  EXPECT_TRUE(number == 0);
  EXPECT_EQ(number.repr(), VU32(0));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 1);

  number.mul(1);

  EXPECT_TRUE(number == 0);
  EXPECT_EQ(number.repr(), VU32(0));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 1);

  number.mul(-1);

  EXPECT_TRUE(number == 0);
  EXPECT_EQ(number.repr(), VU32(0));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 1);

  number.mul("31463637078751506859145305315");

  EXPECT_TRUE(number == 0);
  EXPECT_EQ(number.repr(), VU32(0));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 1);

  number.mul("-9980318645145194644931463637078751506859");

  EXPECT_TRUE(number == 0);
  EXPECT_EQ(number.repr(), VU32(0));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 1);
}

TEST(BignumMul, MultiplyNumbersWithPositiveSign)
{
  Bignum number = "8218371298912";

  number.mul("32351231");
  EXPECT_TRUE(number == "265874428334872160672");
  EXPECT_EQ(number.repr(), VU32(872160672, 874428334, 265));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 21);

  number.mul("4008466832875461693485");

  EXPECT_TRUE(number == "1065748827690058922298290281566890135621920");
  EXPECT_EQ(number.repr(), VU32(
    135621920, 281566890, 922298290, 827690058, 1065748));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 43);

  number = "832293660279672509189581663";
  number.mul(Bignum("2840658480438264700090351"));

  EXPECT_TRUE(number == "2364262044288455816261834231491600181537983992833713");
  EXPECT_EQ(number.repr(), VU32(
    992833713, 181537983, 231491600, 816261834, 44288455, 2364262));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 52);
}

TEST(BignumMul, MultiplyNumbersWithPositiveSignBig)
{
  Bignum number = "164665076543125159727257639456409157078170117"
                  "812040900467884178262509853997645954725919499"
                  "944751198602077426807800888779491067362148227"
                  "256524103423941159866286111114120267686277618"
                  "006803316989433495117693971633949258981883586";

  number.mul("111010760145103836112885130659514571055610237"
             "770165278839798968139975159703507986773012541"
             "072989167427142825363461283272220077309055985");

  EXPECT_TRUE(number ==  "18279595316404031036673582057029416229897627"
                        "614537565033757733899865204281984323129255051"
                        "259411193048762643693224413900736839650303737"
                        "664822532402619498683981129367685060016485089"
                        "471865111717600779598722491173179797466309658"
                        "258419101015781751653319327354818216277291616"
                        "841051231158025403158244340905719776912518546"
                        "413486425217846216350234377060622250826562210");
  EXPECT_EQ(number.repr(), VU32(
    826562210,  60622250, 350234377, 217846216, 413486425,
    912518546, 905719776, 158244340, 158025403, 841051231,
    277291616, 354818216, 653319327,  15781751, 258419101,
    466309658, 173179797, 598722491, 717600779, 471865111,
     16485089, 367685060, 683981129, 402619498, 664822532,
    650303737, 900736839, 693224413,  48762643, 259411193,
    129255051, 281984323, 899865204,  33757733, 614537565,
    229897627,  57029416,  36673582, 316404031,  18279595
  ));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 45 * 7 + 44);
}

TEST(BignumMul, MultiplyNumbersWithNegativeSign)
{
  Bignum number = "-292160506000000000000905150";

  number.mul(-1);
  EXPECT_TRUE(number == "292160506000000000000905150");
  EXPECT_EQ(number.repr(), VU32(905150, 0, 292160506));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 27);

  number.mul(-1);
  number.mul("-974403625576632191790000000000");

  EXPECT_TRUE(number == "284682256296703402930137427181690688628398718500000000000");
  EXPECT_EQ(number.repr(), VU32(
    0, 398718500, 690688628, 137427181, 703402930, 682256296, 284));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 57);

  number.mul(-1);
  number.mul("-4307382096053601782488803693948767078308");

  EXPECT_TRUE(number == "1226235"
                        "253836562977983985654263935878440380014873530"
                        "046731483542601001832567736348298000000000000");
  EXPECT_EQ(number.repr(), VU32(
            0, 348298000, 832567736, 542601001,  46731483,
     14873530, 878440380, 654263935, 977983985, 253836562,
      1226235));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 45 * 2 + 7);
}

TEST(BignumMul, MultiplyNumbersWithNegativeSignBig)
{
  Bignum number = "-854337595738090087033677721878789275968629824"
                   "984128630000000000005349472261700183050057112"
                   "960705147812178956962680857997225432965857146"
                   "212585329620378343002643450808237411220810025";

  number.mul( "-2124130512026463652725367030352177145836062"
             "729644512477319362330000000000000000000578761"
             "325941717576957759995316554062254387077628063"
             "990959029838838465359205649158094084276362960"
             "764953013131806931037271194830649235660318637");
  
  EXPECT_TRUE(number ==   "1814724554678607207916808244564052466566852"
                        "907693996465076999319669545380235206011720331"
                        "331708477507048876365005488390715827839225192"
                        "122873491946249083413417059229093300099729355"
                        "297490869070696655660021980897009184245408290"
                        "065599541324550959912041633364435607579611288"
                        "697277948469888810366098500702706004294620865"
                        "653726341716923942067127530458986500498280114"
                        "311495871348398410442537989096314656743935925");
  EXPECT_EQ(number.repr(), VU32(
    743935925,  96314656, 442537989, 348398410, 311495871,
    498280114, 458986500,  67127530, 716923942, 653726341,
    294620865, 702706004, 366098500, 469888810, 697277948,
    579611288, 364435607, 912041633, 324550959,  65599541,
    245408290, 897009184, 660021980,  70696655, 297490869,
     99729355, 229093300, 413417059, 946249083, 122873491,
    839225192, 390715827, 365005488, 507048876, 331708477,
     11720331, 380235206, 319669545, 465076999, 907693996,
    466566852, 244564052, 207916808, 554678607,   1814724
    ));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 45 * 8 + 43);
}

TEST(BignumMul, MultiplyNumbersWithDifferentSign)
{
  Bignum number = 1;
  number.mul(-1);

  EXPECT_TRUE(number == -1);
  EXPECT_EQ(number.repr(), VU32(1));
  EXPECT_EQ(number.sign, NEGATIVE);
  EXPECT_EQ(number.size, 1);

  number.mul("322989437981311695777247646");

  EXPECT_TRUE(number == "-322989437981311695777247646");
  EXPECT_EQ(number.repr(), VU32(777247646, 981311695, 322989437));
  EXPECT_EQ(number.sign, NEGATIVE);
  EXPECT_EQ(number.size, 27);

  number.mul(1);

  EXPECT_TRUE(number == "-322989437981311695777247646");
  EXPECT_EQ(number.repr(), VU32(777247646, 981311695, 322989437));
  EXPECT_EQ(number.sign, NEGATIVE);
  EXPECT_EQ(number.size, 27);

  number.mul("65098594401312594207787181514");

  EXPECT_TRUE(number == "-21026158419053318928849944076243482066931827930731216044");
  EXPECT_EQ(number.repr(), VU32(
    731216044, 931827930, 243482066, 849944076,  53318928,
    26158419,        21));
  EXPECT_EQ(number.sign, NEGATIVE);
  EXPECT_EQ(number.size, 56);
}

TEST(BignumMul, MultiplyTwoZerosOperator)
{
  Bignum number = 0;
  number *= 0;

  EXPECT_TRUE(number == 0);
  EXPECT_EQ(number.repr(), VU32(0));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 1);
}

TEST(BignumMul, MultiplyNumberWithZeroOperator)
{
  Bignum number = 459292727602571;
  number *= 0;

  EXPECT_TRUE(number == 0);
  EXPECT_EQ(number.repr(), VU32(0));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 1);

  number *= "550122939309917072562206531";

  EXPECT_TRUE(number == 0);
  EXPECT_EQ(number.repr(), VU32(0));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 1);

  number = -1;
  number *= 0;

  EXPECT_TRUE(number == 0);
  EXPECT_EQ(number.repr(), VU32(0));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 1);
}

TEST(BignumMul, MultiplyNumbersWithPositiveSignOperator)
{
  Bignum number = "55784645421813027272715";
  number *= "62549630806559172100768";

  EXPECT_TRUE(number == "3489308975809216212440645511209683724196945120");
  EXPECT_EQ(number.repr(), VU32(
    196945120, 209683724, 440645511, 809216212, 489308975, 3));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 46);

  number *= 1;

  EXPECT_TRUE(number == "3489308975809216212440645511209683724196945120");
  EXPECT_EQ(number.repr(), VU32(
    196945120, 209683724, 440645511, 809216212, 489308975, 3));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 46);

  number *= 0;

  EXPECT_TRUE(number == 0);
  EXPECT_EQ(number.repr(), VU32(0));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 1);

  Bignum number2 = "707925712823816168800000000000";
  number2 *= number;

  EXPECT_TRUE(number2 == 0);
  EXPECT_EQ(number2.repr(), VU32(0));
  EXPECT_EQ(number2.sign, POSITIVE);
  EXPECT_EQ(number2.size, 1);
}

TEST(BignumMul, MultiplyNumbersWithNegativeSignOperator)
{
  Bignum number = "-88251368788097031084082310012";
  number *= std::string("-99641272027186649556000000000");

  EXPECT_TRUE(number == "8793478644186345672720810730500999796155194154672000000000");
  EXPECT_EQ(number.repr(), VU32(
            0, 194154672, 999796155, 810730500, 345672720,
    478644186,      8793,));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 58);

  number = -1;
  number *= std::string("-100000");

  EXPECT_TRUE(number == "100000");
  EXPECT_EQ(number.repr(), VU32(100000));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 6);

  number *= -1;

  Bignum number2 = "-9961592990497139382044582140";
  number2 *= number;

  EXPECT_TRUE(number2 == "996159299049713938204458214000000");
  EXPECT_EQ(number2.repr(), VU32(214000000, 938204458, 299049713, 996159));
  EXPECT_EQ(number2.sign, POSITIVE);
  EXPECT_EQ(number2.size, 33);
}

TEST(BignumMul, MultiplyNumbersWithDifferentSignOperator)
{
  Bignum number = "831775756193665995";
  number *= -0x7fffffffffffffff;

  EXPECT_TRUE(number == "-7671777250610394531850591919164582965");
  EXPECT_EQ(number.repr(), VU32(164582965, 850591919, 610394531, 671777250, 7));
  EXPECT_EQ(number.sign, NEGATIVE);
  EXPECT_EQ(number.size, 37);

  number *= -1;

  EXPECT_TRUE(number == "7671777250610394531850591919164582965");
  EXPECT_EQ(number.repr(), VU32(164582965, 850591919, 610394531, 671777250, 7));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 37);

  number *= "-2";

  EXPECT_TRUE(number == "-15343554501220789063701183838329165930"); 
  EXPECT_EQ(number.repr(), VU32(329165930, 701183838, 220789063, 343554501, 15));
  EXPECT_EQ(number.sign, NEGATIVE);
  EXPECT_EQ(number.size, 38);

  number *= "9640694159795129495438938172";

  EXPECT_TRUE(number == "-147922516270417532244590616608035660356984223407924252977598879960");
  EXPECT_EQ(number.repr(), VU32(
    598879960, 924252977, 984223407,  35660356, 590616608,
    417532244, 922516270,       147));
  EXPECT_EQ(number.sign, NEGATIVE);
  EXPECT_EQ(number.size, 66);
}
