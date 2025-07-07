#pragma once

#include <chrono>
#include <ostream>

#include "Core.hpp"
#include "atb/time.h"

inline auto operator<<(std::ostream &os, timespec ts) -> std::ostream & {
  return os << "timespec{.tv_sec=" << ts.tv_sec << ", .tv_nsec=" << ts.tv_nsec
            << "}";
}

constexpr auto operator==(timespec lhs, timespec rhs) -> bool {
  return (lhs.tv_sec == rhs.tv_sec) && (lhs.tv_nsec == rhs.tv_nsec);
}

constexpr auto operator!=(timespec lhs, timespec rhs) -> bool {
  return !(lhs == rhs);
}

constexpr auto ToString(atb_timespec_Compare_Result cmp_res)
    -> std::string_view {
  switch (cmp_res) {
    case atb_timespec_Compare_EQUAL: return "EQUAL";
    case atb_timespec_Compare_LESS: return "LESS";
    case atb_timespec_Compare_GREATER: return "GREATER";
  }
  return "UNKNOWN";
}

inline auto operator<<(std::ostream &os,
                       atb_timespec_Compare_Result cmp_res) -> std::ostream & {
  os << std::quoted(ToString(cmp_res)) << " (" << static_cast<int>(cmp_res)
     << ")";
  return os;
}

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
namespace helper {

DefineFieldsMatchFor(timespec, 2, tv_sec, tv_nsec);

} // namespace helper
