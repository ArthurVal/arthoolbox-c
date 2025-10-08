#pragma once

#include "atb/span/ints.h"
#include "test_span.hpp"

namespace atb {

#define _DEFINE_FIELDMATCHERS(_, NAME, ...)             \
  DefineFieldsMatchFor(atb_Span_##NAME, 2, data, size); \
  DefineFieldsMatchFor(atb_View_##NAME, 2, data, size);

ATB_INTS_X_FOREACH(_DEFINE_FIELDMATCHERS)

#undef _DEFINE_FIELDMATCHERS

} // namespace atb

#define _DECLARE_STREAMS(_, NAME, ...)                              \
  auto operator<<(std::ostream &, atb_Span_##NAME)->std::ostream &; \
  auto operator<<(std::ostream &, atb_View_##NAME view)->std::ostream &;

ATB_INTS_X_FOREACH(_DECLARE_STREAMS)

#undef _DECLARE_STREAMS
