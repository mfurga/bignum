//
// Copyright (c) 2019 Mateusz Furga <matfurga@gmail.com>
// This software is released under the MIT license (see LICENSE).
// 
// Unit tests for addition.
// ================================================================

#include <gtest/gtest.h>
#include "../src/bignum.h"

#define VU32(...) (std::vector<uint32_t>({__VA_ARGS__}))

using namespace bignum;

TEST(BignumAdd, AddTwoZeros)
{
  Bignum number = 0;

  number.add(0);

  EXPECT_TRUE(number == 0);
  EXPECT_EQ(number.repr(), VU32(0));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 1);
}

TEST(BignumAdd, AddNumbersWithPostivieSign)
{
  Bignum number_positive = "9744892027605525392106078926436593913846";
  number_positive.add("8627525041266639822166859196189930534297");

  EXPECT_TRUE(number_positive == "18372417068872165214272938122626524448143");
  EXPECT_EQ(number_positive.repr(), VU32(
    524448143, 938122626, 165214272, 417068872, 18372));
  EXPECT_EQ(number_positive.sign, POSITIVE);
  EXPECT_EQ(number_positive.size, 41);

  number_positive.add(0);

  EXPECT_TRUE(number_positive == "18372417068872165214272938122626524448143");
  EXPECT_EQ(number_positive.repr(), VU32(
    524448143, 938122626, 165214272, 417068872, 18372));
  EXPECT_EQ(number_positive.sign, POSITIVE);
  EXPECT_EQ(number_positive.size, 41);

  number_positive.add("999999000000000000000000000000000000000000");

  EXPECT_TRUE(number_positive == "1018371417068872165214272938122626524448143");
  EXPECT_EQ(number_positive.repr(), VU32(
    524448143, 938122626, 165214272, 417068872, 1018371));
  EXPECT_EQ(number_positive.sign, POSITIVE);
  EXPECT_EQ(number_positive.size, 43);
}

TEST(BignumAdd, AddNumbersWithPostivieSignBig)
{
  Bignum number_positive_big = "768663491179321245108633592689956254657415731"
                               "779052948794365077176054842706954455743645323"
                               "334231745214326880541466899627205388633735392"
                               "949226431342597034456155045394497769936737009"
                               "258664709878783553385380460948334602575745970";

  number_positive_big.add("962726459720235844266019055133618607936911574"
                          "290478728755116007095335898961592023880010093"
                          "867459360564457124392493487005141055610730938"
                          "441992404813604515150795892734717286719048812"
                          "075968038249316371121246871013070174178412344");

  EXPECT_TRUE(number_positive_big == "1731389950899557089374652647823574862594327306"
                                      "069531677549481084271390741668546479623655417"
                                      "201691105778784004933960386632346444244466331"
                                      "391218836156201549606950938129215056655785821"
                                      "334632748128099924506627331961404776754158314");
  EXPECT_EQ(number_positive_big.repr(), VU32(
    754158314, 961404776, 506627331, 128099924, 334632748,
    655785821, 129215056, 606950938, 156201549, 391218836,
    244466331, 632346444, 933960386, 778784004, 201691105,
    623655417, 668546479, 271390741, 549481084,  69531677,
    594327306, 823574862, 374652647, 899557089, 731389950, 1));
  EXPECT_EQ(number_positive_big.sign, POSITIVE);
  EXPECT_EQ(number_positive_big.size, 5 * 45 + 1);
}

TEST(BignumAdd, AddNumbersWithNegativeSign)
{
  Bignum number_negative = -1;
  number_negative.add(-1);

  EXPECT_TRUE(number_negative == -2);
  EXPECT_EQ(number_negative.repr(), VU32(2));
  EXPECT_EQ(number_negative.size, 1);
  EXPECT_EQ(number_negative.sign, NEGATIVE);

  number_negative.add("-7776007467881184677682575033360149");

  EXPECT_TRUE(number_negative == "-7776007467881184677682575033360151");
  EXPECT_EQ(number_negative.repr(), VU32(
    33360151, 677682575, 467881184, 7776007));
  EXPECT_EQ(number_negative.sign, NEGATIVE);
  EXPECT_EQ(number_negative.size, 34);

  number_negative.add("-9187465947709705071157097142332847884");

  EXPECT_TRUE(number_negative == "-9195241955177586255834779717366208035");
  EXPECT_EQ(number_negative.repr(), VU32(
    366208035, 834779717, 177586255, 195241955, 9));
  EXPECT_EQ(number_negative.sign, NEGATIVE);
  EXPECT_EQ(number_negative.size, 37);
}

