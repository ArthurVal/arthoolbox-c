#pragma once

#include "Core.hpp"

#include "atb/memory/view.h"

#include <ostream>
#include <string_view>
#include <type_traits>

inline std::ostream &operator<<(std::ostream &os, atb_MemView mem) noexcept {

  os << helper::MakeStringFromFmt(atb_MemView_FMT, atb_MemView_FMT_VA_ARG(mem));

  if (atb_MemView_IsInvalid(mem)) {
    os << " => /!\\ INVALID";

  } else if ((mem.data != nullptr) and (mem.size > 0)) {
    os << " => 0x";

    char byte_str[3] = {'\0'};
    const uint8_t *byte = nullptr;
    atb_MemView_ForEachAs(uint8_t, byte, mem) {

      helper::IntToStr(byte_str, byte_str + 2, *byte,
                       {
                           /*.base= */ 16,
                           /*.fill = */ '0',
                       });

      os << " " << byte_str;
    }
  }

  return os;
}

namespace helper {

DefineFieldsMatchFor(atb_MemView, 2, data, size);

} // namespace helper
