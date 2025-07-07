#pragma once

#include <ostream>

#include "Ratio.hpp"
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

namespace helper {

DefineFieldsMatchFor(timespec, 2, tv_sec, tv_nsec);

} // namespace helper