TEST(BignumAdd, AddNumbersWithNegativeSignBig)
{
  Bignum number_negative_big = "-911122553795495168964594279051746789601497396"
                                "849487332910239568180296577873763741838737384"
                                "902878793113378019814867378574339590055967261"
                                "046860149364912587537235185206873111739649515"
                                "662370814987417882629262844781882794657235853";

  number_negative_big.add("-9934276287010054164147647711152395006583668393"
                            "800347699448025963090957807544717126171193490"
                            "932709713854454369965382353324026244864844810"
                            "616032733490953093878417418278564916630042468"
                            "837021724858844249187991287667351745676224071");

  EXPECT_TRUE(number_negative_big == "-10845398840805549333112241990204141796185165790"
                                        "649835032358265531271254385418480868009930875"
                                        "835588506967832389780249731898365834920812071"
                                        "662892882855865681415652603485438028369691984"
                                        "499392539846262131817254132449234540333459924");
  EXPECT_EQ(number_negative_big.repr(), VU32(
    333459924, 449234540, 817254132, 846262131, 499392539,
    369691984, 485438028, 415652603, 855865681, 662892882,
    920812071, 898365834, 780249731, 967832389, 835588506,
      9930875, 418480868, 271254385, 358265531, 649835032,
    185165790, 204141796, 112241990, 805549333, 845398840, 10));
  EXPECT_EQ(number_negative_big.sign, NEGATIVE);
  EXPECT_EQ(number_negative_big.size, 5 * 45 + 2);
}

TEST(BignumAdd, AddNumbersWithDifferentSign)
{
  Bignum number = 1298391283;
  number.add("-54719180335789878364");

  EXPECT_TRUE(number == "-54719180334491487081");
  EXPECT_EQ(number.repr(), VU32(491487081, 719180334, 54));
  EXPECT_EQ(number.sign, NEGATIVE);
  EXPECT_EQ(number.size, 20);

  number.add("54719180334491487081");

  EXPECT_TRUE(number == 0);
  EXPECT_EQ(number.repr(), VU32(0));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 1);

  number.add(-1);

  EXPECT_TRUE(number == -1);
  EXPECT_EQ(number.repr(), VU32(1));
  EXPECT_EQ(number.sign, NEGATIVE);
  EXPECT_EQ(number.size, 1);

  number.add(1);

  EXPECT_TRUE(number == 0);
  EXPECT_EQ(number.repr(), VU32(0));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 1);

  number.add("-68721701084578848266");

  EXPECT_TRUE(number == "-68721701084578848266");
  EXPECT_EQ(number.repr(), VU32(578848266, 721701084, 68));
  EXPECT_EQ(number.sign, NEGATIVE);
  EXPECT_EQ(number.size, 20);

  number.add("68721701084578848265");

  EXPECT_TRUE(number == -1);
  EXPECT_EQ(number.repr(), VU32(1));
  EXPECT_EQ(number.sign, NEGATIVE);
  EXPECT_EQ(number.size, 1);
}

TEST(BignumAdd, AddNumbersWithDifferentSignBig)
{
  Bignum number_big = "437077265578085174001862394625344405874109873"
                      "885794217599617983969274534222814952905178872"
                      "142094578705794275912276650352513973172093622"
                      "610221728942613882729237213738818500265101166"
                      "253302491784027344410319457405642082964772489";

  number_big.add("-834485322151135799896773293451687495409228824"
                  "371752748715498733111612285355090995500540255"
                  "075985177206485632132683388091645824314885086"
                  "795341310174491657801418818908278590545618672"
                  "276259763129865674099626535575285098535630549");
  
  EXPECT_TRUE(number_big == "-397408056573050625894910898826343089535118950"
                             "485958531115880749142337751132276042595361382"
                             "933890598500691356220406737739131851142791464"
                             "185119581231877775072181605169460090280517506"
                             "022957271345838329689307078169643015570858060");
  EXPECT_EQ(number_big.repr(), VU32(
    570858060, 169643015, 689307078, 345838329,  22957271,
    280517506, 169460090,  72181605, 231877775, 185119581,
    142791464, 739131851, 220406737, 500691356, 933890598,
    595361382, 132276042, 142337751, 115880749, 485958531,
    535118950, 826343089, 894910898, 573050625, 397408056
  ));
  EXPECT_EQ(number_big.sign, NEGATIVE);
  EXPECT_EQ(number_big.size, 45 * 5);

  number_big.add("397408056573050625894910898826343089535118950"
                 "485958531115880749142337751132276042595361382"
                 "933890598500691356220406737739131851142791464"
                 "185119581231877775072181605169460090280517506"
                 "022957271345838329689307078169643015570858060");
  EXPECT_TRUE(number_big == 0);
  EXPECT_EQ(number_big.repr(), VU32(0));
  EXPECT_EQ(number_big.sign, POSITIVE);
  EXPECT_EQ(number_big.size, 1);
}

