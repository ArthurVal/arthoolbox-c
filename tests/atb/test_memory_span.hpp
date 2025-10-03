#pragma once

#include <ostream>

#include "atb/memory/span.h"
#include "utils.hpp"

auto operator<<(std::ostream &os, atb_MemSpan mem) -> std::ostream &;

constexpr auto operator==(atb_MemSpan lhs, atb_MemSpan rhs) noexcept -> bool {
  return (lhs.data == rhs.data) && (lhs.size == rhs.size);
}

constexpr auto operator!=(atb_MemSpan lhs, atb_MemSpan rhs) noexcept -> bool {
  return (lhs.data == rhs.data) && (lhs.size == rhs.size);
}

namespace atb {

DefineFieldsMatchFor(atb_MemSpan, 2, data, size);

} // namespace atb
