#pragma once

#include <string_view>

#include "atb/span/string.h"
#include "test_span.hpp"

auto operator<<(std::ostream &os, atb_StrSpan span) -> std::ostream &;
auto operator<<(std::ostream &os, atb_StrView view) -> std::ostream &;

namespace atb {

DefineFieldsMatchFor(atb_StrSpan, 2, data, size);
DefineFieldsMatchFor(atb_StrView, 2, data, size);

constexpr auto ToSv(atb_StrView view) -> std::string_view {
  return std::string_view(view.data, view.size);
}

constexpr auto ToSv(atb_StrSpan span) -> std::string_view {
  return std::string_view(span.data, span.size);
}

} // namespace atb
