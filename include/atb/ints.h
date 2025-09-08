#pragma once

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

/// X-Macro iterating over all integers sizes (8, 16, 32, 64)
///
/// The `DO` parameter is expected to be a macro that will be evaluated with the
/// following signature:
/// DO(INT_SIZE) With:
///  - INT_SIZE: The size (in bits) of the int type (e.g. 8, 16, 32, ...);
#define ATB_INTS_X_FOREACH_SIZES(DO) DO(8) DO(16) DO(32) DO(64)

/// X-Macro iterating over ALL unsigned integers and their sizes
///
/// The `DO` parameter is expected to be a macro that will be evaluated with the
/// following signature:
/// DO(INT_T, INT_SIZE, INT_ABBREV) With:
///  - INT_T     : The underlying type (e.g. `int8_t`, `int32_t`, ...);
///  - INT_SIZE  : The size (in bits) of the int type (e.g. 8, 16, 32, ...);
///  - INT_ABBREV: The common abbrev associated (e.g. i8, u16, i32, ...);
#define ATB_INTS_X_FOREACH_UNSIGNED(DO)                                        \
  DO(uintmax_t, MAX, umax)                                                     \
  DO(uint8_t, 8, u8)                                                           \
  DO(uint16_t, 16, u16)                                                        \
  DO(uint32_t, 32, u32)                                                        \
  DO(uint64_t, 64, u64)

/// X-Macro iterating over ALL signed integers and their sizes
///
/// The `DO` parameter is expected to be a macro that will be evaluated with the
/// following signature:
/// DO(INT_T, INT_SIZE, INT_ABBREV) With:
///  - INT_T     : The underlying type (e.g. `int8_t`, `int32_t`, ...);
///  - INT_SIZE  : The size (in bits) of the int type (e.g. 8, 16, 32, ...);
///  - INT_ABBREV: The common abbrev associated (e.g. i8, u16, i32, ...);
#define ATB_INTS_X_FOREACH_SIGNED(DO)                                          \
  DO(intmax_t, MAX, imax)                                                      \
  DO(int8_t, 8, i8)                                                            \
  DO(int16_t, 16, i16)                                                         \
  DO(int32_t, 32, i32)                                                         \
  DO(int64_t, 64, i64)

/// X-Macro iterating over ALL integers (unsigned/signed) and their sizes
///
/// The `DO` parameter is expected to be a macro that will be evaluated with the
/// following signature:
/// DO(INT_T, INT_SIZE, INT_ABBREV) With:
///  - INT_T     : The underlying type (e.g. `int8_t`, `int32_t`, ...);
///  - INT_SIZE  : The size (in bits) of the int type (e.g. 8, 16, 32, ...);
///  - INT_ABBREV: The common abbrev associated (e.g. i8, u16, i32, ...);
#define ATB_INTS_X_FOREACH(DO)                                                 \
  ATB_INTS_X_FOREACH_UNSIGNED(DO) ATB_INTS_X_FOREACH_SIGNED(DO)

/***************************************************************************/
/*                        IsOverflowing/Underflowing                       */
/***************************************************************************/

/* SIGNED ********************************************************************/

