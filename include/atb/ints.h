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

/// `<OP>_IsOverflowing(lhs, rhs) -> bool` returns TRUE when `lhs <OP> rhs`
/// (with <OP> being one of +, -, *) overflows the underlying int type range.
/// FALSE otherwise (operation is safe).

/// `<OP>_IsUnderflowing(lhs, rhs) -> bool` returns TRUE when `lhs <OP> rhs`
/// (with <OP> being one of +, -, *) underflows the underlying int type range.
/// FALSE otherwise (operation is safe).

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
    if ((lhs > 0) && (rhs < -1)) {                                 \
      return (lhs > (MIN / rhs));                                  \
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

/// `<OP>_Unsafe(lhs, rhs, &dest) -> bool`
/// Perform `*dest = lhs <OP> rhs` (with <OP> being one of +, -, *) without
/// checking for overflows/underflows.
/// Always return TRUE.
///
/// \pre dest != NULL

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

/// `<OP>_Safely(lhs, rhs, &dest) -> bool`
/// Perform `*dest = lhs <OP> rhs` (with <OP> being one of +, -, *) when the
/// given operation is SAFE (i.e. no overflows nor underflows).
/// When the operation is performed, returns TRUE.
/// Otherwise (overflows/underflows), returns FALSE and dest is LEFT UNCHANGED.
///
/// \pre dest != NULL

#define _DEFINE_SAFELY(OP, T, NAME, ...)                                     \
  static inline bool atb_##OP##_Safely_##NAME(T lhs, T rhs, T *const dest) { \
    assert(dest != NULL);                                                    \
                                                                             \
    if (atb_##OP##_IsOverflowing_##NAME(lhs, rhs) ||                         \
        atb_##OP##_IsUnderflowing_##NAME(lhs, rhs)) {                        \
      return false;                                                          \
    } else {                                                                 \
      return atb_##OP##_Unsafe_##NAME(lhs, rhs, dest);                       \
    }                                                                        \
  }

#define _DEFINE_ALL_SAFELY(...)    \
  _DEFINE_SAFELY(Add, __VA_ARGS__) \
  _DEFINE_SAFELY(Sub, __VA_ARGS__) \
  _DEFINE_SAFELY(Mul, __VA_ARGS__)

ATB_INTS_X_FOREACH(_DEFINE_ALL_SAFELY)

#undef _DEFINE_ALL_SAFELY
#undef _DEFINE_SAFELY

/*****************************************************************************/
/*                                 _Saturate                                 */
/*****************************************************************************/

/// `<OP>_Saturate(lhs, rhs, &dest) -> bool`
/// Perform `*dest = lhs <OP> rhs` (with <OP> being one of +, -, *) when the
/// given operation is SAFE (i.e. no overflows nor underflows).
/// When the operation is performed, returns TRUE.
/// Otherwise (overflows/underflows), returns FALSE and dest is set to either
/// the MAX/MIN value of the underlying int type, if the operation would have
/// overflows/underflows respectively.
///
/// \pre dest != NULL

#define _DEFINE_SATURATE(OP, T, NAME, MIN, MAX, ...)                           \
  static inline bool atb_##OP##_Saturate_##NAME(T lhs, T rhs, T *const dest) { \
    assert(dest != NULL);                                                      \
                                                                               \
    if (atb_##OP##_IsOverflowing_##NAME(lhs, rhs)) {                           \
      *dest = MAX;                                                             \
      return false;                                                            \
    } else if (atb_##OP##_IsUnderflowing_##NAME(lhs, rhs)) {                   \
      *dest = MIN;                                                             \
      return false;                                                            \
    } else {                                                                   \
      return atb_##OP##_Unsafe_##NAME(lhs, rhs, dest);                         \
    }                                                                          \
  }

#define _DEFINE_ALL_SATURATE(...)    \
  _DEFINE_SATURATE(Add, __VA_ARGS__) \
  _DEFINE_SATURATE(Sub, __VA_ARGS__) \
  _DEFINE_SATURATE(Mul, __VA_ARGS__)

ATB_INTS_X_FOREACH(_DEFINE_ALL_SATURATE)

#undef _DEFINE_ALL_SATURATE
#undef _DEFINE_SATURATE

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
