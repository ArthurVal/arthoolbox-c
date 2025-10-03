#pragma once

#include <optional>

#include "atb/compare.h"
#include "utils.hpp"

namespace atb {

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

} // namespace atb

auto operator<<(std::ostream &os, atb_Cmp_t cmp_res) -> std::ostream &;
