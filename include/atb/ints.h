#pragma once

#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
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
/// DO(T, ABBREV, MIN, MAX, SIZE) With:
///  - T     : The underlying type (e.g. `int8_t`, `int32_t`, ...);
///  - ABBREV: The common abbrev associated (e.g. i8, u16, i32, ...);
///  - MIN   : Minimum value accepted by this type
///  - MAX   : Maximum value accepted by this type
///  - SIZE  : The size (in bits) of the int type (e.g. 8, 16, 32, ...);
#define ATB_INTS_X_FOREACH_UNSIGNED(DO) \
  DO(uint8_t, u8, 0, UINT8_MAX, 8)      \
  DO(uint16_t, u16, 0, UINT16_MAX, 16)  \
  DO(uint32_t, u32, 0, UINT32_MAX, 32)  \
  DO(uint64_t, u64, 0, UINT64_MAX, 64)

/// X-Macro iterating over ALL signed integers and their sizes
///
/// The `DO` parameter is expected to be a macro that will be evaluated with the
/// following signature:
/// DO(T, ABBREV, MIN, MAX, SIZE) With:
///  - T     : The underlying type (e.g. `int8_t`, `int32_t`, ...);
///  - ABBREV: The common abbrev associated (e.g. i8, u16, i32, ...);
///  - MIN   : Minimum value accepted by this type
///  - MAX   : Maximum value accepted by this type
///  - SIZE  : The size (in bits) of the int type (e.g. 8, 16, 32, ...);
#define ATB_INTS_X_FOREACH_SIGNED(DO)        \
  DO(int8_t, i8, INT8_MIN, INT8_MAX, 8)      \
  DO(int16_t, i16, INT16_MIN, INT16_MAX, 16) \
  DO(int32_t, i32, INT32_MIN, INT32_MAX, 32) \
  DO(int64_t, i64, INT64_MIN, INT64_MAX, 64)

/// X-Macro iterating over ALL integers (unsigned/signed) and their sizes
///
/// The `DO` parameter is expected to be a macro that will be evaluated with the
/// following signature:
/// DO(T, ABBREV, MIN, MAX, SIZE) With:
///  - T     : The underlying type (e.g. `int8_t`, `int32_t`, ...);
///  - ABBREV: The common abbrev associated (e.g. i8, u16, i32, ...);
///  - MIN   : Minimum value accepted by this type
///  - MAX   : Maximum value accepted by this type
///  - SIZE  : The size (in bits) of the int type (e.g. 8, 16, 32, ...);
#define ATB_INTS_X_FOREACH(DO) \
  ATB_INTS_X_FOREACH_UNSIGNED(DO) ATB_INTS_X_FOREACH_SIGNED(DO)

/***************************************************************************/
/*                        IsOverflowing/Underflowing                       */
/***************************************************************************/

/* SIGNED ******************************************************************/

#define _DEFINE_ADD_ISOVERFLOWING_SIGNED(T, NAME, _, MAX, ...)    \
  static inline bool atb_Add_IsOverflowing_##NAME(T lhs, T rhs) { \
    return (rhs > 0) && (lhs > (MAX - rhs));                      \
  }

#define _DEFINE_SUB_ISOVERFLOWING_SIGNED(T, NAME, _, MAX, ...)    \
  static inline bool atb_Sub_IsOverflowing_##NAME(T lhs, T rhs) { \
    return (rhs < 0) && (lhs > (MAX + rhs));                      \
  }

#define _DEFINE_MUL_ISOVERFLOWING_SIGNED(T, NAME, _, MAX, ...)    \
  static inline bool atb_Mul_IsOverflowing_##NAME(T lhs, T rhs) { \
    if ((lhs > 0) && (rhs > 0)) {                                 \
      return lhs > (MAX / rhs);                                   \
    } else if ((lhs < 0) && (rhs < 0)) {                          \
      return lhs < (MAX / rhs);                                   \
    } else {                                                      \
      return false;                                               \
    }                                                             \
  }

#define _DEFINE_ADD_ISUNDERFLOWING_SIGNED(T, NAME, MIN, ...)       \
  static inline bool atb_Add_IsUnderflowing_##NAME(T lhs, T rhs) { \
    return (rhs < 0) && (lhs < (MIN - rhs));                       \
  }

