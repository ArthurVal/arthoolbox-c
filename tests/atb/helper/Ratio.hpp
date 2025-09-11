#pragma once

#include <ostream>

#include "Core.hpp"
#include "atb/ratio.h"

inline auto operator<<(std::ostream &os, atb_Ratio ratio) -> std::ostream & {
  return os << "Ratio{" << ratio.num << ", " << ratio.den << "}";
}

constexpr auto operator==(atb_Ratio lhs, atb_Ratio rhs) -> bool {
  return (lhs.num == rhs.num) && (lhs.den == rhs.den);
}

constexpr auto operator!=(atb_Ratio lhs, atb_Ratio rhs) -> bool {
  return !(lhs == rhs);
}

constexpr auto ToString(atb_Ratio_Compare_Result cmp_res) -> std::string_view {
  switch (cmp_res) {
    case atb_Ratio_Compare_EQUAL:
      return "EQUAL";
    case atb_Ratio_Compare_LESS:
      return "LESS";
    case atb_Ratio_Compare_GREATER:
      return "GREATER";
    case atb_Ratio_Compare_UNKNOWN:
      return "UNKNOWN";
  }
  return "";
}

inline auto operator<<(std::ostream &os,
                       atb_Ratio_Compare_Result cmp_res) -> std::ostream & {
  os << std::quoted(ToString(cmp_res)) << " (" << static_cast<int>(cmp_res)
     << ")";
  return os;
}

namespace helper {

DefineFieldsMatchFor(atb_Ratio, 2, num, den);

} // namespace helper
