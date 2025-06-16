#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>

/* i8 **********************************************************************/
static inline bool atb_IsOverflowing_Add_i8(int8_t lhs, int8_t rhs);
static inline bool atb_IsOverflowing_Sub_i8(int8_t lhs, int8_t rhs);
static inline bool atb_IsOverflowing_Mul_i8(int8_t lhs, int8_t rhs);

static inline bool atb_IsUnderflowing_Add_i8(int8_t lhs, int8_t rhs);
static inline bool atb_IsUnderflowing_Sub_i8(int8_t lhs, int8_t rhs);
static inline bool atb_IsUnderflowing_Mul_i8(int8_t lhs, int8_t rhs);

/* i16 *********************************************************************/
static inline bool atb_IsOverflowing_Add_i16(int16_t lhs, int16_t rhs);
static inline bool atb_IsOverflowing_Sub_i16(int16_t lhs, int16_t rhs);
static inline bool atb_IsOverflowing_Mul_i16(int16_t lhs, int16_t rhs);

static inline bool atb_IsUnderflowing_Add_i16(int16_t lhs, int16_t rhs);
static inline bool atb_IsUnderflowing_Sub_i16(int16_t lhs, int16_t rhs);
static inline bool atb_IsUnderflowing_Mul_i16(int16_t lhs, int16_t rhs);

/* i32 *********************************************************************/
static inline bool atb_IsOverflowing_Add_i32(int32_t lhs, int32_t rhs);
static inline bool atb_IsOverflowing_Sub_i32(int32_t lhs, int32_t rhs);
static inline bool atb_IsOverflowing_Mul_i32(int32_t lhs, int32_t rhs);

static inline bool atb_IsUnderflowing_Add_i32(int32_t lhs, int32_t rhs);
static inline bool atb_IsUnderflowing_Sub_i32(int32_t lhs, int32_t rhs);
static inline bool atb_IsUnderflowing_Mul_i32(int32_t lhs, int32_t rhs);

/* i64 *********************************************************************/
static inline bool atb_IsOverflowing_Add_i64(int64_t lhs, int64_t rhs);
static inline bool atb_IsOverflowing_Sub_i64(int64_t lhs, int64_t rhs);
static inline bool atb_IsOverflowing_Mul_i64(int64_t lhs, int64_t rhs);

static inline bool atb_IsUnderflowing_Add_i64(int64_t lhs, int64_t rhs);
static inline bool atb_IsUnderflowing_Sub_i64(int64_t lhs, int64_t rhs);
static inline bool atb_IsUnderflowing_Mul_i64(int64_t lhs, int64_t rhs);

/* imax ********************************************************************/
static inline bool atb_IsOverflowing_Add_imax(intmax_t lhs, intmax_t rhs);
static inline bool atb_IsOverflowing_Sub_imax(intmax_t lhs, intmax_t rhs);
static inline bool atb_IsOverflowing_Mul_imax(intmax_t lhs, intmax_t rhs);

static inline bool atb_IsUnderflowing_Add_imax(intmax_t lhs, intmax_t rhs);
static inline bool atb_IsUnderflowing_Sub_imax(intmax_t lhs, intmax_t rhs);
static inline bool atb_IsUnderflowing_Mul_imax(intmax_t lhs, intmax_t rhs);

/* u8 **********************************************************************/
static inline bool atb_IsOverflowing_Add_u8(uint8_t lhs, uint8_t rhs);
static inline bool atb_IsOverflowing_Sub_u8(uint8_t lhs, uint8_t rhs);
static inline bool atb_IsOverflowing_Mul_u8(uint8_t lhs, uint8_t rhs);

static inline bool atb_IsUnderflowing_Add_u8(uint8_t lhs, uint8_t rhs);
static inline bool atb_IsUnderflowing_Sub_u8(uint8_t lhs, uint8_t rhs);
static inline bool atb_IsUnderflowing_Mul_u8(uint8_t lhs, uint8_t rhs);