#define _DEFINE_SUB_ISUNDERFLOWING_SIGNED(T, NAME, MIN, ...)       \
  static inline bool atb_Sub_IsUnderflowing_##NAME(T lhs, T rhs) { \
    return (rhs > 0) && (lhs < (MIN + rhs));                       \
  }

#define _DEFINE_MUL_ISUNDERFLOWING_SIGNED(T, NAME, MIN, ...)       \
  static inline bool atb_Mul_IsUnderflowing_##NAME(T lhs, T rhs) { \
    if ((lhs > 0) && (rhs < 0)) {                                  \
      return lhs > (MIN / rhs);                                    \
    } else if ((lhs < 0) && (rhs > 0)) {                           \
      return lhs < (MIN / rhs);                                    \
    } else {                                                       \
      return false;                                                \
    }                                                              \
  }

#define _DEFINE_ALL_ISXX_SIGNED(...)             \
  _DEFINE_ADD_ISOVERFLOWING_SIGNED(__VA_ARGS__)  \
  _DEFINE_SUB_ISOVERFLOWING_SIGNED(__VA_ARGS__)  \
  _DEFINE_MUL_ISOVERFLOWING_SIGNED(__VA_ARGS__)  \
  _DEFINE_ADD_ISUNDERFLOWING_SIGNED(__VA_ARGS__) \
  _DEFINE_SUB_ISUNDERFLOWING_SIGNED(__VA_ARGS__) \
  _DEFINE_MUL_ISUNDERFLOWING_SIGNED(__VA_ARGS__)

ATB_INTS_X_FOREACH_SIGNED(_DEFINE_ALL_ISXX_SIGNED)

#undef _DEFINE_ALL_ISXX_SIGNED
#undef _DEFINE_ADD_ISOVERFLOWING_SIGNED
#undef _DEFINE_SUB_ISOVERFLOWING_SIGNED
#undef _DEFINE_MUL_ISOVERFLOWING_SIGNED
#undef _DEFINE_ADD_ISUNDERFLOWING_SIGNED
#undef _DEFINE_SUB_ISUNDERFLOWING_SIGNED
#undef _DEFINE_MUL_ISUNDERFLOWING_SIGNED

/* UNSIGNED ******************************************************************/

#define _DEFINE_ADD_ISOVERFLOWING_UNSIGNED(T, NAME, _, MAX, ...)  \
  static inline bool atb_Add_IsOverflowing_##NAME(T lhs, T rhs) { \
    return (rhs > 0) && (lhs > (MAX - rhs));                      \
  }

#define _DEFINE_SUB_ISOVERFLOWING_UNSIGNED(T, NAME, ...)          \
  static inline bool atb_Sub_IsOverflowing_##NAME(T lhs, T rhs) { \
    (void)lhs;                                                    \
    (void)rhs;                                                    \
    return false;                                                 \
  }

#define _DEFINE_MUL_ISOVERFLOWING_UNSIGNED(T, NAME, _, MAX, ...)  \
  static inline bool atb_Mul_IsOverflowing_##NAME(T lhs, T rhs) { \
    return (rhs != 0) && (lhs > (MAX / rhs));                     \
  }

#define _DEFINE_ADD_ISUNDERFLOWING_UNSIGNED(T, NAME, ...)          \
  static inline bool atb_Add_IsUnderflowing_##NAME(T lhs, T rhs) { \
    (void)lhs;                                                     \
    (void)rhs;                                                     \
    return false;                                                  \
  }

#define _DEFINE_SUB_ISUNDERFLOWING_UNSIGNED(T, NAME, ...)          \
  static inline bool atb_Sub_IsUnderflowing_##NAME(T lhs, T rhs) { \
    return (lhs < rhs);                                            \
  }

#define _DEFINE_MUL_ISUNDERFLOWING_UNSIGNED(T, NAME, ...)          \
  static inline bool atb_Mul_IsUnderflowing_##NAME(T lhs, T rhs) { \
    (void)lhs;                                                     \
    (void)rhs;                                                     \
    return false;                                                  \
  }

#define _DEFINE_ALL_ISXX_UNSIGNED(...)             \
  _DEFINE_ADD_ISOVERFLOWING_UNSIGNED(__VA_ARGS__)  \
  _DEFINE_SUB_ISOVERFLOWING_UNSIGNED(__VA_ARGS__)  \
  _DEFINE_MUL_ISOVERFLOWING_UNSIGNED(__VA_ARGS__)  \
  _DEFINE_ADD_ISUNDERFLOWING_UNSIGNED(__VA_ARGS__) \
  _DEFINE_SUB_ISUNDERFLOWING_UNSIGNED(__VA_ARGS__) \
  _DEFINE_MUL_ISUNDERFLOWING_UNSIGNED(__VA_ARGS__)

