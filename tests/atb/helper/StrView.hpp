#pragma once

#include "Core.hpp"

#include "atb/str_view.h"

#include <ostream>
#include <string_view>
#include <type_traits>

inline std::ostream &operator<<(std::ostream &os, atb_StrView str) noexcept {
  os << helper::MakeStringFromFmt(atb_StrView_FMT, atb_StrView_FMT_VA_ARG(str));
  if (str.data != nullptr) {
    os << " => "
       << helper::MakeStringFromFmt("\"" atb_Str_FMT "\"",
                                    atb_Str_FMT_VA_ARG(str));
  }
  return os;
}

inline std::ostream &operator<<(std::ostream &os,
                                atb_StrView_Compare_Result res) noexcept {
  os << (int)res;
  os << " (=\"" << [res]() {
    switch (res) {
    case atb_StrView_Compare_LESS:
      return "LESS";
    case atb_StrView_Compare_EQUAL:
      return "EQUAL";
    case atb_StrView_Compare_GREATER:
      return "GREATER";
    }
    return "UNKNOWN";
  }() << "\")";
  return os;
}

constexpr auto ToStringView(struct atb_StrView str) -> std::string_view {
  return std::string_view(str.data, str.size);
}

// StrView
// Cmp to std::string_view ////////////////////////////////////////////////////
constexpr bool operator==(struct atb_StrView lhs, std::string_view rhs) {
  return ToStringView(lhs) == rhs;
}

constexpr bool operator==(std::string_view lhs, struct atb_StrView rhs) {
  return rhs == lhs;
}

// Cmp to itself //////////////////////////////////////////////////////////////
constexpr bool operator==(struct atb_StrView lhs, struct atb_StrView rhs) {
  return lhs == ToStringView(rhs);
}

constexpr bool operator!=(struct atb_StrView lhs, struct atb_StrView rhs) {
  return not(lhs == rhs);
}

constexpr bool operator<(struct atb_StrView lhs, struct atb_StrView rhs) {
  return ToStringView(lhs) < ToStringView(rhs);
}

constexpr bool operator>(struct atb_StrView lhs, struct atb_StrView rhs) {
  return rhs < lhs;
}

constexpr bool operator<=(struct atb_StrView lhs, struct atb_StrView rhs) {
  return not(lhs > rhs);
}

constexpr bool operator>=(struct atb_StrView lhs, struct atb_StrView rhs) {
  return not(lhs < rhs);
}

namespace helper {
DefineFieldsMatchFor(atb_StrView, 2, data, size);
}