/* u16 *********************************************************************/
static inline bool atb_IsOverflowing_Add_u16(uint16_t lhs, uint16_t rhs);
static inline bool atb_IsOverflowing_Sub_u16(uint16_t lhs, uint16_t rhs);
static inline bool atb_IsOverflowing_Mul_u16(uint16_t lhs, uint16_t rhs);

static inline bool atb_IsUnderflowing_Add_u16(uint16_t lhs, uint16_t rhs);
static inline bool atb_IsUnderflowing_Sub_u16(uint16_t lhs, uint16_t rhs);
static inline bool atb_IsUnderflowing_Mul_u16(uint16_t lhs, uint16_t rhs);

/* u32 *********************************************************************/
static inline bool atb_IsOverflowing_Add_u32(uint32_t lhs, uint32_t rhs);
static inline bool atb_IsOverflowing_Sub_u32(uint32_t lhs, uint32_t rhs);
static inline bool atb_IsOverflowing_Mul_u32(uint32_t lhs, uint32_t rhs);

static inline bool atb_IsUnderflowing_Add_u32(uint32_t lhs, uint32_t rhs);
static inline bool atb_IsUnderflowing_Sub_u32(uint32_t lhs, uint32_t rhs);
static inline bool atb_IsUnderflowing_Mul_u32(uint32_t lhs, uint32_t rhs);

/* u64 *********************************************************************/
static inline bool atb_IsOverflowing_Add_u64(uint64_t lhs, uint64_t rhs);
static inline bool atb_IsOverflowing_Sub_u64(uint64_t lhs, uint64_t rhs);
static inline bool atb_IsOverflowing_Mul_u64(uint64_t lhs, uint64_t rhs);

static inline bool atb_IsUnderflowing_Add_u64(uint64_t lhs, uint64_t rhs);
static inline bool atb_IsUnderflowing_Sub_u64(uint64_t lhs, uint64_t rhs);
static inline bool atb_IsUnderflowing_Mul_u64(uint64_t lhs, uint64_t rhs);

/* umax ********************************************************************/
static inline bool atb_IsOverflowing_Add_umax(uintmax_t lhs, uintmax_t rhs);
static inline bool atb_IsOverflowing_Sub_umax(uintmax_t lhs, uintmax_t rhs);
static inline bool atb_IsOverflowing_Mul_umax(uintmax_t lhs, uintmax_t rhs);

static inline bool atb_IsUnderflowing_Add_umax(uintmax_t lhs, uintmax_t rhs);
static inline bool atb_IsUnderflowing_Sub_umax(uintmax_t lhs, uintmax_t rhs);
static inline bool atb_IsUnderflowing_Mul_umax(uintmax_t lhs, uintmax_t rhs);

/***************************************************************************/
/*                            Inline definitions                           */
/***************************************************************************/
/* i8 **********************************************************************/
static inline bool atb_IsOverflowing_Add_i8(int8_t lhs, int8_t rhs) {
  return (rhs > 0) && (lhs > (INT8_MAX - rhs));
}

static inline bool atb_IsOverflowing_Sub_i8(int8_t lhs, int8_t rhs) {
  return (rhs < 0) && (lhs > (INT8_MAX + rhs));
}

static inline bool atb_IsOverflowing_Mul_i8(int8_t lhs, int8_t rhs) {
  if ((lhs > 0) && (rhs > 0)) {
    return lhs > (INT8_MAX / rhs);
  } else if ((lhs < 0) && (rhs < 0)) {
    return lhs < (INT8_MAX / rhs);
  } else {
    return false;
  }
}

static inline bool atb_IsUnderflowing_Add_i8(int8_t lhs, int8_t rhs) {
  return (rhs < 0) && (lhs < (INT8_MIN - rhs));
}

static inline bool atb_IsUnderflowing_Sub_i8(int8_t lhs, int8_t rhs) {
  return (rhs > 0) && (lhs < (INT8_MIN + rhs));
}