ATB_INTS_X_FOREACH_UNSIGNED(_DEFINE_ALL_ISXX_UNSIGNED)

#undef _DEFINE_ALL_ISXX_UNSIGNED
#undef _DEFINE_ADD_ISOVERFLOWING_UNSIGNED
#undef _DEFINE_SUB_ISOVERFLOWING_UNSIGNED
#undef _DEFINE_MUL_ISOVERFLOWING_UNSIGNED
#undef _DEFINE_ADD_ISUNDERFLOWING_UNSIGNED
#undef _DEFINE_SUB_ISUNDERFLOWING_UNSIGNED
#undef _DEFINE_MUL_ISUNDERFLOWING_UNSIGNED

/*****************************************************************************/
/*                                  _Unsafe                                  */
/*****************************************************************************/

#define _DEFINE_ADD_UNSAFE(T, NAME, ...)                                  \
  static inline bool atb_Add_Unsafe_##NAME(T lhs, T rhs, T *const dest) { \
    assert(dest != NULL);                                                 \
    *dest = (lhs + rhs);                                                  \
    return true;                                                          \
  }

#define _DEFINE_SUB_UNSAFE(T, NAME, ...)                                  \
  static inline bool atb_Sub_Unsafe_##NAME(T lhs, T rhs, T *const dest) { \
    assert(dest != NULL);                                                 \
    *dest = (lhs - rhs);                                                  \
    return true;                                                          \
  }

#define _DEFINE_MUL_UNSAFE(T, NAME, ...)                                  \
  static inline bool atb_Mul_Unsafe_##NAME(T lhs, T rhs, T *const dest) { \
    assert(dest != NULL);                                                 \
    *dest = (lhs * rhs);                                                  \
    return true;                                                          \
  }

#define _DEFINE_ALL_UNSAFE(...)   \
  _DEFINE_ADD_UNSAFE(__VA_ARGS__) \
  _DEFINE_SUB_UNSAFE(__VA_ARGS__) \
  _DEFINE_MUL_UNSAFE(__VA_ARGS__)

ATB_INTS_X_FOREACH(_DEFINE_ALL_UNSAFE)

#undef _DEFINE_ALL_UNSAFE
#undef _DEFINE_MUL_UNSAFE
#undef _DEFINE_SUB_UNSAFE
#undef _DEFINE_ADD_UNSAFE

/*****************************************************************************/
/*                                  _Safely                                  */
/*****************************************************************************/

#define _DEFINE_ADD_SAFELY(T, NAME, ...)                                  \
  static inline bool atb_Add_Safely_##NAME(T lhs, T rhs, T *const dest) { \
    assert(dest != NULL);                                                 \
                                                                          \
    bool success = !(atb_Add_IsOverflowing_##NAME(lhs, rhs) ||            \
                     atb_Add_IsUnderflowing_##NAME(lhs, rhs));            \
                                                                          \
    if (success) {                                                        \
      *dest = (lhs + rhs);                                                \
    }                                                                     \
                                                                          \
    return success;                                                       \
  }

#define _DEFINE_SUB_SAFELY(T, NAME, ...)                                  \
  static inline bool atb_Sub_Safely_##NAME(T lhs, T rhs, T *const dest) { \
    assert(dest != NULL);                                                 \
                                                                          \
    bool success = !(atb_Sub_IsOverflowing_##NAME(lhs, rhs) ||            \
                     atb_Sub_IsUnderflowing_##NAME(lhs, rhs));            \
                                                                          \
    if (success) {                                                        \
      *dest = (lhs - rhs);                                                \
    }                                                                     \
                                                                          \
    return success;                                                       \
  }

#define _DEFINE_MUL_SAFELY(T, NAME, ...)                                  \
  static inline bool atb_Mul_Safely_##NAME(T lhs, T rhs, T *const dest) { \
    assert(dest != NULL);                                                 \
                                                                          \
    bool success = !(atb_Mul_IsOverflowing_##NAME(lhs, rhs) ||            \
                     atb_Mul_IsUnderflowing_##NAME(lhs, rhs));            \
                                                                          \
    if (success) {                                                        \
      *dest = (lhs * rhs);                                                \
    }                                                                     \
                                                                          \
    return success;                                                       \
  }

