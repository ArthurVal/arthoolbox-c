#include <chrono>
using namespace std::chrono_literals;

#include "atb/time.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "helper/Ratio.hpp"
#include "helper/Time.hpp"

namespace {

constexpr auto ToChronoDuration(timespec ts) {
  return std::chrono::seconds{ts.tv_sec} + std::chrono::nanoseconds{ts.tv_nsec};
}

TEST(TestAtbTime, Now) {
  const auto now = atb_timespec_Now(CLOCK_REALTIME);

  const std::chrono::nanoseconds expected =
      std::chrono::system_clock::now().time_since_epoch();

  EXPECT_LE(abs(ToChronoDuration(now) - expected), 5us)
      << SCOPE_LOOP_MSG_2(now, expected);
}

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
  EXPECT_DEBUG_DEATH({ atb_timespec_From(0, {1, 0}); }, "den != 0");
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

struct RetryCallPredicateMock {
  MOCK_METHOD(bool, Predicate, (void *), ());

  static auto Inst() -> RetryCallPredicateMock & {
    static auto mock = RetryCallPredicateMock();
    return mock;
  }

  static auto Fn(void *args) -> bool { return Inst().Predicate(args); }
};

template <class InputIt, class ClockType = std::chrono::high_resolution_clock>
constexpr auto WriteStampInto(InputIt d_first) {
  using ::testing::InvokeWithoutArgs;
  return InvokeWithoutArgs(
      [d_first]() mutable { *d_first++ = ClockType::now(); });
}

TEST(TestAtbTime, RetryCall) {
  using ::testing::_;
  using ::testing::DoAll;
  using ::testing::InvokeWithoutArgs;
  using ::testing::Return;

  const auto mock_fn = &RetryCallPredicateMock::Fn;
  auto &mock = RetryCallPredicateMock::Inst();
  ON_CALL(mock, Predicate(_)).WillByDefault(Return(true));

  // Test count = 0
  EXPECT_CALL(mock, Predicate(nullptr))
      .WillOnce(Return(false))
      .WillOnce(Return(true))
      .RetiresOnSaturation();

  EXPECT_FALSE(atb_Time_RetryCall({mock_fn, nullptr}, 0,
                                  atb_timespec_From(0, atb_us())));

  EXPECT_TRUE(atb_Time_RetryCall({mock_fn, nullptr}, 0,
                                 atb_timespec_From(0, atb_us())));

  // Test args ptr
  int foo = 2;
  EXPECT_CALL(mock, Predicate(&foo))
      .WillOnce(Return(false))
      .WillOnce(Return(true))
      .RetiresOnSaturation();

  EXPECT_FALSE(
      atb_Time_RetryCall({mock_fn, &foo}, 0, atb_timespec_From(0, atb_us())));

  EXPECT_TRUE(
      atb_Time_RetryCall({mock_fn, &foo}, 0, atb_timespec_From(0, atb_us())));

  // Test delay
  using clk = std::chrono::high_resolution_clock;
  using time_point = clk::time_point;

  constexpr auto count = 100u;
  std::vector<time_point> stamps;
  stamps.reserve(count + 1);

  EXPECT_CALL(mock, Predicate(nullptr))
      .Times(stamps.capacity())
      .WillRepeatedly(
          DoAll(WriteStampInto(std::back_inserter(stamps)), Return(false)))
      .RetiresOnSaturation();

  const auto delay = atb_timespec_From(50, atb_ms());
  auto expected = (count * ToChronoDuration(delay));

  const auto begin = clk::now();
  auto succeed = atb_Time_RetryCall({mock_fn, nullptr}, count, delay);
  auto elapsed = clk::now() - begin;

  EXPECT_FALSE(succeed);

  EXPECT_LE(abs(elapsed - expected), expected * 0.05) // 5% error ?
      << SCOPE_LOOP_MSG_2(elapsed, expected);

  ASSERT_GE(stamps.size(), 1);
  EXPECT_EQ(stamps.size(), count + 1);

  {
    auto stamp_t_0 = std::cbegin(stamps);
    auto stamp_t_1 = std::next(stamp_t_0);

    // Compute the mean
    elapsed = 0ms;
    for (; stamp_t_1 != std::cend(stamps); ++stamp_t_0, ++stamp_t_1) {
      elapsed += (*stamp_t_1 - *stamp_t_0);
    }
    elapsed /= (stamps.size() - 1);

    // Compare to the expected value
    expected = ToChronoDuration(delay);
    EXPECT_LE(abs(elapsed - expected), expected * 0.05) // 5% error ?
        << SCOPE_LOOP_MSG_2(elapsed, expected);
  }
}

TEST(DeathTestAtbTime, RetryCall) {
  EXPECT_DEBUG_DEATH(atb_Time_RetryCall({nullptr, nullptr}, 0, {});
                     , "function != NULL");
}

} // namespace