static inline bool atb_IsUnderflowing_Mul_i8(int8_t lhs, int8_t rhs) {
  if ((lhs > 0) && (rhs < 0)) {
    return lhs > (INT8_MIN / rhs);
  } else if ((lhs < 0) && (rhs > 0)) {
    return lhs < (INT8_MIN / rhs);
  } else {
    return false;
  }
}

/* i16 *********************************************************************/
static inline bool atb_IsOverflowing_Add_i16(int16_t lhs, int16_t rhs) {
  return (rhs > 0) && (lhs > (INT16_MAX - rhs));
}

static inline bool atb_IsOverflowing_Sub_i16(int16_t lhs, int16_t rhs) {
  return (rhs < 0) && (lhs > (INT16_MAX + rhs));
}

static inline bool atb_IsOverflowing_Mul_i16(int16_t lhs, int16_t rhs) {
  if ((lhs > 0) && (rhs > 0)) {
    return lhs > (INT16_MAX / rhs);
  } else if ((lhs < 0) && (rhs < 0)) {
    return lhs < (INT16_MAX / rhs);
  } else {
    return false;
  }
}

static inline bool atb_IsUnderflowing_Add_i16(int16_t lhs, int16_t rhs) {
  return (rhs < 0) && (lhs < (INT16_MIN - rhs));
}

static inline bool atb_IsUnderflowing_Sub_i16(int16_t lhs, int16_t rhs) {
  return (rhs > 0) && (lhs < (INT16_MIN + rhs));
}

static inline bool atb_IsUnderflowing_Mul_i16(int16_t lhs, int16_t rhs) {
  if ((lhs > 0) && (rhs < 0)) {
    return lhs > (INT16_MIN / rhs);
  } else if ((lhs < 0) && (rhs > 0)) {
    return lhs < (INT16_MIN / rhs);
  } else {
    return false;
  }
}

/* i32 *********************************************************************/
static inline bool atb_IsOverflowing_Add_i32(int32_t lhs, int32_t rhs) {
  return (rhs > 0) && (lhs > (INT32_MAX - rhs));
}

static inline bool atb_IsOverflowing_Sub_i32(int32_t lhs, int32_t rhs) {
  return (rhs < 0) && (lhs > (INT32_MAX + rhs));
}

static inline bool atb_IsOverflowing_Mul_i32(int32_t lhs, int32_t rhs) {
  if ((lhs > 0) && (rhs > 0)) {
    return lhs > (INT32_MAX / rhs);
  } else if ((lhs < 0) && (rhs < 0)) {
    return lhs < (INT32_MAX / rhs);
  } else {
    return false;
  }
}

static inline bool atb_IsUnderflowing_Add_i32(int32_t lhs, int32_t rhs) {
  return (rhs < 0) && (lhs < (INT32_MIN - rhs));
}

static inline bool atb_IsUnderflowing_Sub_i32(int32_t lhs, int32_t rhs) {
  return (rhs > 0) && (lhs < (INT32_MIN + rhs));
}

static inline bool atb_IsUnderflowing_Mul_i32(int32_t lhs, int32_t rhs) {
  if ((lhs > 0) && (rhs < 0)) {
    return lhs > (INT32_MIN / rhs);
  } else if ((lhs < 0) && (rhs > 0)) {
    return lhs < (INT32_MIN / rhs);
  } else {
    return false;
  }
}

/* i64 *********************************************************************/
static inline bool atb_IsOverflowing_Add_i64(int64_t lhs, int64_t rhs) {
  return (rhs > 0) && (lhs > (INT64_MAX - rhs));
}

static inline bool atb_IsOverflowing_Sub_i64(int64_t lhs, int64_t rhs) {
  return (rhs < 0) && (lhs > (INT64_MAX + rhs));
}

static inline bool atb_IsOverflowing_Mul_i64(int64_t lhs, int64_t rhs) {
  if ((lhs > 0) && (rhs > 0)) {
    return lhs > (INT64_MAX / rhs);
  } else if ((lhs < 0) && (rhs < 0)) {
    return lhs < (INT64_MAX / rhs);
  } else {
    return false;
  }
}

