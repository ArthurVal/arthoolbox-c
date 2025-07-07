
#include "atb/time.h"
#include "gtest/gtest.h"
#include "helper/Ratio.hpp"
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
                helper::FieldsMatch(timespec{0, 0}))
        << "ratio = " << ratio;
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

TEST(TestAtbTime, Compare) {
  // TESTS EQ
  for (auto [lhs, rhs] : std::array{
           std::array{timespec{1, 2}, timespec{1, 2}},
           std::array{timespec{3, 2}, timespec{3, 2}},
           std::array{timespec{-3, 2}, timespec{-3, 2}},
           std::array{timespec{-3, -2}, timespec{-3, -2}},
           std::array{atb_timespec_From(10, atb_sec()),
                      atb_timespec_From(10000, atb_ms())},
       }) {
    SCOPED_TRACE(SCOPE_LOOP_MSG_2(lhs, rhs));

    EXPECT_EQ(atb_timespec_Compare_EQUAL, atb_timespec_Compare(lhs, rhs));
    EXPECT_TRUE(atb_timespec_Eq(lhs, rhs));
    EXPECT_FALSE(atb_timespec_Ne(lhs, rhs));

    EXPECT_TRUE(atb_timespec_Ge(lhs, rhs));
    EXPECT_TRUE(atb_timespec_Le(lhs, rhs));
  }

  // TESTS NE
  for (auto [lhs, rhs] : std::array{
           std::array{timespec{1, 2}, timespec{1, 3}},
           std::array{timespec{394, 2}, timespec{2, 4}},
           std::array{timespec{2, 4}, timespec{-10, 23939}},
           std::array{timespec{1, -2}, timespec{-1, -2}},
           std::array{atb_timespec_From(10, atb_sec()),
                      atb_timespec_From(10, atb_ms())},
       }) {
    SCOPED_TRACE(SCOPE_LOOP_MSG_2(lhs, rhs));

    EXPECT_NE(atb_timespec_Compare_EQUAL, atb_timespec_Compare(lhs, rhs));
    EXPECT_FALSE(atb_timespec_Eq(lhs, rhs));
    EXPECT_TRUE(atb_timespec_Ne(lhs, rhs));
  }

  // TESTS GREATER
  for (auto [lhs, rhs] : std::array{
           std::array{timespec{3, 2}, timespec{1, 2}},
           std::array{timespec{3, 4}, timespec{1, 2}},
           std::array{timespec{1, 2}, timespec{-1, 2}},
           std::array{timespec{1, 2}, timespec{1, -2}},
           std::array{timespec{1, -4}, timespec{-1, 2}},
           std::array{timespec{-1, 4}, timespec{-20, 40}},
           std::array{atb_timespec_From(10, atb_sec()),
                      atb_timespec_From(10, atb_ms())},
       }) {
    SCOPED_TRACE(SCOPE_LOOP_MSG_2(lhs, rhs));

    EXPECT_FALSE(atb_timespec_Eq(lhs, rhs));
    EXPECT_TRUE(atb_timespec_Ne(lhs, rhs));

    EXPECT_EQ(atb_timespec_Compare_GREATER, atb_timespec_Compare(lhs, rhs));
    EXPECT_TRUE(atb_timespec_Ge(lhs, rhs));
    EXPECT_TRUE(atb_timespec_Gt(lhs, rhs));

    EXPECT_FALSE(atb_timespec_Le(lhs, rhs));
    EXPECT_FALSE(atb_timespec_Lt(lhs, rhs));
  }

  // TESTS LESS
  for (auto [lhs, rhs] : std::array{
           std::array{timespec{1, 2}, timespec{3, 2}},
           std::array{timespec{1, 2}, timespec{3, 4}},
           std::array{timespec{-1, 2}, timespec{1, 2}},
           std::array{timespec{1, -2}, timespec{1, 2}},
           std::array{timespec{-1, 2}, timespec{1, -4}},
           std::array{timespec{-20, 40}, timespec{-1, 4}},
           std::array{atb_timespec_From(10, atb_us()),
                      atb_timespec_From(10, atb_ms())},
       }) {
    SCOPED_TRACE(SCOPE_LOOP_MSG_2(lhs, rhs));

    EXPECT_FALSE(atb_timespec_Eq(lhs, rhs));
    EXPECT_TRUE(atb_timespec_Ne(lhs, rhs));

    EXPECT_EQ(atb_timespec_Compare_LESS, atb_timespec_Compare(lhs, rhs));
    EXPECT_TRUE(atb_timespec_Le(lhs, rhs));
    EXPECT_TRUE(atb_timespec_Lt(lhs, rhs));

    EXPECT_FALSE(atb_timespec_Ge(lhs, rhs));
    EXPECT_FALSE(atb_timespec_Gt(lhs, rhs));
  }
}

} // namespace