#define _DEFINE_ALL_SAFELY(...)   \
  _DEFINE_ADD_SAFELY(__VA_ARGS__) \
  _DEFINE_SUB_SAFELY(__VA_ARGS__) \
  _DEFINE_MUL_SAFELY(__VA_ARGS__)

ATB_INTS_X_FOREACH(_DEFINE_ALL_SAFELY)

#undef _DEFINE_ALL_SAFELY
#undef _DEFINE_MUL_SAFELY
#undef _DEFINE_SUB_SAFELY
#undef _DEFINE_ADD_SAFELY

/*****************************************************************************/
/*                                 _Saturate                                 */
/*****************************************************************************/

#define _DEFINE_ADD_SATURATE(T, NAME, MIN, MAX, ...)                        \
  static inline bool atb_Add_Saturate_##NAME(T lhs, T rhs, T *const dest) { \
    assert(dest != NULL);                                                   \
                                                                            \
    bool success = true;                                                    \
                                                                            \
    if (atb_Add_IsOverflowing_##NAME(lhs, rhs)) {                           \
      success = false;                                                      \
      *dest = MAX;                                                          \
    } else if (atb_Add_IsUnderflowing_##NAME(lhs, rhs)) {                   \
      success = false;                                                      \
      *dest = MIN;                                                          \
    } else {                                                                \
      *dest = (lhs + rhs);                                                  \
    }                                                                       \
                                                                            \
    return success;                                                         \
  }

#define _DEFINE_SUB_SATURATE(T, NAME, MIN, MAX, ...)                        \
  static inline bool atb_Sub_Saturate_##NAME(T lhs, T rhs, T *const dest) { \
    assert(dest != NULL);                                                   \
                                                                            \
    bool success = true;                                                    \
                                                                            \
    if (atb_Sub_IsOverflowing_##NAME(lhs, rhs)) {                           \
      success = false;                                                      \
      *dest = MAX;                                                          \
    } else if (atb_Sub_IsUnderflowing_##NAME(lhs, rhs)) {                   \
      success = false;                                                      \
      *dest = MIN;                                                          \
    } else {                                                                \
      *dest = (lhs - rhs);                                                  \
    }                                                                       \
                                                                            \
    return success;                                                         \
  }

#define _DEFINE_MUL_SATURATE(T, NAME, MIN, MAX, ...)                        \
  static inline bool atb_Mul_Saturate_##NAME(T lhs, T rhs, T *const dest) { \
    assert(dest != NULL);                                                   \
                                                                            \
    bool success = true;                                                    \
                                                                            \
    if (atb_Mul_IsOverflowing_##NAME(lhs, rhs)) {                           \
      success = false;                                                      \
      *dest = MAX;                                                          \
    } else if (atb_Mul_IsUnderflowing_##NAME(lhs, rhs)) {                   \
      success = false;                                                      \
      *dest = MIN;                                                          \
    } else {                                                                \
      *dest = (lhs * rhs);                                                  \
    }                                                                       \
                                                                            \
    return success;                                                         \
  }

#define _DEFINE_ALL_SATURATE(...)   \
  _DEFINE_ADD_SATURATE(__VA_ARGS__) \
  _DEFINE_SUB_SATURATE(__VA_ARGS__) \
  _DEFINE_MUL_SATURATE(__VA_ARGS__)

ATB_INTS_X_FOREACH(_DEFINE_ALL_SATURATE)

#undef _DEFINE_ALL_SATURATE
#undef _DEFINE_MUL_SATURATE
#undef _DEFINE_SUB_SATURATE
#undef _DEFINE_ADD_SATURATE

/*****************************************************************************/
/*                          _Arithmetic_Policy_t                             */
/*****************************************************************************/

#define _DECLARE_ALL_POLICY_TYPE(T, NAME, ...)  \
  typedef struct atb_Arithmetic_Policy_##NAME { \
    bool (*add)(T, T, T *const);                \
    bool (*sub)(T, T, T *const);                \
    bool (*mul)(T, T, T *const);                \
  } atb_Arithmetic_Policy_##NAME##_t;

ATB_INTS_X_FOREACH(_DECLARE_ALL_POLICY_TYPE)

#undef _DECLARE_ALL_POLICY_TYPE

#if defined(__cplusplus)
} /* extern "C" */
#endif