static inline bool atb_IsUnderflowing_Add_i64(int64_t lhs, int64_t rhs) {
  return (rhs < 0) && (lhs < (INT64_MIN - rhs));
}

static inline bool atb_IsUnderflowing_Sub_i64(int64_t lhs, int64_t rhs) {
  return (rhs > 0) && (lhs < (INT64_MIN + rhs));
}

static inline bool atb_IsUnderflowing_Mul_i64(int64_t lhs, int64_t rhs) {
  if ((lhs > 0) && (rhs < 0)) {
    return lhs > (INT64_MIN / rhs);
  } else if ((lhs < 0) && (rhs > 0)) {
    return lhs < (INT64_MIN / rhs);
  } else {
    return false;
  }
}

/* imax ********************************************************************/
static inline bool atb_IsOverflowing_Add_imax(intmax_t lhs, intmax_t rhs) {
  return (rhs > 0) && (lhs > (INTMAX_MAX - rhs));
}

static inline bool atb_IsOverflowing_Sub_imax(intmax_t lhs, intmax_t rhs) {
  return (rhs < 0) && (lhs > (INTMAX_MAX + rhs));
}

static inline bool atb_IsOverflowing_Mul_imax(intmax_t lhs, intmax_t rhs) {
  if ((lhs > 0) && (rhs > 0)) {
    return lhs > (INTMAX_MAX / rhs);
  } else if ((lhs < 0) && (rhs < 0)) {
    return lhs < (INTMAX_MAX / rhs);
  } else {
    return false;
  }
}

static inline bool atb_IsUnderflowing_Add_imax(intmax_t lhs, intmax_t rhs) {
  return (rhs < 0) && (lhs < (INTMAX_MIN - rhs));
}

static inline bool atb_IsUnderflowing_Sub_imax(intmax_t lhs, intmax_t rhs) {
  return (rhs > 0) && (lhs < (INTMAX_MIN + rhs));
}

static inline bool atb_IsUnderflowing_Mul_imax(intmax_t lhs, intmax_t rhs) {
  if ((lhs > 0) && (rhs < 0)) {
    return lhs > (INTMAX_MIN / rhs);
  } else if ((lhs < 0) && (rhs > 0)) {
    return lhs < (INTMAX_MIN / rhs);
  } else {
    return false;
  }
}

/* u8 **********************************************************************/
static inline bool atb_IsOverflowing_Add_u8(uint8_t lhs, uint8_t rhs) {
  return (rhs > 0) && (lhs > (UINT8_MAX - rhs));
}

static inline bool atb_IsOverflowing_Sub_u8(uint8_t lhs, uint8_t rhs) {
  (void)lhs;
  (void)rhs;
  return false;
}

static inline bool atb_IsOverflowing_Mul_u8(uint8_t lhs, uint8_t rhs) {
  return (rhs != 0) && (lhs > (UINT8_MAX / rhs));
}

static inline bool atb_IsUnderflowing_Add_u8(uint8_t lhs, uint8_t rhs) {
  (void)lhs;
  (void)rhs;
  return false;
}

static inline bool atb_IsUnderflowing_Sub_u8(uint8_t lhs, uint8_t rhs) {
  return (lhs < rhs);
}

static inline bool atb_IsUnderflowing_Mul_u8(uint8_t lhs, uint8_t rhs) {
  (void)lhs;
  (void)rhs;
  return false;
}

/* u16 *********************************************************************/
static inline bool atb_IsOverflowing_Add_u16(uint16_t lhs, uint16_t rhs) {
  return (rhs > 0) && (lhs > (UINT16_MAX - rhs));
}

static inline bool atb_IsOverflowing_Sub_u16(uint16_t lhs, uint16_t rhs) {
  (void)lhs;
  (void)rhs;
  return false;
}

