#pragma once

#include <cstdint>
#include <ostream>

#include "atb/span.h"
#include "utils.hpp"

namespace atb {

auto operator<<(std::ostream &os,
                const atb_View_Copy_Opt_t &opt) -> std::ostream &;

ATB_SPAN_VIEW_DECLARE(, Span_u32, View_u32, std::uint32_t);
auto operator<<(std::ostream &os, Span_u32 span) -> std::ostream &;
auto operator==(Span_u32 lhs, Span_u32 rhs) noexcept -> bool;
auto operator!=(Span_u32 lhs, Span_u32 rhs) noexcept -> bool;

auto operator<<(std::ostream &os, View_u32 span) -> std::ostream &;
auto operator==(View_u32 lhs, View_u32 rhs) noexcept -> bool;
auto operator!=(View_u32 lhs, View_u32 rhs) noexcept -> bool;

} // namespace atb
