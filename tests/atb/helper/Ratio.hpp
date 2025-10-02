#pragma once

#include <ostream>

#include "Compare.hpp"
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

namespace helper {

DefineFieldsMatchFor(atb_Ratio, 2, num, den);

} // namespace helper
