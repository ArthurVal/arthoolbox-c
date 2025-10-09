#pragma once

#include "atb/export.h"
#include "atb/ints.h"
#include "atb/span.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define _ATB_DECLARE_INT_SPANS(T, NAME, ...) \
  ATB_SPAN_VIEW_DECLARE(ATB_PUBLIC extern, atb_Span_##NAME, atb_View_##NAME, T);

ATB_INTS_X_FOREACH(_ATB_DECLARE_INT_SPANS)

#undef _ATB_DECLARE_INT_SPANS

/**@{*/
/**
 * \brief Convert a span from one int type to another of different size
 *
 * \param[in] other Span of the same int type, but with a different size
 *
 * \returns atb_[Span|View]_<INT> Corresponding to the converted span
 */
ATB_PUBLIC extern struct atb_Span_i8 atb_Span_i8_From_i16(
    struct atb_Span_i16 other);
ATB_PUBLIC extern struct atb_Span_i8 atb_Span_i8_From_i32(
    struct atb_Span_i32 other);
ATB_PUBLIC extern struct atb_Span_i8 atb_Span_i8_From_i64(
    struct atb_Span_i64 other);

ATB_PUBLIC extern struct atb_Span_i16 atb_Span_i16_From_i8(
    struct atb_Span_i8 other);
ATB_PUBLIC extern struct atb_Span_i16 atb_Span_i16_From_i32(
    struct atb_Span_i32 other);
ATB_PUBLIC extern struct atb_Span_i16 atb_Span_i16_From_i64(
    struct atb_Span_i64 other);

ATB_PUBLIC extern struct atb_Span_i32 atb_Span_i32_From_i8(
    struct atb_Span_i8 other);
ATB_PUBLIC extern struct atb_Span_i32 atb_Span_i32_From_i16(
    struct atb_Span_i16 other);
ATB_PUBLIC extern struct atb_Span_i32 atb_Span_i32_From_i64(
    struct atb_Span_i64 other);

ATB_PUBLIC extern struct atb_Span_i64 atb_Span_i64_From_i8(
    struct atb_Span_i8 other);
ATB_PUBLIC extern struct atb_Span_i64 atb_Span_i64_From_i16(
    struct atb_Span_i16 other);
ATB_PUBLIC extern struct atb_Span_i64 atb_Span_i64_From_i32(
    struct atb_Span_i32 other);

ATB_PUBLIC extern struct atb_Span_u8 atb_Span_u8_From_u16(
    struct atb_Span_u16 other);
ATB_PUBLIC extern struct atb_Span_u8 atb_Span_u8_From_u32(
    struct atb_Span_u32 other);
ATB_PUBLIC extern struct atb_Span_u8 atb_Span_u8_From_u64(
    struct atb_Span_u64 other);

ATB_PUBLIC extern struct atb_Span_u16 atb_Span_u16_From_u8(
    struct atb_Span_u8 other);
ATB_PUBLIC extern struct atb_Span_u16 atb_Span_u16_From_u32(
    struct atb_Span_u32 other);
ATB_PUBLIC extern struct atb_Span_u16 atb_Span_u16_From_u64(
    struct atb_Span_u64 other);

ATB_PUBLIC extern struct atb_Span_u32 atb_Span_u32_From_u8(
    struct atb_Span_u8 other);
ATB_PUBLIC extern struct atb_Span_u32 atb_Span_u32_From_u16(
    struct atb_Span_u16 other);
ATB_PUBLIC extern struct atb_Span_u32 atb_Span_u32_From_u64(
    struct atb_Span_u64 other);

ATB_PUBLIC extern struct atb_Span_u64 atb_Span_u64_From_u8(
    struct atb_Span_u8 other);
ATB_PUBLIC extern struct atb_Span_u64 atb_Span_u64_From_u16(
    struct atb_Span_u16 other);
ATB_PUBLIC extern struct atb_Span_u64 atb_Span_u64_From_u32(
    struct atb_Span_u32 other);

ATB_PUBLIC extern struct atb_View_i8 atb_View_i8_From_i16(
    struct atb_View_i16 other);
ATB_PUBLIC extern struct atb_View_i8 atb_View_i8_From_i32(
    struct atb_View_i32 other);
ATB_PUBLIC extern struct atb_View_i8 atb_View_i8_From_i64(
    struct atb_View_i64 other);

ATB_PUBLIC extern struct atb_View_i16 atb_View_i16_From_i8(
    struct atb_View_i8 other);
ATB_PUBLIC extern struct atb_View_i16 atb_View_i16_From_i32(
    struct atb_View_i32 other);
ATB_PUBLIC extern struct atb_View_i16 atb_View_i16_From_i64(
    struct atb_View_i64 other);

ATB_PUBLIC extern struct atb_View_i32 atb_View_i32_From_i8(
    struct atb_View_i8 other);
ATB_PUBLIC extern struct atb_View_i32 atb_View_i32_From_i16(
    struct atb_View_i16 other);
ATB_PUBLIC extern struct atb_View_i32 atb_View_i32_From_i64(
    struct atb_View_i64 other);

ATB_PUBLIC extern struct atb_View_i64 atb_View_i64_From_i8(
    struct atb_View_i8 other);
ATB_PUBLIC extern struct atb_View_i64 atb_View_i64_From_i16(
    struct atb_View_i16 other);
ATB_PUBLIC extern struct atb_View_i64 atb_View_i64_From_i32(
    struct atb_View_i32 other);

ATB_PUBLIC extern struct atb_View_u8 atb_View_u8_From_u16(
    struct atb_View_u16 other);
ATB_PUBLIC extern struct atb_View_u8 atb_View_u8_From_u32(
    struct atb_View_u32 other);
ATB_PUBLIC extern struct atb_View_u8 atb_View_u8_From_u64(
    struct atb_View_u64 other);

ATB_PUBLIC extern struct atb_View_u16 atb_View_u16_From_u8(
    struct atb_View_u8 other);
ATB_PUBLIC extern struct atb_View_u16 atb_View_u16_From_u32(
    struct atb_View_u32 other);
ATB_PUBLIC extern struct atb_View_u16 atb_View_u16_From_u64(
    struct atb_View_u64 other);

ATB_PUBLIC extern struct atb_View_u32 atb_View_u32_From_u8(
    struct atb_View_u8 other);
ATB_PUBLIC extern struct atb_View_u32 atb_View_u32_From_u16(
    struct atb_View_u16 other);
ATB_PUBLIC extern struct atb_View_u32 atb_View_u32_From_u64(
    struct atb_View_u64 other);

ATB_PUBLIC extern struct atb_View_u64 atb_View_u64_From_u8(
    struct atb_View_u8 other);
ATB_PUBLIC extern struct atb_View_u64 atb_View_u64_From_u16(
    struct atb_View_u16 other);
ATB_PUBLIC extern struct atb_View_u64 atb_View_u64_From_u32(
    struct atb_View_u32 other);
/**@}*/

#if defined(__cplusplus)
}
#endif
