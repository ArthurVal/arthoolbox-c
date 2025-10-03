#pragma once

#include <ostream>
#include <string_view>

#include "atb/string/view.h"
#include "test_compare.hpp"
#include "test_string_span.hpp"
#include "utils.hpp"

constexpr auto ToSV(atb_StrView atb_view) -> std::string_view {
  return std::string_view(atb_view.data, atb_view.size);
}

// Cmp to std::string_view ////////////////////////////////////////////////////
constexpr bool operator==(atb_StrView lhs, std::string_view rhs) {
  return ToSV(lhs) == rhs;
}

constexpr bool operator==(std::string_view lhs, atb_StrView rhs) {
  return rhs == lhs;
}

// Cmp to itself //////////////////////////////////////////////////////////////
constexpr bool operator==(atb_StrView lhs, atb_StrView rhs) {
  return lhs == ToSV(rhs);
}

constexpr bool operator!=(atb_StrView lhs, atb_StrView rhs) {
  return not(lhs == rhs);
}

constexpr bool operator<(atb_StrView lhs, atb_StrView rhs) {
  return ToSV(lhs) < ToSV(rhs);
}

constexpr bool operator>(atb_StrView lhs, atb_StrView rhs) { return rhs < lhs; }

constexpr bool operator<=(atb_StrView lhs, atb_StrView rhs) {
  return not(lhs > rhs);
}

constexpr bool operator>=(atb_StrView lhs, atb_StrView rhs) {
  return not(lhs < rhs);
}

auto operator<<(std::ostream &os, atb_StrView view) -> std::ostream &;

namespace atb {
DefineFieldsMatchFor(atb_StrView, 2, data, size);
}
