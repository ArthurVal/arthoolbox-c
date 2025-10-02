#pragma once

#include <ostream>
#include <string_view>

#include "Core.hpp"
#include "atb/string/format.h"
#include "atb/string/span.h"

constexpr auto ToSV(atb_StrSpan atb_span) -> std::string_view {
  return std::string_view(atb_span.data, atb_span.size);
}

inline auto operator<<(std::ostream &os, atb_StrSpan span) -> std::ostream & {
  using helper::MakeStringFromFmt;

  os << MakeStringFromFmt(K_ATB_FMT_STR_RAW, ATB_FMT_VA_ARG_STR_RAW(span));

  if (span.data != nullptr) {
    os << " --> "
       << MakeStringFromFmt(K_ATB_FMT_STR_QUOTED,
                            ATB_FMT_VA_ARG_STR_QUOTED(span));
  }

  return os;
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

namespace helper {
DefineFieldsMatchFor(atb_StrSpan, 2, data, size);
}
