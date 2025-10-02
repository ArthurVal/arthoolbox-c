#pragma once

#include <ostream>

#include "Core.hpp"
#include "atb/memory/format.h"
#include "atb/memory/view.h"

inline std::ostream &operator<<(std::ostream &os, atb_MemView mem) noexcept {
  os << helper::MakeStringFromFmt(K_ATB_FMT_MEM, ATB_FMT_VA_ARG_MEM(mem));

  if (mem.data == nullptr) {
    os << " -> /!\\ INVALID";
  } else if (mem.size != 0) {
    const uint8_t *byte = (const uint8_t *)mem.data;
    const uint8_t *const end = (byte + mem.size);

    os << " -> 0x";

    const auto flags = os.flags();

    os << std::hex;
    while (byte != end) os << " " << (int)(*byte++);

    os.flags(flags);
  }

  return os;
}

namespace helper {

DefineFieldsMatchFor(atb_MemView, 2, data, size);

} // namespace helper
