#pragma once

#include <chrono>
#include <ostream>

#include "atb/time.h"
#include "test_compare.hpp"
#include "test_ratio.hpp"
#include "utils.hpp"

constexpr auto ToTimespec(std::chrono::nanoseconds d) -> timespec {
  return {
      .tv_sec = d.count() / 1'000'000'000,
      .tv_nsec = d.count() % 1'000'000'000,
  };
}

constexpr auto operator==(timespec lhs, timespec rhs) -> bool {
  return (lhs.tv_sec == rhs.tv_sec) && (lhs.tv_nsec == rhs.tv_nsec);
}

constexpr auto operator!=(timespec lhs, timespec rhs) -> bool {
  return !(lhs == rhs);
}

auto operator<<(std::ostream &os, timespec ts) -> std::ostream &;

namespace std {

template <class Rep, class Period>
constexpr auto operator<<(
    std::ostream &os, std::chrono::duration<Rep, Period> d) -> std::ostream & {
  os << d.count();
  if constexpr (std::is_same_v<Period, std::nano>) {
    os << "ns";
  } else if constexpr (std::is_same_v<Period, std::micro>) {
    os << "us";
  } else if constexpr (std::is_same_v<Period, std::milli>) {
    os << "ms";
  } else if constexpr (std::is_same_v<Period, std::ratio<1>>) {
    os << "s";
  } else if constexpr (Period::type::den == 1) {
    os << "[" << Period::type::num << "]s";
  } else {
    os << "[" << Period::type::num << '/' << Period::type::den << "]s";
  }

  return os;
}

} // namespace std

namespace atb {

DefineFieldsMatchFor(timespec, 2, tv_sec, tv_nsec);

} // namespace atb
