#pragma once

#define atb_internal_Macro_ARGN_20 \
  20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#define atb_internal_Macro_ARGN_IMPL_20(_0, _1, _2, _3, _4, _5, _6, _7, _8,    \
                                        _9, _10, _11, _12, _13, _14, _15, _16, \
                                        _17, _18, _19, _20, N, ...)            \
  N

#define atb_internal_Macro_ARGN(...) \
  atb_internal_Macro_ARGN_IMPL_20(__VA_ARGS__)