TEST(BignumAdd, AddTwoZerosOperator)
{
  Bignum number = 0;

  number += 0;
  number += "0";
  number += std::string("0");
  number += number;

  EXPECT_TRUE(number == 0);
  EXPECT_EQ(number.repr(), VU32(0));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 1);
}

TEST(BignumAdd, AddNumbersWithPostivieSignOperator)
{
  Bignum number_positive = "9744892027605525392106078926436593913846";
  number_positive += "8627525041266639822166859196189930534297";

  EXPECT_TRUE(number_positive == "18372417068872165214272938122626524448143");
  EXPECT_EQ(number_positive.repr(), VU32(
    524448143, 938122626, 165214272, 417068872, 18372));
  EXPECT_EQ(number_positive.sign, POSITIVE);
  EXPECT_EQ(number_positive.size, 41);

  number_positive += 0;

  EXPECT_TRUE(number_positive == "18372417068872165214272938122626524448143");
  EXPECT_EQ(number_positive.repr(), VU32(
    524448143, 938122626, 165214272, 417068872, 18372));
  EXPECT_EQ(number_positive.sign, POSITIVE);
  EXPECT_EQ(number_positive.size, 41);

  number_positive += std::string("10000000000099999999");

  EXPECT_TRUE(number_positive == "18372417068872165214282938122626624448142");
  EXPECT_EQ(number_positive.repr(), VU32(
    624448142, 938122626, 165214282, 417068872, 18372));
  EXPECT_EQ(number_positive.sign, POSITIVE);
  EXPECT_EQ(number_positive.size, 41);
}

TEST(BignumAdd, AddNumbersWithNegativeSignOperator)
{
  Bignum number;
  number = -1;

  number += -1;

  EXPECT_TRUE(number == "-2");
  EXPECT_EQ(number.repr(), VU32(2));
  EXPECT_EQ(number.sign, NEGATIVE);
  EXPECT_EQ(number.size, 1);

  number += "-6584206686066855598285259874175574224035";

  EXPECT_TRUE(number == "-6584206686066855598285259874175574224037");
  EXPECT_EQ(number.repr(), VU32(
    574224037, 259874175, 855598285, 206686066, 6584));
  EXPECT_EQ(number.sign, NEGATIVE);
  EXPECT_EQ(number.size, 40); 

  Bignum number_negative = "-77333970574883073173820193";
  number += number_negative;

  EXPECT_TRUE(number == "-6584206686066932932255834757248748044230");
  EXPECT_EQ(number.repr(), VU32(
    748044230, 834757248, 932932255, 206686066, 6584));
  EXPECT_EQ(number.sign, NEGATIVE);
  EXPECT_EQ(number.size, 40);
}

TEST(BignumAdd, AddNumbersWithDifferentSignOperator)
{
  Bignum number = 9605751714;
  number += -9605751714;

  EXPECT_TRUE(number == 0);
  EXPECT_EQ(number.repr(), VU32(0));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 1);

  number += "-443866550854727184307283043324628102";

  EXPECT_TRUE(number == "-443866550854727184307283043324628102");
  EXPECT_EQ(number.repr(), VU32(
    324628102, 307283043, 854727184, 443866550));
  EXPECT_EQ(number.sign, NEGATIVE);
  EXPECT_EQ(number.size, 36);

  Bignum number_positive = "443866550854727184307283043324628102";
  number += number_positive;

  EXPECT_TRUE(number == 0);
  EXPECT_EQ(number.repr(), VU32(0));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 1);
}

TEST(BignumAdd, AddNumbersIncrementOperator)
{
  Bignum number = -1;

  number++;

  EXPECT_TRUE(number == 0);
  EXPECT_EQ(number.repr(), VU32(0));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 1);

  for (int i = 0; i < 100; i++)
    number++;

  EXPECT_TRUE(number == 100);
  EXPECT_EQ(number.repr(), VU32(100));
  EXPECT_EQ(number.sign, POSITIVE);
  EXPECT_EQ(number.size, 3);

  number = "-129839491283495791283123";

  number++;

  EXPECT_TRUE(number == "-129839491283495791283122");
  EXPECT_EQ(number.repr(), VU32(791283122, 491283495, 129839));
  EXPECT_EQ(number.sign, NEGATIVE);
  EXPECT_EQ(number.size, 24);

  for (int i = 0; i < 1000; i++)
    number++;

  EXPECT_TRUE(number == "-129839491283495791282122");
  EXPECT_EQ(number.repr(), VU32(791282122, 491283495, 129839));
  EXPECT_EQ(number.sign, NEGATIVE);
  EXPECT_EQ(number.size, 24);
}
