#include <limits>

#include "atb/ratio.h"
#include "gtest/gtest.h"
#include "helper/Ratio.hpp"

namespace {

constexpr auto k_max = std::numeric_limits<atb_Ratio_elem_t>::max();
constexpr auto k_min = std::numeric_limits<atb_Ratio_elem_t>::min();

TEST(AtbRatioTest, Tof) {
  EXPECT_NEAR((float)0.333'333, atb_Ratio_Tof32({1, 3}), 1e-6);
  EXPECT_NEAR((double)0.666'666'666, atb_Ratio_Tof64({2, 3}), 1e-9);
}

TEST(AtbRatioDeathTest, Tof) {
  EXPECT_DEBUG_DEATH(atb_Ratio_Tof32(atb_Ratio{1, 0}), "ratio.den != 0");
  EXPECT_DEBUG_DEATH(atb_Ratio_Tof64(atb_Ratio{1, 0}), "ratio.den != 0");
}

TEST(AtbRatioTest, APPLY) {
  EXPECT_EQ((atb_Ratio_APPLY((atb_Ratio{2, 1}), 10)), 20);
  EXPECT_EQ((atb_Ratio_APPLY((atb_Ratio{1, 2}), 10)), 5);
  EXPECT_NEAR((atb_Ratio_APPLY((atb_Ratio{1, 10}), 0.5)), 0.05, 1e-9);
}

TEST(AtbRatioTest, Inv) {
  atb_Ratio inv;
  EXPECT_TRUE(atb_Ratio_Inv(atb_Ratio{2, 1}, &inv));
  EXPECT_EQ(inv, (atb_Ratio{1, 2}));

  EXPECT_TRUE(atb_Ratio_Inv(atb_Ratio{4, 3}, &inv));
  EXPECT_EQ(inv, (atb_Ratio{3, 4}));

  EXPECT_TRUE(atb_Ratio_Inv(atb_Ratio{0, 3}, &inv));
  EXPECT_EQ(inv, (atb_Ratio{3, 0}));

  EXPECT_TRUE(atb_Ratio_Inv(atb_Ratio{3, 0}, &inv));
  EXPECT_EQ(inv, (atb_Ratio{0, 3}));

  EXPECT_TRUE(atb_Ratio_Inv(atb_Ratio{14, -12}, &inv));
  EXPECT_EQ(inv, (atb_Ratio{-12, 14}));

  // Aliasing is OK
  EXPECT_TRUE(atb_Ratio_Inv(inv, &inv));
  EXPECT_EQ(inv, (atb_Ratio{14, -12}));

  // Min/Max have no effects
  EXPECT_TRUE(atb_Ratio_Inv(atb_Ratio{k_max, -12}, &inv));
  EXPECT_EQ(inv, (atb_Ratio{-12, k_max}));

  EXPECT_TRUE(atb_Ratio_Inv(atb_Ratio{-12, k_min}, &inv));
  EXPECT_EQ(inv, (atb_Ratio{k_min, -12}));
}

TEST(AtbRatioDeathTest, Inv) {
  EXPECT_DEBUG_DEATH(atb_Ratio_Inv(atb_Ratio{1, 1}, nullptr), "dest != NULL");
}

TEST(AtbRatioTest, Reduce) {
  atb_Ratio res;
  EXPECT_TRUE(atb_Ratio_Reduce(atb_Ratio{1, 2}, &res));
  EXPECT_EQ((atb_Ratio{1, 2}), res);

  EXPECT_TRUE(atb_Ratio_Reduce(atb_Ratio{9, 12}, &res));
  EXPECT_EQ((atb_Ratio{3, 4}), res);

  EXPECT_TRUE(atb_Ratio_Reduce(atb_Ratio{2, 4}, &res));
  EXPECT_EQ((atb_Ratio{1, 2}), res);

  EXPECT_TRUE(atb_Ratio_Reduce(atb_Ratio{3000, 6000}, &res));
  EXPECT_EQ((atb_Ratio{1, 2}), res);

  EXPECT_TRUE(atb_Ratio_Reduce(atb_Ratio{4, 4}, &res));
  EXPECT_NE((atb_Ratio{1, 2}), res);

  // Test with 0
  EXPECT_TRUE(atb_Ratio_Reduce(atb_Ratio{0, 4}, &res));
  EXPECT_EQ((atb_Ratio{0, 4}), res);

  EXPECT_TRUE(atb_Ratio_Reduce(atb_Ratio{4, 0}, &res));
  EXPECT_EQ((atb_Ratio{4, 0}), res);

  EXPECT_TRUE(atb_Ratio_Reduce(atb_Ratio{0, 0}, &res));
  EXPECT_EQ((atb_Ratio{0, 0}), res);
}

TEST(AtbRatioDeathTest, Reduce) {
  EXPECT_DEBUG_DEATH(atb_Ratio_Reduce(atb_Ratio{1, 1}, nullptr),
                     "dest != NULL");
}

TEST(AtbRatioTest, Add) {
  atb_Ratio res;

  // Nominal tests
  res = {0, 0};
  EXPECT_TRUE(atb_Ratio_Add(atb_Ratio{1, 2}, atb_Ratio{1, 2}, &res));
  EXPECT_EQ(res, (atb_Ratio{2, 2}));

  res = {0, 0};
  EXPECT_TRUE(atb_Ratio_Add(atb_Ratio{2, 4}, atb_Ratio{1, 2}, &res));
  EXPECT_EQ(res, (atb_Ratio{8, 8}));

  res = {0, 0};
  EXPECT_TRUE(atb_Ratio_Add(atb_Ratio{3, 1}, atb_Ratio{1, 2}, &res));
  EXPECT_EQ(res, (atb_Ratio{7, 2}));

  res = {0, 0};
  EXPECT_TRUE(atb_Ratio_Add(atb_Ratio{1, 2}, atb_Ratio{-1, 2}, &res));
  EXPECT_EQ(res, (atb_Ratio{0, 2}));

  // Failure
  res = {0, 0};
  EXPECT_FALSE(atb_Ratio_Add(atb_Ratio{1, 2}, atb_Ratio{k_max, 2}, &res));
  EXPECT_EQ(res, (atb_Ratio{0, 0}));

  res = {0, 0};
  EXPECT_FALSE(atb_Ratio_Add(atb_Ratio{1, 2}, atb_Ratio{2, k_max}, &res));
  EXPECT_EQ(res, (atb_Ratio{0, 0}));

  res = {0, 0};
  EXPECT_FALSE(atb_Ratio_Add(atb_Ratio{k_max, 2}, atb_Ratio{1, 2}, &res));
  EXPECT_EQ(res, (atb_Ratio{0, 0}));

  res = {0, 0};
  EXPECT_FALSE(atb_Ratio_Add(atb_Ratio{2, k_max}, atb_Ratio{1, 2}, &res));
  EXPECT_EQ(res, (atb_Ratio{0, 0}));

  // dest = nullptr
  EXPECT_TRUE(atb_Ratio_Add(atb_Ratio{1, 2}, atb_Ratio{-1, 2}, nullptr));
  EXPECT_FALSE(atb_Ratio_Add(atb_Ratio{2, k_max}, atb_Ratio{1, 2}, nullptr));

  // Aliasing
  res = {0, 1};
  EXPECT_TRUE(atb_Ratio_Add(res, atb_Ratio{1, 2}, &res));
  EXPECT_EQ(res, (atb_Ratio{1, 2}));

  res = {10, 5};
  EXPECT_TRUE(atb_Ratio_Add(atb_Ratio{1, 2}, res, &res));
  EXPECT_EQ(res, (atb_Ratio{25, 10}));

  res = {10, 5};
  EXPECT_FALSE(atb_Ratio_Add(atb_Ratio{k_max, 2}, res, &res));
  EXPECT_EQ(res, (atb_Ratio{10, 5}));

  // .den = 0
  res = {1, 1};
  EXPECT_TRUE(atb_Ratio_Add(atb_Ratio{2, 0}, atb_Ratio{1, 2}, &res));
  EXPECT_EQ(res, (atb_Ratio{4, 0}));

  res = {1, 1};
  EXPECT_TRUE(atb_Ratio_Add(atb_Ratio{2, 5}, atb_Ratio{10, 0}, &res));
  EXPECT_EQ(res, (atb_Ratio{50, 0}));

  res = {1, 1};
  EXPECT_TRUE(atb_Ratio_Add(atb_Ratio{1, 1}, atb_Ratio{0, 10}, &res));
  EXPECT_EQ(res, (atb_Ratio{10, 10}));

  res = {1, 1};
  EXPECT_FALSE(atb_Ratio_Add(atb_Ratio{1, k_max}, atb_Ratio{0, 10}, &res));
  EXPECT_EQ(res, (atb_Ratio{1, 1}));
}

TEST(AtbRatioTest, Sub) {
  atb_Ratio res;

  // Nominal tests
  res = {0, 0};
  EXPECT_TRUE(atb_Ratio_Sub(atb_Ratio{1, 2}, atb_Ratio{1, 2}, &res));
  EXPECT_EQ(res, (atb_Ratio{0, 2}));

  res = {0, 0};
  EXPECT_TRUE(atb_Ratio_Sub(atb_Ratio{2, 4}, atb_Ratio{1, 2}, &res));
  EXPECT_EQ(res, (atb_Ratio{0, 8}));

  res = {0, 0};
  EXPECT_TRUE(atb_Ratio_Sub(atb_Ratio{3, 1}, atb_Ratio{1, 2}, &res));
  EXPECT_EQ(res, (atb_Ratio{5, 2}));

  res = {0, 0};
  EXPECT_TRUE(atb_Ratio_Sub(atb_Ratio{1, 2}, atb_Ratio{-1, 2}, &res));
  EXPECT_EQ(res, (atb_Ratio{2, 2}));

  // // Failure
  res = {0, 0};
  EXPECT_FALSE(atb_Ratio_Sub(atb_Ratio{k_min, 2}, atb_Ratio{k_max, 2}, &res));
  EXPECT_EQ(res, (atb_Ratio{0, 0}));

  res = {0, 0};
  EXPECT_FALSE(atb_Ratio_Sub(atb_Ratio{1, 2}, atb_Ratio{2, k_max}, &res));
  EXPECT_EQ(res, (atb_Ratio{0, 0}));

  res = {0, 0};
  EXPECT_FALSE(atb_Ratio_Sub(atb_Ratio{k_max, 2}, atb_Ratio{k_min, 2}, &res));
  EXPECT_EQ(res, (atb_Ratio{0, 0}));

  res = {0, 0};
  EXPECT_FALSE(atb_Ratio_Sub(atb_Ratio{2, k_max}, atb_Ratio{1, 2}, &res));
  EXPECT_EQ(res, (atb_Ratio{0, 0}));

  // dest = nullptr
  EXPECT_TRUE(atb_Ratio_Sub(atb_Ratio{1, 2}, atb_Ratio{-1, 2}, nullptr));
  EXPECT_FALSE(atb_Ratio_Sub(atb_Ratio{2, k_max}, atb_Ratio{1, 2}, nullptr));

  // Aliasing
  res = {0, 1};
  EXPECT_TRUE(atb_Ratio_Sub(atb_Ratio{1, 2}, res, &res));
  EXPECT_EQ(res, (atb_Ratio{1, 2}));

  res = {10, 5};
  EXPECT_TRUE(atb_Ratio_Sub(res, atb_Ratio{1, 2}, &res));
  EXPECT_EQ(res, (atb_Ratio{15, 10}));

  res = {10, 5};
  EXPECT_FALSE(atb_Ratio_Sub(atb_Ratio{k_max, k_max}, res, &res));
  EXPECT_EQ(res, (atb_Ratio{10, 5}));

  // .den = 0
  res = {1, 1};
  EXPECT_TRUE(atb_Ratio_Sub(atb_Ratio{2, 0}, atb_Ratio{1, 2}, &res));
  EXPECT_EQ(res, (atb_Ratio{4, 0}));

  res = {1, 1};
  EXPECT_TRUE(atb_Ratio_Sub(atb_Ratio{2, 5}, atb_Ratio{10, 0}, &res));
  EXPECT_EQ(res, (atb_Ratio{-50, 0}));

  res = {1, 1};
  EXPECT_TRUE(atb_Ratio_Sub(atb_Ratio{1, 1}, atb_Ratio{0, 10}, &res));
  EXPECT_EQ(res, (atb_Ratio{10, 10}));

  res = {1, 1};
  EXPECT_FALSE(atb_Ratio_Sub(atb_Ratio{1, k_max}, atb_Ratio{0, 10}, &res));
  EXPECT_EQ(res, (atb_Ratio{1, 1}));
}

TEST(AtbRatioTest, Mul) {
  atb_Ratio res;

  // Nominal tests
  res = {0, 0};
  EXPECT_TRUE(atb_Ratio_Mul({1, 2}, {1, 2}, &res));
  EXPECT_EQ(res, (atb_Ratio{1, 4}));

  res = {0, 0};
  EXPECT_TRUE(atb_Ratio_Mul({2, 4}, {1, 2}, &res));
  EXPECT_EQ(res, (atb_Ratio{2, 8}));

  res = {0, 0};
  EXPECT_TRUE(atb_Ratio_Mul({3, 1}, {1, 2}, &res));
  EXPECT_EQ(res, (atb_Ratio{3, 2}));

  res = {0, 0};
  EXPECT_TRUE(atb_Ratio_Mul({-1, 2}, {1, -2}, &res));
  EXPECT_EQ(res, (atb_Ratio{-1, -4}));

  // Failure
  res = {0, 0};
  EXPECT_FALSE(atb_Ratio_Mul(atb_Ratio{k_max, 2}, atb_Ratio{k_max, 2}, &res));
  EXPECT_EQ(res, (atb_Ratio{0, 0}));

  res = {0, 0};
  EXPECT_FALSE(atb_Ratio_Mul(atb_Ratio{1, 2}, atb_Ratio{2, k_max}, &res));
  EXPECT_EQ(res, (atb_Ratio{0, 0}));

  res = {0, 0};
  EXPECT_FALSE(atb_Ratio_Mul(atb_Ratio{0, k_min}, atb_Ratio{k_min, 2}, &res));
  EXPECT_EQ(res, (atb_Ratio{0, 0}));

  res = {0, 0};
  EXPECT_FALSE(atb_Ratio_Mul(atb_Ratio{2, k_max}, atb_Ratio{1, 2}, &res));
  EXPECT_EQ(res, (atb_Ratio{0, 0}));

  // dest = nullptr
  EXPECT_TRUE(atb_Ratio_Mul(atb_Ratio{-1, 2}, atb_Ratio{1, 2}, nullptr));
  EXPECT_FALSE(atb_Ratio_Mul(atb_Ratio{2, k_max}, atb_Ratio{1, 2}, nullptr));

  // Aliasing
  res = {0, 1};
  EXPECT_TRUE(atb_Ratio_Mul(atb_Ratio{1, 2}, res, &res));
  EXPECT_EQ(res, (atb_Ratio{0, 2}));

  res = {10, 5};
  EXPECT_TRUE(atb_Ratio_Mul(res, atb_Ratio{1, 2}, &res));
  EXPECT_EQ(res, (atb_Ratio{10, 10}));

  res = {10, 5};
  EXPECT_FALSE(atb_Ratio_Mul(atb_Ratio{k_max, k_max}, res, &res));
  EXPECT_EQ(res, (atb_Ratio{10, 5}));

  // .den = 0
  res = {1, 1};
  EXPECT_TRUE(atb_Ratio_Mul(atb_Ratio{2, 0}, atb_Ratio{1, 2}, &res));
  EXPECT_EQ(res, (atb_Ratio{2, 0}));

  res = {1, 1};
  EXPECT_TRUE(atb_Ratio_Mul(atb_Ratio{2, 5}, atb_Ratio{10, 0}, &res));
  EXPECT_EQ(res, (atb_Ratio{20, 0}));

  res = {1, 1};
  EXPECT_TRUE(atb_Ratio_Mul(atb_Ratio{1, 1}, atb_Ratio{0, 10}, &res));
  EXPECT_EQ(res, (atb_Ratio{0, 10}));

  res = {1, 1};
  EXPECT_FALSE(atb_Ratio_Mul(atb_Ratio{1, k_max}, atb_Ratio{0, 10}, &res));
  EXPECT_EQ(res, (atb_Ratio{1, 1}));
}

TEST(AtbRatioTest, Div) {
  atb_Ratio res;

  // Nominal tests
  res = {0, 0};
  EXPECT_TRUE(atb_Ratio_Div({1, 2}, {1, 2}, &res));
  EXPECT_EQ(res, (atb_Ratio{2, 2}));

  res = {0, 0};
  EXPECT_TRUE(atb_Ratio_Div({2, 4}, {1, 2}, &res));
  EXPECT_EQ(res, (atb_Ratio{4, 4}));

  res = {0, 0};
  EXPECT_TRUE(atb_Ratio_Div({3, 1}, {1, 2}, &res));
  EXPECT_EQ(res, (atb_Ratio{6, 1}));

  res = {0, 0};
  EXPECT_TRUE(atb_Ratio_Div({-1, 2}, {1, -2}, &res));
  EXPECT_EQ(res, (atb_Ratio{2, 2}));

  // Failure
  res = {0, 0};
  EXPECT_FALSE(atb_Ratio_Div(atb_Ratio{k_max, 2}, atb_Ratio{k_max, 2}, &res));
  EXPECT_EQ(res, (atb_Ratio{0, 0}));

  res = {0, 0};
  EXPECT_FALSE(atb_Ratio_Div(atb_Ratio{300, 2}, atb_Ratio{2, k_max}, &res));
  EXPECT_EQ(res, (atb_Ratio{0, 0}));

  res = {0, 0};
  EXPECT_FALSE(atb_Ratio_Div(atb_Ratio{0, k_min}, atb_Ratio{k_min, 2}, &res));
  EXPECT_EQ(res, (atb_Ratio{0, 0}));

  res = {0, 0};
  EXPECT_FALSE(atb_Ratio_Div(atb_Ratio{2, k_max}, atb_Ratio{40, 2}, &res));
  EXPECT_EQ(res, (atb_Ratio{0, 0}));

  // dest = nullptr
  EXPECT_TRUE(atb_Ratio_Div(atb_Ratio{-1, 2}, atb_Ratio{1, 2}, nullptr));
  EXPECT_FALSE(atb_Ratio_Div(atb_Ratio{2, k_max}, atb_Ratio{2, 4}, nullptr));

  // Aliasing
  res = {0, 1};
  EXPECT_TRUE(atb_Ratio_Div(atb_Ratio{1, 2}, res, &res));
  EXPECT_EQ(res, (atb_Ratio{1, 0}));

  res = {10, 5};
  EXPECT_TRUE(atb_Ratio_Div(res, atb_Ratio{1, 2}, &res));
  EXPECT_EQ(res, (atb_Ratio{20, 5}));

  res = {10, 5};
  EXPECT_FALSE(atb_Ratio_Div(atb_Ratio{k_max, k_max}, res, &res));
  EXPECT_EQ(res, (atb_Ratio{10, 5}));

  // .den = 0
  res = {1, 1};
  EXPECT_TRUE(atb_Ratio_Div(atb_Ratio{2, 0}, atb_Ratio{1, 2}, &res));
  EXPECT_EQ(res, (atb_Ratio{4, 0}));

  res = {1, 1};
  EXPECT_TRUE(atb_Ratio_Div(atb_Ratio{2, 5}, atb_Ratio{10, 0}, &res));
  EXPECT_EQ(res, (atb_Ratio{0, 50}));

  res = {1, 1};
  EXPECT_TRUE(atb_Ratio_Div(atb_Ratio{1, 1}, atb_Ratio{0, 10}, &res));
  EXPECT_EQ(res, (atb_Ratio{10, 0}));

  res = {1, 1};
  EXPECT_FALSE(atb_Ratio_Div(atb_Ratio{1, k_max}, atb_Ratio{10, 0}, &res));
  EXPECT_EQ(res, (atb_Ratio{1, 1}));
}

TEST(AtbRatioTest, Comparisons) {
  int cmp;

  // TESTS EQ
  for (auto [lhs, rhs] : std::array{
           std::array{atb_Ratio{1, 2}, atb_Ratio{1, 2}},
           std::array{atb_Ratio{1, 2}, atb_Ratio{2, 4}},
           std::array{atb_Ratio{2, 4}, atb_Ratio{10, 20}},
           std::array{atb_Ratio{1, 2}, atb_Ratio{-1, -2}},
           std::array{atb_Ratio{3, 2}, atb_Ratio{3, 2}},
           std::array{atb_Ratio{-3, 2}, atb_Ratio{-3, 2}},
           std::array{atb_Ratio{-3, 2}, atb_Ratio{3, -2}},
           std::array{atb_Ratio{-3, -2}, atb_Ratio{-3, -2}},
           std::array{atb_Ratio{k_max, k_max}, atb_Ratio{k_max, k_max}},
           std::array{atb_Ratio{k_max, k_max}, atb_Ratio{1, 1}},
           std::array{atb_Ratio{k_max, k_max}, atb_Ratio{4, 4}},
       }) {
    SCOPED_TRACE(SCOPE_LOOP_MSG_2(lhs, rhs));

    cmp = atb_Ratio_Compare_GREATER;
    EXPECT_TRUE(atb_Ratio_Compare(lhs, rhs, &cmp));
    EXPECT_EQ(atb_Ratio_Compare_EQUAL, cmp);
    EXPECT_TRUE(atb_Ratio_Eq(lhs, rhs));
    EXPECT_FALSE(atb_Ratio_Ne(lhs, rhs));

    EXPECT_TRUE(atb_Ratio_Ge(lhs, rhs));
    EXPECT_TRUE(atb_Ratio_Le(lhs, rhs));
  }

  // TESTS NE
  for (auto [lhs, rhs] : std::array{
           std::array{atb_Ratio{1, 2}, atb_Ratio{1, 3}},
           std::array{atb_Ratio{394, 2}, atb_Ratio{2, 4}},
           std::array{atb_Ratio{2, 4}, atb_Ratio{-10, 23939}},
           std::array{atb_Ratio{1, -2}, atb_Ratio{-1, -2}},
       }) {
    SCOPED_TRACE(SCOPE_LOOP_MSG_2(lhs, rhs));

    cmp = atb_Ratio_Compare_EQUAL;
    EXPECT_TRUE(atb_Ratio_Compare(lhs, rhs, &cmp));
    EXPECT_NE(atb_Ratio_Compare_EQUAL, cmp);

    EXPECT_FALSE(atb_Ratio_Eq(lhs, rhs));
    EXPECT_TRUE(atb_Ratio_Ne(lhs, rhs));
  }

  // TESTS GREATER
  for (auto [lhs, rhs] : std::array{
           std::array{atb_Ratio{3, 2}, atb_Ratio{1, 2}},
           std::array{atb_Ratio{3, 4}, atb_Ratio{1, 2}},
           std::array{atb_Ratio{1, 2}, atb_Ratio{-1, 2}},
           std::array{atb_Ratio{1, 2}, atb_Ratio{1, -2}},
           std::array{atb_Ratio{-1, -2}, atb_Ratio{1, -2}},
           std::array{atb_Ratio{1, -4}, atb_Ratio{-1, 2}},
           std::array{atb_Ratio{-1, 4}, atb_Ratio{1, -2}},
           std::array{atb_Ratio{-1, 4}, atb_Ratio{-20, 40}},
           std::array{atb_Ratio{-1, 4}, atb_Ratio{-20, 40}},
       }) {
    SCOPED_TRACE(SCOPE_LOOP_MSG_2(lhs, rhs));

    EXPECT_FALSE(atb_Ratio_Eq(lhs, rhs));
    EXPECT_TRUE(atb_Ratio_Ne(lhs, rhs));

    cmp = atb_Ratio_Compare_EQUAL;
    EXPECT_TRUE(atb_Ratio_Compare(lhs, rhs, &cmp));
    EXPECT_EQ(atb_Ratio_Compare_GREATER, cmp);
    EXPECT_TRUE(atb_Ratio_Ge(lhs, rhs));
    EXPECT_TRUE(atb_Ratio_Gt(lhs, rhs));

    EXPECT_FALSE(atb_Ratio_Le(lhs, rhs));
    EXPECT_FALSE(atb_Ratio_Lt(lhs, rhs));
  }

  // TESTS LESS
  for (auto [lhs, rhs] : std::array{
           std::array{atb_Ratio{1, 2}, atb_Ratio{3, 2}},
           std::array{atb_Ratio{1, 2}, atb_Ratio{3, 4}},
           std::array{atb_Ratio{-1, 2}, atb_Ratio{1, 2}},
           std::array{atb_Ratio{1, -2}, atb_Ratio{1, 2}},
           std::array{atb_Ratio{1, -2}, atb_Ratio{-1, -2}},
           std::array{atb_Ratio{-1, 2}, atb_Ratio{1, -4}},
           std::array{atb_Ratio{1, -2}, atb_Ratio{-1, 4}},
           std::array{atb_Ratio{-20, 40}, atb_Ratio{-1, 4}},
       }) {
    SCOPED_TRACE(SCOPE_LOOP_MSG_2(lhs, rhs));

    EXPECT_FALSE(atb_Ratio_Eq(lhs, rhs));
    EXPECT_TRUE(atb_Ratio_Ne(lhs, rhs));

    cmp = atb_Ratio_Compare_EQUAL;
    EXPECT_TRUE(atb_Ratio_Compare(lhs, rhs, &cmp));
    EXPECT_EQ(atb_Ratio_Compare_LESS, cmp);
    EXPECT_TRUE(atb_Ratio_Le(lhs, rhs));
    EXPECT_TRUE(atb_Ratio_Lt(lhs, rhs));

    EXPECT_FALSE(atb_Ratio_Ge(lhs, rhs));
    EXPECT_FALSE(atb_Ratio_Gt(lhs, rhs));
  }

  // TESTS FAILURE
  for (auto [lhs, rhs] : std::array{
           std::array{atb_Ratio{k_max, k_max}, atb_Ratio{k_max, k_min}},
           std::array{atb_Ratio{k_max, k_min}, atb_Ratio{k_max, k_min}},
           std::array{atb_Ratio{k_min, 40}, atb_Ratio{-1, k_max}},
           std::array{atb_Ratio{k_min, k_min}, atb_Ratio{k_min, k_min}},
           std::array{atb_Ratio{k_min, k_max}, atb_Ratio{k_min, k_max}},
       }) {
    SCOPED_TRACE(SCOPE_LOOP_MSG_2(lhs, rhs));

    cmp = 20;
    EXPECT_FALSE(atb_Ratio_Compare(lhs, rhs, &cmp));
    EXPECT_EQ(20, cmp);

    EXPECT_FALSE(atb_Ratio_Eq(lhs, rhs));
    EXPECT_FALSE(atb_Ratio_Ne(lhs, rhs));
    EXPECT_FALSE(atb_Ratio_Le(lhs, rhs));
    EXPECT_FALSE(atb_Ratio_Lt(lhs, rhs));
    EXPECT_FALSE(atb_Ratio_Ge(lhs, rhs));
    EXPECT_FALSE(atb_Ratio_Gt(lhs, rhs));
  }
}

} // namespace
