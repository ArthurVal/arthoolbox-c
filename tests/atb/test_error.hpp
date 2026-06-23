#pragma once

#include <ostream>

#include "atb/error.h"
#include "utils.hpp"

auto operator<<(std::ostream &os, const atb_Error &err) -> std::ostream &;
auto operator<<(std::ostream &os, const atb_Error *const err) -> std::ostream &;

auto operator==(atb_Error lhs, atb_Error rhs) -> bool;

namespace atb {

DefineFieldsMatchFor(atb_Error, 2, category, code);

} // namespace atb