static inline bool atb_IsOverflowing_Mul_u16(uint16_t lhs, uint16_t rhs) {
  return (rhs != 0) && (lhs > (UINT16_MAX / rhs));
}

static inline bool atb_IsUnderflowing_Add_u16(uint16_t lhs, uint16_t rhs) {
  (void)lhs;
  (void)rhs;
  return false;
}

static inline bool atb_IsUnderflowing_Sub_u16(uint16_t lhs, uint16_t rhs) {
  return (lhs < rhs);
}

static inline bool atb_IsUnderflowing_Mul_u16(uint16_t lhs, uint16_t rhs) {
  (void)lhs;
  (void)rhs;
  return false;
}

/* u32 *********************************************************************/
static inline bool atb_IsOverflowing_Add_u32(uint32_t lhs, uint32_t rhs) {
  return (rhs > 0) && (lhs > (UINT32_MAX - rhs));
}

static inline bool atb_IsOverflowing_Sub_u32(uint32_t lhs, uint32_t rhs) {
  (void)lhs;
  (void)rhs;
  return false;
}

static inline bool atb_IsOverflowing_Mul_u32(uint32_t lhs, uint32_t rhs) {
  return (rhs != 0) && (lhs > (UINT32_MAX / rhs));
}

static inline bool atb_IsUnderflowing_Add_u32(uint32_t lhs, uint32_t rhs) {
  (void)lhs;
  (void)rhs;
  return false;
}

static inline bool atb_IsUnderflowing_Sub_u32(uint32_t lhs, uint32_t rhs) {
  return (lhs < rhs);
}

static inline bool atb_IsUnderflowing_Mul_u32(uint32_t lhs, uint32_t rhs) {
  (void)lhs;
  (void)rhs;
  return false;
}

/* u64 *********************************************************************/
static inline bool atb_IsOverflowing_Add_u64(uint64_t lhs, uint64_t rhs) {
  return (rhs > 0) && (lhs > (UINT64_MAX - rhs));
}

static inline bool atb_IsOverflowing_Sub_u64(uint64_t lhs, uint64_t rhs) {
  (void)lhs;
  (void)rhs;
  return false;
}

static inline bool atb_IsOverflowing_Mul_u64(uint64_t lhs, uint64_t rhs) {
  return (rhs != 0) && (lhs > (UINT64_MAX / rhs));
}

static inline bool atb_IsUnderflowing_Add_u64(uint64_t lhs, uint64_t rhs) {
  (void)lhs;
  (void)rhs;
  return false;
}

static inline bool atb_IsUnderflowing_Sub_u64(uint64_t lhs, uint64_t rhs) {
  return (lhs < rhs);
}

static inline bool atb_IsUnderflowing_Mul_u64(uint64_t lhs, uint64_t rhs) {
  (void)lhs;
  (void)rhs;
  return false;
}

/* umax ********************************************************************/
static inline bool atb_IsOverflowing_Add_umax(uintmax_t lhs, uintmax_t rhs) {
  return (rhs > 0) && (lhs > (UINTMAX_MAX - rhs));
}

static inline bool atb_IsOverflowing_Sub_umax(uintmax_t lhs, uintmax_t rhs) {
  (void)lhs;
  (void)rhs;
  return false;
}

static inline bool atb_IsOverflowing_Mul_umax(uintmax_t lhs, uintmax_t rhs) {
  return (rhs != 0) && (lhs > (UINTMAX_MAX / rhs));
}

static inline bool atb_IsUnderflowing_Add_umax(uintmax_t lhs, uintmax_t rhs) {
  (void)lhs;
  (void)rhs;
  return false;
}

static inline bool atb_IsUnderflowing_Sub_umax(uintmax_t lhs, uintmax_t rhs) {
  return (lhs < rhs);
}

static inline bool atb_IsUnderflowing_Mul_umax(uintmax_t lhs, uintmax_t rhs) {
  (void)lhs;
  (void)rhs;
  return false;
}

#if defined(__cplusplus)
} /* extern "C" */
#endif
