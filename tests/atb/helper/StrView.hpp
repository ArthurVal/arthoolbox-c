#pragma once

#include <ostream>
#include <string_view>

#include "Compare.hpp"
#include "Core.hpp"
#include "atb/string/format.h"
#include "atb/string/view.h"

constexpr auto ToSV(atb_StrView atb_view) -> std::string_view {
  return std::string_view(atb_view.data, atb_view.size);
}

inline auto operator<<(std::ostream &os, atb_StrView view) -> std::ostream & {
  using helper::MakeStringFromFmt;

  os << MakeStringFromFmt(K_ATB_FMT_STR_RAW, ATB_FMT_VA_ARG_STR_RAW(view));

  if (view.data != nullptr) {
    os << " --> "
       << MakeStringFromFmt(K_ATB_FMT_STR_QUOTED,
                            ATB_FMT_VA_ARG_STR_QUOTED(view));
  }

  return os;
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

namespace helper {
DefineFieldsMatchFor(atb_StrView, 2, data, size);
}
