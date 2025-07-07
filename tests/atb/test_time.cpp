
#include "atb/time.h"
#include "gtest/gtest.h"
#include "helper/Time.hpp"

namespace {

TEST(TestAtbTime, Now) { GTEST_SKIP(); }

TEST(TestAtbTime, From) {
  for (auto ratio : {
           atb_ns(),
           atb_us(),
           atb_ms(),
           atb_sec(),
           atb_minutes(),
           atb_hours(),
           atb_days(),
           atb_weeks(),
           atb_months(),
           atb_years(),
       }) {
    EXPECT_THAT(atb_timespec_From(0, ratio),
                helper::FieldsMatch(timespec{0, 0})) << "ratio = " << ratio;
  }

  EXPECT_THAT(atb_timespec_From(100, atb_ns()),
              helper::FieldsMatch(timespec{0, 100}));

  EXPECT_THAT(atb_timespec_From(100, atb_us()),
              helper::FieldsMatch(timespec{0, 100'000}));

  EXPECT_THAT(atb_timespec_From(100, atb_ms()),
              helper::FieldsMatch(timespec{0, 100'000'000}));

  EXPECT_THAT(atb_timespec_From(1000, atb_ms()),
              helper::FieldsMatch(timespec{1, 0}));

  EXPECT_THAT(atb_timespec_From(1300, atb_ms()),
              helper::FieldsMatch(timespec{1, 300'000'000}));

  EXPECT_THAT(atb_timespec_From(-10300, atb_ms()),
              helper::FieldsMatch(timespec{-10, -300'000'000}));
}

TEST(DeathTestAtbTime, From) {
  EXPECT_DEBUG_DEATH({ atb_timespec_From(0, {1, 0}); }, "!= 0");
}

// TEST(TestAtbRatio, Comparisons) {
//   // TESTS EQ
//   for (auto [lhs, rhs] : std::array{
//            std::array{atb_Ratio{1, 2}, atb_Ratio{1, 2}},
//            std::array{atb_Ratio{1, 2}, atb_Ratio{2, 4}},
//            std::array{atb_Ratio{2, 4}, atb_Ratio{10, 20}},
//            std::array{atb_Ratio{1, 2}, atb_Ratio{-1, -2}},
//            std::array{atb_Ratio{3, 2}, atb_Ratio{3, 2}},
//            std::array{atb_Ratio{-3, 2}, atb_Ratio{-3, 2}},
//            std::array{atb_Ratio{-3, 2}, atb_Ratio{3, -2}},
//            std::array{atb_Ratio{-3, -2}, atb_Ratio{-3, -2}},
//        }) {
//     SCOPED_TRACE(SCOPE_LOOP_MSG_2(lhs, rhs));

//     EXPECT_EQ(atb_Ratio_Compare_EQUAL, atb_Ratio_Compare(lhs, rhs));
//     EXPECT_TRUE(atb_Ratio_Eq(lhs, rhs));
//     EXPECT_FALSE(atb_Ratio_Ne(lhs, rhs));

//     EXPECT_TRUE(atb_Ratio_Ge(lhs, rhs));
//     EXPECT_TRUE(atb_Ratio_Le(lhs, rhs));
//   }

//   // TESTS NE
//   for (auto [lhs, rhs] : std::array{
//            std::array{atb_Ratio{1, 2}, atb_Ratio{1, 3}},
//            std::array{atb_Ratio{394, 2}, atb_Ratio{2, 4}},
//            std::array{atb_Ratio{2, 4}, atb_Ratio{-10, 23939}},
//            std::array{atb_Ratio{1, -2}, atb_Ratio{-1, -2}},
//        }) {
//     SCOPED_TRACE(SCOPE_LOOP_MSG_2(lhs, rhs));

//     EXPECT_NE(atb_Ratio_Compare_EQUAL, atb_Ratio_Compare(lhs, rhs));
//     EXPECT_FALSE(atb_Ratio_Eq(lhs, rhs));
//     EXPECT_TRUE(atb_Ratio_Ne(lhs, rhs));
//   }

//   // TESTS GREATER
//   for (auto [lhs, rhs] : std::array{
//            std::array{atb_Ratio{3, 2}, atb_Ratio{1, 2}},
//            std::array{atb_Ratio{3, 4}, atb_Ratio{1, 2}},
//            std::array{atb_Ratio{1, 2}, atb_Ratio{-1, 2}},
//            std::array{atb_Ratio{1, 2}, atb_Ratio{1, -2}},
//            std::array{atb_Ratio{-1, -2}, atb_Ratio{1, -2}},
//            std::array{atb_Ratio{1, -4}, atb_Ratio{-1, 2}},
//            std::array{atb_Ratio{-1, 4}, atb_Ratio{1, -2}},
//            std::array{atb_Ratio{-1, 4}, atb_Ratio{-20, 40}},
//        }) {
//     SCOPED_TRACE(SCOPE_LOOP_MSG_2(lhs, rhs));

//     EXPECT_FALSE(atb_Ratio_Eq(lhs, rhs));
//     EXPECT_TRUE(atb_Ratio_Ne(lhs, rhs));

//     EXPECT_EQ(atb_Ratio_Compare_GREATER, atb_Ratio_Compare(lhs, rhs));
//     EXPECT_TRUE(atb_Ratio_Ge(lhs, rhs));
//     EXPECT_TRUE(atb_Ratio_Gt(lhs, rhs));

//     EXPECT_FALSE(atb_Ratio_Le(lhs, rhs));
//     EXPECT_FALSE(atb_Ratio_Lt(lhs, rhs));
//   }

//   // TESTS LESS
//   for (auto [lhs, rhs] : std::array{
//            std::array{atb_Ratio{1, 2}, atb_Ratio{3, 2}},
//            std::array{atb_Ratio{1, 2}, atb_Ratio{3, 4}},
//            std::array{atb_Ratio{-1, 2}, atb_Ratio{1, 2}},
//            std::array{atb_Ratio{1, -2}, atb_Ratio{1, 2}},
//            std::array{atb_Ratio{1, -2}, atb_Ratio{-1, -2}},
//            std::array{atb_Ratio{-1, 2}, atb_Ratio{1, -4}},
//            std::array{atb_Ratio{1, -2}, atb_Ratio{-1, 4}},
//            std::array{atb_Ratio{-20, 40}, atb_Ratio{-1, 4}},
//        }) {
//     SCOPED_TRACE(SCOPE_LOOP_MSG_2(lhs, rhs));

//     EXPECT_FALSE(atb_Ratio_Eq(lhs, rhs));
//     EXPECT_TRUE(atb_Ratio_Ne(lhs, rhs));

//     EXPECT_EQ(atb_Ratio_Compare_LESS, atb_Ratio_Compare(lhs, rhs));
//     EXPECT_TRUE(atb_Ratio_Le(lhs, rhs));
//     EXPECT_TRUE(atb_Ratio_Lt(lhs, rhs));

//     EXPECT_FALSE(atb_Ratio_Ge(lhs, rhs));
//     EXPECT_FALSE(atb_Ratio_Gt(lhs, rhs));
//   }
// }

} // namespace
