#include "atb/macro.h"
#include "gtest/gtest.h"

namespace {

TEST(TestAtbMacro, ARGN) {
  EXPECT_EQ(0, atb_VA_ARGN());
  EXPECT_EQ(1, atb_VA_ARGN(1));
  EXPECT_EQ(2, atb_VA_ARGN(1, 1));
  EXPECT_EQ(2, atb_VA_ARGN(1, 'v'));
  EXPECT_EQ(5, atb_VA_ARGN(1, 'v', 1, 'v', 1));
}

#define TEST_COUCOU_0 0
#define TEST_COUCOU_1 1
#define TEST_COUCOU_2 2

TEST(TestAtbMacro, TKN_CONCAT) {
  EXPECT_EQ(1234, atb_TKN_CONCAT(1234));
  EXPECT_EQ(1234, atb_TKN_CONCAT(123, 4));
  EXPECT_EQ(1234, atb_TKN_CONCAT(12, 34));
  EXPECT_EQ(1234, atb_TKN_CONCAT(1, 234));
  EXPECT_EQ(1234, atb_TKN_CONCAT(1, 2, 34));
  EXPECT_EQ(1234, atb_TKN_CONCAT(1, 2, 3, 4));

  EXPECT_EQ(TEST_COUCOU_0, atb_TKN_CONCAT(TEST_COUCOU, _0));
  EXPECT_EQ(TEST_COUCOU_1, atb_TKN_CONCAT(TEST, _COUCOU, _1));
  EXPECT_EQ(TEST_COUCOU_2, atb_TKN_CONCAT(TEST, _COU, COU_, 2));
  EXPECT_EQ(TEST_COUCOU_2, atb_TKN_CONCAT(T, E, S, T, _, C, O, U, C, OU_2));
  EXPECT_EQ(TEST_COUCOU_2, atb_TKN_CONCAT(T, E, S, T, _, C, O, U, C, OU_2));

  // Tokens are evaluated
  EXPECT_EQ(TEST_COUCOU_0, atb_TKN_CONCAT(TEST_COUCOU_, TEST_COUCOU_0));
}

TEST(TestAtbMacro, TKN_STR) {
  EXPECT_STREQ("0", atb_TKN_STR(0));
  EXPECT_STREQ("0", atb_TKN_STR(TEST_COUCOU_0));
}
} // namespace