#define _DEFINE_ADD_OVERFLOWING_SIGNED(TYPE, SIZE, NAME)                       \
  static inline bool atb_Add_IsOverflowing_##NAME(TYPE lhs, TYPE rhs) {        \
    return (rhs > 0) && (lhs > (INT##SIZE##_MAX - rhs));                       \
  }

#define _DEFINE_SUB_OVERFLOWING_SIGNED(TYPE, SIZE, NAME)                       \
  static inline bool atb_Sub_IsOverflowing_##NAME(TYPE lhs, TYPE rhs) {        \
    return (rhs < 0) && (lhs > (INT##SIZE##_MAX + rhs));                       \
  }

#define _DEFINE_MUL_OVERFLOWING_SIGNED(TYPE, SIZE, NAME)                       \
  static inline bool atb_Mul_IsOverflowing_##NAME(TYPE lhs, TYPE rhs) {        \
    if ((lhs > 0) && (rhs > 0)) {                                              \
      return lhs > (INT##SIZE##_MAX / rhs);                                    \
    } else if ((lhs < 0) && (rhs < 0)) {                                       \
      return lhs < (INT##SIZE##_MAX / rhs);                                    \
    } else {                                                                   \
      return false;                                                            \
    }                                                                          \
  }

#define _DEFINE_ADD_UNDERFLOWING_SIGNED(TYPE, SIZE, NAME)                      \
  static inline bool atb_Add_IsUnderflowing_##NAME(TYPE lhs, TYPE rhs) {       \
    return (rhs < 0) && (lhs < (INT##SIZE##_MIN - rhs));                       \
  }

#define _DEFINE_SUB_UNDERFLOWING_SIGNED(TYPE, SIZE, NAME)                      \
  static inline bool atb_Sub_IsUnderflowing_##NAME(TYPE lhs, TYPE rhs) {       \
    return (rhs > 0) && (lhs < (INT##SIZE##_MIN + rhs));                       \
  }

#define _DEFINE_MUL_UNDERFLOWING_SIGNED(TYPE, SIZE, NAME)                      \
  static inline bool atb_Mul_IsUnderflowing_##NAME(TYPE lhs, TYPE rhs) {       \
    if ((lhs > 0) && (rhs < 0)) {                                              \
      return lhs > (INT##SIZE##_MIN / rhs);                                    \
    } else if ((lhs < 0) && (rhs > 0)) {                                       \
      return lhs < (INT##SIZE##_MIN / rhs);                                    \
    } else {                                                                   \
      return false;                                                            \
    }                                                                          \
  }

#define _DEFINE_ALL_SIGNED(...)                                                \
  _DEFINE_ADD_OVERFLOWING_SIGNED(__VA_ARGS__)                                  \
  _DEFINE_SUB_OVERFLOWING_SIGNED(__VA_ARGS__)                                  \
  _DEFINE_MUL_OVERFLOWING_SIGNED(__VA_ARGS__)                                  \
  _DEFINE_ADD_UNDERFLOWING_SIGNED(__VA_ARGS__)                                 \
  _DEFINE_SUB_UNDERFLOWING_SIGNED(__VA_ARGS__)                                 \
  _DEFINE_MUL_UNDERFLOWING_SIGNED(__VA_ARGS__)

ATB_INTS_X_FOREACH_SIGNED(_DEFINE_ALL_SIGNED)

#undef _DEFINE_ALL_SIGNED
#undef _DEFINE_ADD_OVERFLOWING_SIGNED
#undef _DEFINE_SUB_OVERFLOWING_SIGNED
#undef _DEFINE_MUL_OVERFLOWING_SIGNED
#undef _DEFINE_ADD_UNDERFLOWING_SIGNED
#undef _DEFINE_SUB_UNDERFLOWING_SIGNED
#undef _DEFINE_MUL_UNDERFLOWING_SIGNED

/* UNSIGNED ******************************************************************/

#define _DEFINE_ADD_OVERFLOWING_UNSIGNED(TYPE, SIZE, NAME)                     \
  static inline bool atb_Add_IsOverflowing_##NAME(TYPE lhs, TYPE rhs) {        \
    return (rhs > 0) && (lhs > (UINT##SIZE##_MAX - rhs));                      \
  }

#define _DEFINE_SUB_OVERFLOWING_UNSIGNED(TYPE, SIZE, NAME)                     \
  static inline bool atb_Sub_IsOverflowing_##NAME(TYPE lhs, TYPE rhs) {        \
    (void)lhs;                                                                 \
    (void)rhs;                                                                 \
    return false;                                                              \
  }

#define _DEFINE_MUL_OVERFLOWING_UNSIGNED(TYPE, SIZE, NAME)                     \
  static inline bool atb_Mul_IsOverflowing_##NAME(TYPE lhs, TYPE rhs) {        \
    return (rhs != 0) && (lhs > (UINT##SIZE##_MAX / rhs));                     \
  }

#define _DEFINE_ADD_UNDERFLOWING_UNSIGNED(TYPE, SIZE, NAME)                    \
  static inline bool atb_Add_IsUnderflowing_##NAME(TYPE lhs, TYPE rhs) {       \
    (void)lhs;                                                                 \
    (void)rhs;                                                                 \
    return false;                                                              \
  }

#define _DEFINE_SUB_UNDERFLOWING_UNSIGNED(TYPE, SIZE, NAME)                    \
  static inline bool atb_Sub_IsUnderflowing_##NAME(TYPE lhs, TYPE rhs) {       \
    return (lhs < rhs);                                                        \
  }

#define _DEFINE_MUL_UNDERFLOWING_UNSIGNED(TYPE, SIZE, NAME)                    \
  static inline bool atb_Mul_IsUnderflowing_##NAME(TYPE lhs, TYPE rhs) {       \
    (void)lhs;                                                                 \
    (void)rhs;                                                                 \
    return false;                                                              \
  }

#define _DEFINE_ALL_UNSIGNED(...)                                              \
  _DEFINE_ADD_OVERFLOWING_UNSIGNED(__VA_ARGS__)                                \
  _DEFINE_SUB_OVERFLOWING_UNSIGNED(__VA_ARGS__)                                \
  _DEFINE_MUL_OVERFLOWING_UNSIGNED(__VA_ARGS__)                                \
  _DEFINE_ADD_UNDERFLOWING_UNSIGNED(__VA_ARGS__)                               \
  _DEFINE_SUB_UNDERFLOWING_UNSIGNED(__VA_ARGS__)                               \
  _DEFINE_MUL_UNDERFLOWING_UNSIGNED(__VA_ARGS__)

ATB_INTS_X_FOREACH_UNSIGNED(_DEFINE_ALL_UNSIGNED)

#undef _DEFINE_ALL_UNSIGNED
#undef _DEFINE_ADD_OVERFLOWING_UNSIGNED
#undef _DEFINE_SUB_OVERFLOWING_UNSIGNED
#undef _DEFINE_MUL_OVERFLOWING_UNSIGNED
#undef _DEFINE_ADD_UNDERFLOWING_UNSIGNED
#undef _DEFINE_SUB_UNDERFLOWING_UNSIGNED
#undef _DEFINE_MUL_UNDERFLOWING_UNSIGNED

#if defined(__cplusplus)
} /* extern "C" */
#endif
