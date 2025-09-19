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

constexpr auto ToTimespec(std::chrono::nanoseconds d) -> timespec {
  return {
      .tv_sec = d.count() / 1'000'000'000,
      .tv_nsec = d.count() % 1'000'000'000,
  };
}

TEST(AtbTimeTest, From) {
  timespec ts;

  for (auto ratio : {
           K_ATB_NS,
           K_ATB_US,
           K_ATB_MS,
           K_ATB_SEC,
           K_ATB_MINUTES,
           K_ATB_HOURS,
           K_ATB_DAYS,
           K_ATB_WEEKS,
           K_ATB_MONTHS,
           K_ATB_YEARS,
       }) {
    SCOPED_TRACE(SCOPE_LOOP_MSG_1(ratio));

    ts = {-1, -1};
    EXPECT_TRUE(atb_timespec_Set(&ts, 0, ratio));
    EXPECT_THAT(ts, helper::FieldsMatch(timespec{0, 0}));
  }

  EXPECT_TRUE(atb_timespec_Set(&ts, 100, K_ATB_NS));
  EXPECT_THAT(ts, helper::FieldsMatch(timespec{0, 100}));

  ts = {-1, -1};
  EXPECT_TRUE(atb_timespec_Set(&ts, 100, K_ATB_US));
  EXPECT_THAT(ts, helper::FieldsMatch(timespec{0, 100'000}));

  ts = {-1, -1};
  EXPECT_TRUE(atb_timespec_Set(&ts, 100, K_ATB_MS));
  EXPECT_THAT(ts, helper::FieldsMatch(timespec{0, 100'000'000}));

  ts = {-1, -1};
  EXPECT_TRUE(atb_timespec_Set(&ts, 1000, K_ATB_MS));
  EXPECT_THAT(ts, helper::FieldsMatch(timespec{1, 0}));

  ts = {-1, -1};
  EXPECT_TRUE(atb_timespec_Set(&ts, 1300, K_ATB_MS));
  EXPECT_THAT(ts, helper::FieldsMatch(timespec{1, 300'000'000}));

  ts = {-1, -1};
  EXPECT_TRUE(atb_timespec_Set(&ts, -10300, K_ATB_MS));
  EXPECT_THAT(ts, helper::FieldsMatch(timespec{-10, -300'000'000}));

  // Failure
  ts = {-1, -1};
  EXPECT_FALSE(
      atb_timespec_Set(&ts, std::numeric_limits<int64_t>::max(), K_ATB_YEARS));
  EXPECT_THAT(ts, helper::FieldsMatch(timespec{-1, -1}));

  // Test _FROM
  EXPECT_THAT((atb_timespec_FROM(100, K_ATB_NS)),
              helper::FieldsMatch(timespec{0, 100}));

  EXPECT_THAT((atb_timespec_FROM(100, K_ATB_US)),
              helper::FieldsMatch(timespec{0, 100'000}));

  EXPECT_THAT((atb_timespec_FROM(100, K_ATB_MS)),
              helper::FieldsMatch(timespec{0, 100'000'000}));

  EXPECT_THAT((atb_timespec_FROM(1000, K_ATB_MS)),
              helper::FieldsMatch(timespec{1, 0}));

  EXPECT_THAT((atb_timespec_FROM(1300, K_ATB_MS)),
              helper::FieldsMatch(timespec{1, 300'000'000}));

  EXPECT_THAT((atb_timespec_FROM(-10300, K_ATB_MS)),
              helper::FieldsMatch(timespec{-10, -300'000'000}));
}

TEST(AtbTimeDeathTest, From) {
  timespec ts;
  EXPECT_DEBUG_DEATH({ atb_timespec_Set(nullptr, 0, {1, 1}); }, "self != NULL");
  EXPECT_DEBUG_DEATH({ atb_timespec_Set(&ts, 0, {1, 0}); }, "den != 0");
}

TEST(AtbTimeTest, Compare) {
  // TESTS EQ
  for (auto [lhs, rhs] : std::array{
           std::array{timespec{1, 2}, timespec{1, 2}},
           std::array{timespec{3, 2}, timespec{3, 2}},
           std::array{timespec{-3, 2}, timespec{-3, 2}},
           std::array{timespec{-3, -2}, timespec{-3, -2}},
           std::array{ToTimespec(10s), ToTimespec(10000ms)},
       }) {
    SCOPED_TRACE(SCOPE_LOOP_MSG_2(lhs, rhs));

    EXPECT_EQ(K_ATB_TIMESPEC_CMP_EQUAL, atb_timespec_Compare(lhs, rhs));
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
           std::array{ToTimespec(10s), ToTimespec(10ms)},
       }) {
    SCOPED_TRACE(SCOPE_LOOP_MSG_2(lhs, rhs));

    EXPECT_NE(K_ATB_TIMESPEC_CMP_EQUAL, atb_timespec_Compare(lhs, rhs));
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
           std::array{ToTimespec(10s), ToTimespec(10ms)},
       }) {
    SCOPED_TRACE(SCOPE_LOOP_MSG_2(lhs, rhs));

    EXPECT_FALSE(atb_timespec_Eq(lhs, rhs));
    EXPECT_TRUE(atb_timespec_Ne(lhs, rhs));

    EXPECT_EQ(K_ATB_TIMESPEC_CMP_GREATER, atb_timespec_Compare(lhs, rhs));
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
           std::array{ToTimespec(10us), ToTimespec(10ms)},
       }) {
    SCOPED_TRACE(SCOPE_LOOP_MSG_2(lhs, rhs));

    EXPECT_FALSE(atb_timespec_Eq(lhs, rhs));
    EXPECT_TRUE(atb_timespec_Ne(lhs, rhs));

    EXPECT_EQ(K_ATB_TIMESPEC_CMP_LESS, atb_timespec_Compare(lhs, rhs));
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

TEST(AtbTimeTest, RetryCall) {
  using ::testing::_;
  using ::testing::DoAll;
  using ::testing::Return;

  const auto mock_fn = &RetryCallPredicateMock::Fn;
  auto &mock = RetryCallPredicateMock::Inst();
  ON_CALL(mock, Predicate(_)).WillByDefault(Return(true));

  // Test count = 0
  EXPECT_CALL(mock, Predicate(nullptr))
      .WillOnce(Return(false))
      .WillOnce(Return(true))
      .RetiresOnSaturation();

  EXPECT_FALSE(atb_Time_RetryCall({mock_fn, nullptr}, 0, ToTimespec(0s)));

  EXPECT_TRUE(atb_Time_RetryCall({mock_fn, nullptr}, 0, ToTimespec(0s)));

  // Test args ptr
  int foo = 2;
  EXPECT_CALL(mock, Predicate(&foo))
      .WillOnce(Return(false))
      .WillOnce(Return(true))
      .RetiresOnSaturation();

  EXPECT_FALSE(atb_Time_RetryCall({mock_fn, &foo}, 0, ToTimespec(0ns)));
  EXPECT_TRUE(atb_Time_RetryCall({mock_fn, &foo}, 0, ToTimespec(0ns)));

  // Test delays in between calls
  using clk = std::chrono::high_resolution_clock;
  using time_point = clk::time_point;

  constexpr auto delay = 50ms;
  constexpr auto count = 100u;

  std::vector<time_point> stamps;
  stamps.reserve(count + 1);

  EXPECT_CALL(mock, Predicate(nullptr))
      .Times(stamps.capacity())
      .WillRepeatedly(
          DoAll(WriteStampInto(std::back_inserter(stamps)), Return(false)))
      .RetiresOnSaturation();

  auto elapsed = clk::now().time_since_epoch();

  auto succeed =
      atb_Time_RetryCall({mock_fn, nullptr}, count, ToTimespec(delay));

  elapsed = clk::now().time_since_epoch() - elapsed;

  EXPECT_FALSE(succeed);

  auto expected = (count * delay);
  EXPECT_LE(abs(elapsed - expected), expected * 0.05) // 5% error ?
      << SCOPE_LOOP_MSG_2(elapsed, expected);

  ASSERT_GE(stamps.size(), 1);
  EXPECT_EQ(stamps.size(), count + 1);

  {
    // Compute the mean
    elapsed = 0ms;

    auto stamp_t_0 = std::cbegin(stamps);
    auto stamp_t_1 = std::next(stamp_t_0);
    for (; stamp_t_1 != std::cend(stamps); ++stamp_t_0, ++stamp_t_1) {
      elapsed += (*stamp_t_1 - *stamp_t_0);
    }
    elapsed /= (stamps.size() - 1);

    // Compare to the expected value
    expected = delay;
    EXPECT_LE(abs(elapsed - expected), expected * 0.05) // 5% error ?
        << SCOPE_LOOP_MSG_2(elapsed, expected);
  }
}

TEST(AtbTimeDeathTest, RetryCall) {
  EXPECT_DEBUG_DEATH(atb_Time_RetryCall({nullptr, nullptr}, 0, {});
                     , "function != NULL");
}

} // namespace
