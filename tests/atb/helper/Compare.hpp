#pragma once

#include <optional>
#include <ostream>

#include "Core.hpp"
#include "atb/compare.h"

constexpr auto ToString(atb_Cmp_t cmp_res) -> std::optional<std::string_view> {
  switch (cmp_res) {
    case K_ATB_CMP_EQUAL:
      return "EQUAL";
    case K_ATB_CMP_LESS:
      return "LESS";
    case K_ATB_CMP_GREATER:
      return "GREATER";
  }

  return std::nullopt;
}

inline auto operator<<(std::ostream &os, atb_Cmp_t cmp_res) -> std::ostream & {
  os << std::quoted(ToString(cmp_res).value_or("<UNKNOWN>")) << " ("
     << static_cast<int>(cmp_res) << ")";
  return os;
}
