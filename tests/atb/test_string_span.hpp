#pragma once

#include <ostream>
#include <string_view>

#include "atb/string/span.h"
#include "utils.hpp"

constexpr auto ToSV(atb_StrSpan atb_span) -> std::string_view {
  return std::string_view(atb_span.data, atb_span.size);
}

// Cmp to std::string_view ////////////////////////////////////////////////////
constexpr bool operator==(atb_StrSpan lhs, std::string_view rhs) {
  return ToSV(lhs) == rhs;
}

constexpr bool operator==(std::string_view lhs, atb_StrSpan rhs) {
  return rhs == lhs;
}

// Cmp to itself //////////////////////////////////////////////////////////////
constexpr bool operator==(atb_StrSpan lhs, atb_StrSpan rhs) {
  return lhs == ToSV(rhs);
}

constexpr bool operator!=(atb_StrSpan lhs, atb_StrSpan rhs) {
  return not(lhs == rhs);
}

constexpr bool operator<(atb_StrSpan lhs, atb_StrSpan rhs) {
  return ToSV(lhs) < ToSV(rhs);
}

constexpr bool operator>(atb_StrSpan lhs, atb_StrSpan rhs) { return rhs < lhs; }

constexpr bool operator<=(atb_StrSpan lhs, atb_StrSpan rhs) {
  return not(lhs > rhs);
}

constexpr bool operator>=(atb_StrSpan lhs, atb_StrSpan rhs) {
  return not(lhs < rhs);
}

auto operator<<(std::ostream &os, atb_StrSpan span) -> std::ostream &;

namespace atb {
DefineFieldsMatchFor(atb_StrSpan, 2, data, size);
}
