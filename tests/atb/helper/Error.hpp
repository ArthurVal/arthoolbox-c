#pragma once

#include <ostream>

#include "Core.hpp"
#include "atb/error.h"

inline auto operator<<(std::ostream &os,
                       const atb_Error &err) -> std::ostream & {
  os << "atb_Error"
     << helper::MakeStringFromFmt(K_ATB_ERROR_FMT, atb_Error_FMT_VA_ARG(err));
  return os;
}

inline auto operator<<(std::ostream &os,
                       const atb_Error *const err) -> std::ostream & {
  return helper::PrintPtrTo(os, err);
}

namespace helper {} // namespace helper
