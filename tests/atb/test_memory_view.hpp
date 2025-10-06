#pragma once

#include <ostream>

#include "atb/memory/view.h"
#include "utils.hpp"
#include "test_memory_span.hpp"

std::ostream &operator<<(std::ostream &os, atb_MemView mem);

namespace atb {

DefineFieldsMatchFor(atb_MemView, 2, data, size);

} // namespace atb
