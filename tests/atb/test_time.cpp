#include <chrono>
using namespace std::chrono_literals;

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

std::size_t g_MyPredicate_count_before_success = 0;
void *g_MyPredicate_last_args = nullptr;

auto MyPredicate(void *args) -> bool {
  g_MyPredicate_last_args = args;
  if (g_MyPredicate_count_before_success == 0) {
    return true;
  } else {
    --g_MyPredicate_count_before_success;
    return false;
  }
}

constexpr auto MakePredicate(std::size_t succeed_at = 0,
                             void *expected_args = nullptr)
    -> struct atb_Time_RetryPredicate {
  g_MyPredicate_count_before_success = succeed_at;
  g_MyPredicate_last_args = expected_args;

  return {MyPredicate, expected_args};
}

TEST(TestAtbTime, RetryCall) {
  const auto small_delay = atb_timespec_From(2, atb_ms());

  // Count == 0
  EXPECT_TRUE(
      atb_Time_RetryCall(MakePredicate(0), 0, atb_timespec_From(0, atb_ms())));
  EXPECT_EQ(g_MyPredicate_count_before_success, 0);
  EXPECT_EQ(g_MyPredicate_last_args, nullptr);

  EXPECT_FALSE(
      atb_Time_RetryCall(MakePredicate(2), 0, atb_timespec_From(0, atb_ms())));
  EXPECT_EQ(g_MyPredicate_count_before_success, 1);
  EXPECT_EQ(g_MyPredicate_last_args, nullptr);

  // Count > 0
  EXPECT_TRUE(atb_Time_RetryCall(MakePredicate(0), 20, small_delay));
  EXPECT_EQ(g_MyPredicate_count_before_success, 0);
  EXPECT_EQ(g_MyPredicate_last_args, nullptr);

  EXPECT_TRUE(atb_Time_RetryCall(MakePredicate(10), 20, small_delay));
  EXPECT_EQ(g_MyPredicate_count_before_success, 0);
  EXPECT_EQ(g_MyPredicate_last_args, nullptr);

  EXPECT_TRUE(atb_Time_RetryCall(MakePredicate(20), 20, small_delay));
  EXPECT_EQ(g_MyPredicate_count_before_success, 0);
  EXPECT_EQ(g_MyPredicate_last_args, nullptr);

  EXPECT_FALSE(atb_Time_RetryCall(MakePredicate(25), 20, small_delay));
  EXPECT_EQ(g_MyPredicate_count_before_success, 25 - 20 - 1);
  EXPECT_EQ(g_MyPredicate_last_args, nullptr);

  // Negative delay clamp count to 0
  EXPECT_FALSE(atb_Time_RetryCall(MakePredicate(15), 20, timespec{-1, 0}));
  EXPECT_EQ(g_MyPredicate_count_before_success, 14);
  EXPECT_EQ(g_MyPredicate_last_args, nullptr);

  EXPECT_FALSE(atb_Time_RetryCall(MakePredicate(15), 20, timespec{0, -1}));
  EXPECT_EQ(g_MyPredicate_count_before_success, 14);
  EXPECT_EQ(g_MyPredicate_last_args, nullptr);

  EXPECT_FALSE(atb_Time_RetryCall(MakePredicate(15), 20, timespec{-1, -1}));
  EXPECT_EQ(g_MyPredicate_count_before_success, 14);
  EXPECT_EQ(g_MyPredicate_last_args, nullptr);

  // Args is correctly forwarded
  int i = 0;
  EXPECT_TRUE(atb_Time_RetryCall(MakePredicate(5, &i), 10, small_delay));
  EXPECT_EQ(g_MyPredicate_count_before_success, 0);
  EXPECT_EQ(g_MyPredicate_last_args, &i);

  // Measure delay is respected
  using clock = std::chrono::high_resolution_clock;

  constexpr auto count = 100;
  const auto delay = atb_timespec_From(50, atb_ms());

  const auto begin = clock::now();
  atb_Time_RetryCall(MakePredicate(count + 1), count, delay);
  const auto elapsed = clock::now() - begin;

  const clock::duration expected =
      (count * (std::chrono::seconds{delay.tv_sec} +
                std::chrono::nanoseconds{delay.tv_nsec}));

  // 10%
  EXPECT_LE(abs(elapsed - expected), (expected * 0.1))
      << SCOPE_LOOP_MSG_2(elapsed, expected);
}

TEST(DeathTestAtbTime, RetryCall) {
  EXPECT_DEBUG_DEATH(atb_Time_RetryCall({nullptr, nullptr}, 0, {});
                     , "function != NULL");
}

} // namespace
