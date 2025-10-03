#pragma once

#include <ostream>

#include "atb/ratio.h"
#include "test_compare.hpp"
#include "utils.hpp"

constexpr auto operator==(atb_Ratio lhs, atb_Ratio rhs) -> bool {
  return (lhs.num == rhs.num) && (lhs.den == rhs.den);
}

constexpr auto operator!=(atb_Ratio lhs, atb_Ratio rhs) -> bool {
  return !(lhs == rhs);
}

auto operator<<(std::ostream &os, atb_Ratio ratio) -> std::ostream &;

namespace atb {

DefineFieldsMatchFor(atb_Ratio, 2, num, den);

} // namespace atb
