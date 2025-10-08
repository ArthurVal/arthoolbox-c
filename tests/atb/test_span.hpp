#pragma once

#include <ostream>

#include "atb/span.h"
#include "utils.hpp"

namespace atb {

auto operator<<(std::ostream &os,
                const atb_View_Copy_Opt_t &opt) -> std::ostream &;

} // namespace atb
