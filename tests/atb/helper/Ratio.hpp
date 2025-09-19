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

constexpr auto ToString(ATB_RATIO_CMP cmp_res) -> std::string_view {
  switch (cmp_res) {
    case K_ATB_RATIO_CMP_EQUAL:
      return "EQUAL";
    case K_ATB_RATIO_CMP_LESS:
      return "LESS";
    case K_ATB_RATIO_CMP_GREATER:
      return "GREATER";
  }
  return "";
}

inline auto operator<<(std::ostream &os,
                       ATB_RATIO_CMP cmp_res) -> std::ostream & {
  os << std::quoted(ToString(cmp_res)) << " (" << static_cast<int>(cmp_res)
     << ")";
  return os;
}

namespace helper {

DefineFieldsMatchFor(atb_Ratio, 2, num, den);

} // namespace helper
