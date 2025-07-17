#include "atb/ratio.h"
#include "gtest/gtest.h"
#include "helper/Ratio.hpp"

namespace {

TEST(TestAtbRatio, Tof) {
  EXPECT_NEAR((float)0.333'333, atb_Ratio_Tof32({1, 3}), 1e-6);
  EXPECT_NEAR((double)0.666'666'666, atb_Ratio_Tof64({2, 3}), 1e-9);
}

TEST(TestAtbRatio, APPLY) {
  EXPECT_EQ((atb_Ratio_APPLY((atb_Ratio{2, 1}), 10)), 20);
  EXPECT_EQ((atb_Ratio_APPLY((atb_Ratio{1, 2}), 10)), 5);
  EXPECT_NEAR((atb_Ratio_APPLY((atb_Ratio{1, 10}), 0.5)), 0.05, 1e-9);
}

TEST(TestAtbRatio, Inv) {
  EXPECT_EQ((atb_Ratio{1, 2}), atb_Ratio_Inv(atb_Ratio{2, 1}));
  EXPECT_EQ((atb_Ratio{3, 4}), atb_Ratio_Inv(atb_Ratio{4, 3}));

  EXPECT_EQ((atb_Ratio{0, 0}), atb_Ratio_Inv(atb_Ratio{0, 0}));
  EXPECT_EQ((atb_Ratio{0, 1}), atb_Ratio_Inv(atb_Ratio{1, 0}));
}

TEST(TestAtbRatio, Add) {
  EXPECT_EQ((atb_Ratio{2, 2}), atb_Ratio_Add(atb_Ratio{1, 2}, atb_Ratio{1, 2}));
  EXPECT_EQ((atb_Ratio{8, 8}), atb_Ratio_Add(atb_Ratio{2, 4}, atb_Ratio{1, 2}));
  EXPECT_EQ((atb_Ratio{7, 2}), atb_Ratio_Add(atb_Ratio{3, 1}, atb_Ratio{1, 2}));

  EXPECT_EQ((atb_Ratio{0, 2}),
            atb_Ratio_Add(atb_Ratio{1, 2}, atb_Ratio{-1, 2}));
}

TEST(TestAtbRatio, Sub) {
  EXPECT_EQ((atb_Ratio{0, 2}), atb_Ratio_Sub(atb_Ratio{1, 2}, atb_Ratio{1, 2}));
  EXPECT_EQ((atb_Ratio{0, 8}), atb_Ratio_Sub(atb_Ratio{2, 4}, atb_Ratio{1, 2}));
  EXPECT_EQ((atb_Ratio{5, 2}), atb_Ratio_Sub(atb_Ratio{3, 1}, atb_Ratio{1, 2}));
  EXPECT_EQ((atb_Ratio{-18, 8}),
            atb_Ratio_Sub(atb_Ratio{1, 4}, atb_Ratio{5, 2}));

  EXPECT_EQ((atb_Ratio{2, 2}),
            atb_Ratio_Sub(atb_Ratio{1, 2}, atb_Ratio{-1, 2}));
}

TEST(TestAtbRatio, Mul) {
  EXPECT_EQ((atb_Ratio{1, 4}), atb_Ratio_Mul(atb_Ratio{1, 2}, atb_Ratio{1, 2}));
  EXPECT_EQ((atb_Ratio{2, 8}), atb_Ratio_Mul(atb_Ratio{2, 4}, atb_Ratio{1, 2}));
  EXPECT_EQ((atb_Ratio{3, 2}), atb_Ratio_Mul(atb_Ratio{3, 1}, atb_Ratio{1, 2}));
  EXPECT_EQ((atb_Ratio{5, 8}), atb_Ratio_Mul(atb_Ratio{1, 4}, atb_Ratio{5, 2}));

  EXPECT_EQ((atb_Ratio{-1, 4}),
            atb_Ratio_Mul(atb_Ratio{1, 2}, atb_Ratio{-1, 2}));
}

TEST(TestAtbRatio, Div) {
  EXPECT_EQ((atb_Ratio{2, 2}), atb_Ratio_Div(atb_Ratio{1, 2}, atb_Ratio{1, 2}));
  EXPECT_EQ((atb_Ratio{4, 4}), atb_Ratio_Div(atb_Ratio{2, 4}, atb_Ratio{1, 2}));
  EXPECT_EQ((atb_Ratio{6, 1}), atb_Ratio_Div(atb_Ratio{3, 1}, atb_Ratio{1, 2}));
  EXPECT_EQ((atb_Ratio{2, 20}),
            atb_Ratio_Div(atb_Ratio{1, 4}, atb_Ratio{5, 2}));

  EXPECT_EQ((atb_Ratio{2, -2}),
            atb_Ratio_Div(atb_Ratio{1, 2}, atb_Ratio{-1, 2}));
}

TEST(TestAtbRatio, Reduce) {
  EXPECT_EQ((atb_Ratio{1, 2}), atb_Ratio_Reduce(atb_Ratio{1, 2}));
  EXPECT_EQ((atb_Ratio{3, 4}), atb_Ratio_Reduce(atb_Ratio{9, 12}));
  EXPECT_EQ((atb_Ratio{1, 2}), atb_Ratio_Reduce(atb_Ratio{2, 4}));
  EXPECT_EQ((atb_Ratio{1, 2}), atb_Ratio_Reduce(atb_Ratio{3000, 6000}));

  EXPECT_NE((atb_Ratio{1, 2}), atb_Ratio_Reduce(atb_Ratio{4, 4}));

  EXPECT_EQ((atb_Ratio{0, 4}), atb_Ratio_Reduce(atb_Ratio{0, 4}));
  EXPECT_EQ((atb_Ratio{4, 0}), atb_Ratio_Reduce(atb_Ratio{4, 0}));
}

TEST(TestAtbRatio, FixSign) {
  EXPECT_EQ((atb_Ratio{1, 2}), atb_Ratio_FixSign({1, 2}));
  EXPECT_EQ((atb_Ratio{-1, 2}), atb_Ratio_FixSign({1, -2}));
  EXPECT_EQ((atb_Ratio{-1, 2}), atb_Ratio_FixSign({-1, 2}));
  EXPECT_EQ((atb_Ratio{1, 2}), atb_Ratio_FixSign({-1, -2}));
}

TEST(TestAtbRatio, Comparisons) {
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
       }) {
    SCOPED_TRACE(SCOPE_LOOP_MSG_2(lhs, rhs));

    EXPECT_EQ(atb_Ratio_Compare_EQUAL, atb_Ratio_Compare(lhs, rhs));
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

    EXPECT_NE(atb_Ratio_Compare_EQUAL, atb_Ratio_Compare(lhs, rhs));
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
       }) {
    SCOPED_TRACE(SCOPE_LOOP_MSG_2(lhs, rhs));

    EXPECT_FALSE(atb_Ratio_Eq(lhs, rhs));
    EXPECT_TRUE(atb_Ratio_Ne(lhs, rhs));

    EXPECT_EQ(atb_Ratio_Compare_GREATER, atb_Ratio_Compare(lhs, rhs));
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

    EXPECT_EQ(atb_Ratio_Compare_LESS, atb_Ratio_Compare(lhs, rhs));
    EXPECT_TRUE(atb_Ratio_Le(lhs, rhs));
    EXPECT_TRUE(atb_Ratio_Lt(lhs, rhs));

    EXPECT_FALSE(atb_Ratio_Ge(lhs, rhs));
    EXPECT_FALSE(atb_Ratio_Gt(lhs, rhs));
  }
}

} // namespace
