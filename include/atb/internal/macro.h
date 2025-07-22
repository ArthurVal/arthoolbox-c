#pragma once

#define atb_internal_ARGN_20 \
  20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#define atb_internal_ARGN_IMPL_20(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
                                  _11, _12, _13, _14, _15, _16, _17, _18, _19, \
                                  _20, N, ...)                                 \
  N

#define atb_internal_ARGN(...) atb_internal_ARGN_IMPL_20(__VA_ARGS__)

#define atb_internal_TKN_CONCAT_IMPL(a, b) a##b

#define atb_internal_TKN_CONCAT_0()
#define atb_internal_TKN_CONCAT_1(a) a
#define atb_internal_TKN_CONCAT_2(a, b) atb_internal_TKN_CONCAT_IMPL(a, b)
#define atb_internal_TKN_CONCAT_3(a, ...) \
  atb_internal_TKN_CONCAT_2(a, atb_internal_TKN_CONCAT_2(__VA_ARGS__))
#define atb_internal_TKN_CONCAT_4(a, ...) \
  atb_internal_TKN_CONCAT_2(a, atb_internal_TKN_CONCAT_3(__VA_ARGS__))
#define atb_internal_TKN_CONCAT_5(a, ...) \
  atb_internal_TKN_CONCAT_2(a, atb_internal_TKN_CONCAT_4(__VA_ARGS__))
#define atb_internal_TKN_CONCAT_6(a, ...) \
  atb_internal_TKN_CONCAT_2(a, atb_internal_TKN_CONCAT_5(__VA_ARGS__))
#define atb_internal_TKN_CONCAT_7(a, ...) \
  atb_internal_TKN_CONCAT_2(a, atb_internal_TKN_CONCAT_6(__VA_ARGS__))
#define atb_internal_TKN_CONCAT_8(a, ...) \
  atb_internal_TKN_CONCAT_2(a, atb_internal_TKN_CONCAT_7(__VA_ARGS__))
#define atb_internal_TKN_CONCAT_9(a, ...) \
  atb_internal_TKN_CONCAT_2(a, atb_internal_TKN_CONCAT_8(__VA_ARGS__))
#define atb_internal_TKN_CONCAT_10(a, ...) \
  atb_internal_TKN_CONCAT_2(a, atb_internal_TKN_CONCAT_9(__VA_ARGS__))

#define atb_internal_TKN_CONCAT_N_IMPL(N, ...) \
  atb_internal_TKN_CONCAT_##N(__VA_ARGS__)
#define atb_internal_TKN_CONCAT_N(N, ...) \
  atb_internal_TKN_CONCAT_N_IMPL(N, __VA_ARGS__)

#define atb_internal_TKN_STR(a) #a
