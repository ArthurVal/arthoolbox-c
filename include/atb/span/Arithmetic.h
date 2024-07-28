#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#include "Core.h" /* ATB_SPAN_DECLARE_NEW */

#include <stdint.h> /* intN_t */

ATB_SPAN_DECLARE_NEW(8, int8_t)
ATB_SPAN_DECLARE_NEW(16, int16_t)
ATB_SPAN_DECLARE_NEW(32, int32_t)
ATB_SPAN_DECLARE_NEW(64, int64_t)

ATB_SPAN_DECLARE_NEW(u8, uint8_t)
ATB_SPAN_DECLARE_NEW(u16, uint16_t)
ATB_SPAN_DECLARE_NEW(u32, uint32_t)
ATB_SPAN_DECLARE_NEW(u64, uint64_t)

ATB_SPAN_DECLARE_NEW(Flt, float)
ATB_SPAN_DECLARE_NEW(Dbl, double)

/**
 *  \brief Shrink a range (reduce its size) from the front OR the
 * back
 *
 *  \param[in] range Range we wish to shrink
 *  \param[in] offset Number of elements we wish to remove
 *  \param[in] front Shrink the range from the front if true, the
 * back otherwise
 *
 *  \warning range is EXPECTED to be defined (non null data ptr)
 *  \note If the offset is bigger than the range size, the range will
 * be shrink by its size instead
 *
 *  \return atb_**Span corresponding to the same range, shrinked
 *          by the requested amount
 */
#define atb_AnyArithmeticSpan_Shrink(range, offset, front)                     \
  _Generic((range), ATB_SPAN_ALL_GENERIC_ASSOCIATION(_Shrink, 8),              \
           ATB_SPAN_ALL_GENERIC_ASSOCIATION(_Shrink, 16),                      \
           ATB_SPAN_ALL_GENERIC_ASSOCIATION(_Shrink, 32),                      \
           ATB_SPAN_ALL_GENERIC_ASSOCIATION(_Shrink, 64),                      \
           ATB_SPAN_ALL_GENERIC_ASSOCIATION(_Shrink, u8),                      \
           ATB_SPAN_ALL_GENERIC_ASSOCIATION(_Shrink, u16),                     \
           ATB_SPAN_ALL_GENERIC_ASSOCIATION(_Shrink, u32),                     \
           ATB_SPAN_ALL_GENERIC_ASSOCIATION(_Shrink, u64),                     \
           ATB_SPAN_ALL_GENERIC_ASSOCIATION(_Shrink, Flt),                     \
           ATB_SPAN_ALL_GENERIC_ASSOCIATION(_Shrink, Dbl))(range, offset,      \
                                                           front)

/**
 *  \brief Slide/translate a range by offset elements
 *
 *  \param[in] range Range we wish to slide
 *  \param[in] offset Number of elements we wish to slide
 *
 *  \warning range is EXPECTED to be defined (non null data ptr)
 *
 *  \return atb_**Span corresponding to the same range, slided
 *          by the requested amount
 */
#define atb_AnyArithmeticSpan_Slide(range, offset)                      \
  _Generic((range), ATB_SPAN_ALL_GENERIC_ASSOCIATION(_Slide, 8),               \
           ATB_SPAN_ALL_GENERIC_ASSOCIATION(_Slide, 16),                       \
           ATB_SPAN_ALL_GENERIC_ASSOCIATION(_Slide, 32),                       \
           ATB_SPAN_ALL_GENERIC_ASSOCIATION(_Slide, 64),                       \
           ATB_SPAN_ALL_GENERIC_ASSOCIATION(_Slide, u8),                       \
           ATB_SPAN_ALL_GENERIC_ASSOCIATION(_Slide, u16),                      \
           ATB_SPAN_ALL_GENERIC_ASSOCIATION(_Slide, u32),                      \
           ATB_SPAN_ALL_GENERIC_ASSOCIATION(_Slide, u64),                      \
           ATB_SPAN_ALL_GENERIC_ASSOCIATION(_Slide, Flt),                      \
           ATB_SPAN_ALL_GENERIC_ASSOCIATION(_Slide, Dbl))(range, offset)

/**
 *  \brief Copy other into range, up to the range size
 *
 *  \param[in] range Range we wish to copy value into
 *  \param[in] other Range we wish to copy from
 *
 *  \warning range and other are EXPECTED to be defined (non null
 * data ptr)
 *
 *  \return atb_**Span corresponding to the input range, slided
 *          by the amount of elements written into it
 */
#define atb_AnyArithmeticSpan_CopyInto(range, other)                           \
  _Generic((range), ATB_SPAN_GENERIC_ASSOCIATION(8, _CopyInto, 8),             \
           ATB_SPAN_GENERIC_ASSOCIATION(16, _CopyInto, 16),                    \
           ATB_SPAN_GENERIC_ASSOCIATION(32, _CopyInto, 32),                    \
           ATB_SPAN_GENERIC_ASSOCIATION(64, _CopyInto, 64),                    \
           ATB_SPAN_GENERIC_ASSOCIATION(u8, _CopyInto, u8),                    \
           ATB_SPAN_GENERIC_ASSOCIATION(u16, _CopyInto, u16),                  \
           ATB_SPAN_GENERIC_ASSOCIATION(u32, _CopyInto, u32),                  \
           ATB_SPAN_GENERIC_ASSOCIATION(u64, _CopyInto, u64),                  \
           ATB_SPAN_GENERIC_ASSOCIATION(Flt, _CopyInto, Flt),                  \
           ATB_SPAN_GENERIC_ASSOCIATION(Dbl, _CopyInto, Dbl))(range, other)

/**
 *  \return True when both range are equals (data), false otherwise.
 *
 *  \note If both ranges have NULL data, it actually compares the
 * size of both ranges, hence 2 NULL ranges with the same sizes, are
 * considered equal.
 *
 *  \param[in] lhs, rhs Ranges we wish to compare
 */
#define atb_AnyArithmeticSpan_IsEqualTo(lhs, rhs)                              \
  _Generic((lhs), ATB_SPAN_GENERIC_ASSOCIATION(Const8, _IsEqualTo, Const8),    \
           ATB_SPAN_GENERIC_ASSOCIATION(Const16, _IsEqualTo, Const16),         \
           ATB_SPAN_GENERIC_ASSOCIATION(Const32, _IsEqualTo, Const32),         \
           ATB_SPAN_GENERIC_ASSOCIATION(Const64, _IsEqualTo, Const64),         \
           ATB_SPAN_GENERIC_ASSOCIATION(Constu8, _IsEqualTo, Constu8),         \
           ATB_SPAN_GENERIC_ASSOCIATION(Constu16, _IsEqualTo, Constu16),       \
           ATB_SPAN_GENERIC_ASSOCIATION(Constu32, _IsEqualTo, Constu32),       \
           ATB_SPAN_GENERIC_ASSOCIATION(Constu64, _IsEqualTo, Constu64),       \
           ATB_SPAN_GENERIC_ASSOCIATION(ConstFlt, _IsEqualTo, ConstFlt),       \
           ATB_SPAN_GENERIC_ASSOCIATION(ConstDbl, _IsEqualTo, ConstDbl))(lhs,  \
                                                                         rhs)

ATB_SPAN_DECLARE_NEW_FROM_ALL(8, 16)
ATB_SPAN_DECLARE_NEW_FROM_ALL(8, 32)
ATB_SPAN_DECLARE_NEW_FROM_ALL(8, 64)

/**
 *  \return atb_8Span from the given input range (non-const only)
 *  \param[in] range Input arithmetic SIGNED range we wish to convert
 */
#define atb_8Span_From(range)                                                  \
  _Generic((range), ATB_SPAN_GENERIC_ASSOCIATION(8, _From8, 8),                \
           ATB_SPAN_GENERIC_ASSOCIATION(8, _From16, 16),                       \
           ATB_SPAN_GENERIC_ASSOCIATION(8, _From32, 32),                       \
           ATB_SPAN_GENERIC_ASSOCIATION(8, _From64, 64))(range)

/**
 *  \return atb_Const8Span from the given input range (const or not)
 *  \param[in] range Input arithmetic SIGNED range we wish to convert
 */
#define atb_Const8Span_From(range)                                             \
  _Generic((range), ATB_SPAN_GENERIC_ASSOCIATION(Const8, _From8, 8),           \
           ATB_SPAN_GENERIC_ASSOCIATION(Const8, _From16, 16),                  \
           ATB_SPAN_GENERIC_ASSOCIATION(Const8, _From32, 32),                  \
           ATB_SPAN_GENERIC_ASSOCIATION(Const8, _From64, 64),                  \
           ATB_SPAN_GENERIC_ASSOCIATION(Const8, _FromConst8, Const8),          \
           ATB_SPAN_GENERIC_ASSOCIATION(Const8, _FromConst16, Const16),        \
           ATB_SPAN_GENERIC_ASSOCIATION(Const8, _FromConst32, Const32),        \
           ATB_SPAN_GENERIC_ASSOCIATION(Const8, _FromConst64, Const64))(range)

ATB_SPAN_DECLARE_NEW_FROM_ALL(16, 8)
ATB_SPAN_DECLARE_NEW_FROM_ALL(16, 32)
ATB_SPAN_DECLARE_NEW_FROM_ALL(16, 64)

/**
 *  \return atb_16Span from the given input range (non-const only)
 *  \param[in] range Input arithmetic SIGNED range we wish to convert
 */
#define atb_16Span_From(range)                                                 \
  _Generic((range), ATB_SPAN_GENERIC_ASSOCIATION(16, _From8, 8),               \
           ATB_SPAN_GENERIC_ASSOCIATION(16, _From16, 16),                      \
           ATB_SPAN_GENERIC_ASSOCIATION(16, _From32, 32),                      \
           ATB_SPAN_GENERIC_ASSOCIATION(16, _From64, 64))(range)

/**
 *  \return atb_Const16Span from the given input range (const or not)
 *  \param[in] range Input arithmetic SIGNED range we wish to convert
 */
#define atb_Const16Span_From(range)                                            \
  _Generic((range), ATB_SPAN_GENERIC_ASSOCIATION(Const16, _From8, 8),          \
           ATB_SPAN_GENERIC_ASSOCIATION(Const16, _From16, 16),                 \
           ATB_SPAN_GENERIC_ASSOCIATION(Const16, _From32, 32),                 \
           ATB_SPAN_GENERIC_ASSOCIATION(Const16, _From64, 64),                 \
           ATB_SPAN_GENERIC_ASSOCIATION(Const16, _FromConst8, Const8),         \
           ATB_SPAN_GENERIC_ASSOCIATION(Const16, _FromConst16, Const16),       \
           ATB_SPAN_GENERIC_ASSOCIATION(Const16, _FromConst32, Const32),       \
           ATB_SPAN_GENERIC_ASSOCIATION(Const16, _FromConst64, Const64))(      \
      range)

ATB_SPAN_DECLARE_NEW_FROM_ALL(32, 8)
ATB_SPAN_DECLARE_NEW_FROM_ALL(32, 16)
ATB_SPAN_DECLARE_NEW_FROM_ALL(32, 64)

/**
 *  \return atb_32Span from the given input range (non-const only)
 *  \param[in] range Input arithmetic SIGNED range we wish to convert
 */
#define atb_32Span_From(range)                                                 \
  _Generic((range), ATB_SPAN_GENERIC_ASSOCIATION(32, _From8, 8),               \
           ATB_SPAN_GENERIC_ASSOCIATION(32, _From16, 16),                      \
           ATB_SPAN_GENERIC_ASSOCIATION(32, _From32, 32),                      \
           ATB_SPAN_GENERIC_ASSOCIATION(32, _From64, 64))(range)

/**
 *  \return atb_Const32Span from the given input range (const or not)
 *  \param[in] range Input arithmetic SIGNED range we wish to convert
 */
#define atb_Const32Span_From(range)                                            \
  _Generic((range), ATB_SPAN_GENERIC_ASSOCIATION(Const32, _From8, 8),          \
           ATB_SPAN_GENERIC_ASSOCIATION(Const32, _From16, 16),                 \
           ATB_SPAN_GENERIC_ASSOCIATION(Const32, _From32, 32),                 \
           ATB_SPAN_GENERIC_ASSOCIATION(Const32, _From64, 64),                 \
           ATB_SPAN_GENERIC_ASSOCIATION(Const32, _FromConst8, Const8),         \
           ATB_SPAN_GENERIC_ASSOCIATION(Const32, _FromConst16, Const16),       \
           ATB_SPAN_GENERIC_ASSOCIATION(Const32, _FromConst32, Const32),       \
           ATB_SPAN_GENERIC_ASSOCIATION(Const32, _FromConst64, Const64))(      \
      range)

ATB_SPAN_DECLARE_NEW_FROM_ALL(64, 8)
ATB_SPAN_DECLARE_NEW_FROM_ALL(64, 16)
ATB_SPAN_DECLARE_NEW_FROM_ALL(64, 32)

/**
 *  \return atb_64Span from the given input range (non-const only)
 *  \param[in] range Input arithmetic SIGNED range we wish to convert
 */
#define atb_64Span_From(range)                                                 \
  _Generic((range), ATB_SPAN_GENERIC_ASSOCIATION(64, _From8, 8),               \
           ATB_SPAN_GENERIC_ASSOCIATION(64, _From16, 16),                      \
           ATB_SPAN_GENERIC_ASSOCIATION(64, _From32, 32),                      \
           ATB_SPAN_GENERIC_ASSOCIATION(64, _From64, 64))(range)

/**
 *  \return atb_Const64Span from the given input range (const or not)
 *  \param[in] range Input arithmetic SIGNED range we wish to convert
 */
#define atb_Const64Span_From(range)                                            \
  _Generic((range), ATB_SPAN_GENERIC_ASSOCIATION(Const64, _From8, 8),          \
           ATB_SPAN_GENERIC_ASSOCIATION(Const64, _From16, 16),                 \
           ATB_SPAN_GENERIC_ASSOCIATION(Const64, _From32, 32),                 \
           ATB_SPAN_GENERIC_ASSOCIATION(Const64, _From64, 64),                 \
           ATB_SPAN_GENERIC_ASSOCIATION(Const64, _FromConst8, Const8),         \
           ATB_SPAN_GENERIC_ASSOCIATION(Const64, _FromConst16, Const16),       \
           ATB_SPAN_GENERIC_ASSOCIATION(Const64, _FromConst32, Const32),       \
           ATB_SPAN_GENERIC_ASSOCIATION(Const64, _FromConst64, Const64))(      \
      range)

ATB_SPAN_DECLARE_NEW_FROM_ALL(u8, u16)
ATB_SPAN_DECLARE_NEW_FROM_ALL(u8, u32)
ATB_SPAN_DECLARE_NEW_FROM_ALL(u8, u64)

/**
 *  \return atb_u8Span from the given input range (non-const only)
 *  \param[in] range Input arithmetic UNSIGNED range we wish to convert
 */
#define atb_u8Span_From(range)                                                 \
  _Generic((range), ATB_SPAN_GENERIC_ASSOCIATION(u8, _Fromu8, u8),             \
           ATB_SPAN_GENERIC_ASSOCIATION(u8, _Fromu16, u16),                    \
           ATB_SPAN_GENERIC_ASSOCIATION(u8, _Fromu32, u32),                    \
           ATB_SPAN_GENERIC_ASSOCIATION(u8, _Fromu64, u64))(range)

/**
 *  \return atb_Constu8Span from the given input range (const or not)
 *  \param[in] range Input arithmetic UNSIGNED range we wish to convert
 */
#define atb_Constu8Span_From(range)                                            \
  _Generic((range), ATB_SPAN_GENERIC_ASSOCIATION(Constu8, _Fromu8, u8),        \
           ATB_SPAN_GENERIC_ASSOCIATION(Constu8, _Fromu16, u16),               \
           ATB_SPAN_GENERIC_ASSOCIATION(Constu8, _Fromu32, u32),               \
           ATB_SPAN_GENERIC_ASSOCIATION(Constu8, _Fromu64, u64),               \
           ATB_SPAN_GENERIC_ASSOCIATION(Constu8, _FromConstu8, Constu8),       \
           ATB_SPAN_GENERIC_ASSOCIATION(Constu8, _FromConstu16, Constu16),     \
           ATB_SPAN_GENERIC_ASSOCIATION(Constu8, _FromConstu32, Constu32),     \
           ATB_SPAN_GENERIC_ASSOCIATION(Constu8, _FromConstu64, Constu64))(    \
      range)

ATB_SPAN_DECLARE_NEW_FROM_ALL(u16, u8)
ATB_SPAN_DECLARE_NEW_FROM_ALL(u16, u32)
ATB_SPAN_DECLARE_NEW_FROM_ALL(u16, u64)

/**
 *  \return atb_u16Span from the given input range (non-const only)
 *  \param[in] range Input arithmetic UNSIGNED range we wish to convert
 */
#define atb_u16Span_From(range)                                                \
  _Generic((range), ATB_SPAN_GENERIC_ASSOCIATION(u16, _Fromu8, u8),            \
           ATB_SPAN_GENERIC_ASSOCIATION(u16, _Fromu16, u16),                   \
           ATB_SPAN_GENERIC_ASSOCIATION(u16, _Fromu32, u32),                   \
           ATB_SPAN_GENERIC_ASSOCIATION(u16, _Fromu64, u64))(range)

/**
 *  \return atb_Constu16Span from the given input range (const or not)
 *  \param[in] range Input arithmetic UNSIGNED range we wish to convert
 */
#define atb_Constu16Span_From(range)                                           \
  _Generic((range), ATB_SPAN_GENERIC_ASSOCIATION(Constu16, _Fromu8, u8),       \
           ATB_SPAN_GENERIC_ASSOCIATION(Constu16, _Fromu16, u16),              \
           ATB_SPAN_GENERIC_ASSOCIATION(Constu16, _Fromu32, u32),              \
           ATB_SPAN_GENERIC_ASSOCIATION(Constu16, _Fromu64, u64),              \
           ATB_SPAN_GENERIC_ASSOCIATION(Constu16, _FromConstu8, Constu8),      \
           ATB_SPAN_GENERIC_ASSOCIATION(Constu16, _FromConstu16, Constu16),    \
           ATB_SPAN_GENERIC_ASSOCIATION(Constu16, _FromConstu32, Constu32),    \
           ATB_SPAN_GENERIC_ASSOCIATION(Constu16, _FromConstu64, Constu64))(   \
      range)

ATB_SPAN_DECLARE_NEW_FROM_ALL(u32, u8)
ATB_SPAN_DECLARE_NEW_FROM_ALL(u32, u16)
ATB_SPAN_DECLARE_NEW_FROM_ALL(u32, u64)

/**
 *  \return atb_u32Span from the given input range (non-const only)
 *  \param[in] range Input arithmetic UNSIGNED range we wish to convert
 */
#define atb_u32Span_From(range)                                                \
  _Generic((range), ATB_SPAN_GENERIC_ASSOCIATION(u32, _Fromu8, u8),            \
           ATB_SPAN_GENERIC_ASSOCIATION(u32, _Fromu16, u16),                   \
           ATB_SPAN_GENERIC_ASSOCIATION(u32, _Fromu32, u32),                   \
           ATB_SPAN_GENERIC_ASSOCIATION(u32, _Fromu64, u64))(range)

/**
 *  \return atb_Constu32Span from the given input range (const or not)
 *  \param[in] range Input arithmetic UNSIGNED range we wish to convert
 */
#define atb_Constu32Span_From(range)                                           \
  _Generic((range), ATB_SPAN_GENERIC_ASSOCIATION(Constu32, _Fromu8, u8),       \
           ATB_SPAN_GENERIC_ASSOCIATION(Constu32, _Fromu16, u16),              \
           ATB_SPAN_GENERIC_ASSOCIATION(Constu32, _Fromu32, u32),              \
           ATB_SPAN_GENERIC_ASSOCIATION(Constu32, _Fromu64, u64),              \
           ATB_SPAN_GENERIC_ASSOCIATION(Constu32, _FromConstu8, Constu8),      \
           ATB_SPAN_GENERIC_ASSOCIATION(Constu32, _FromConstu16, Constu16),    \
           ATB_SPAN_GENERIC_ASSOCIATION(Constu32, _FromConstu32, Constu32),    \
           ATB_SPAN_GENERIC_ASSOCIATION(Constu32, _FromConstu64, Constu64))(   \
      range)

ATB_SPAN_DECLARE_NEW_FROM_ALL(u64, u8)
ATB_SPAN_DECLARE_NEW_FROM_ALL(u64, u16)
ATB_SPAN_DECLARE_NEW_FROM_ALL(u64, u32)

/**
 *  \return atb_u64Span from the given input range (non-const only)
 *  \param[in] range Input arithmetic UNSIGNED range we wish to convert
 */
#define atb_u64Span_From(range)                                                \
  _Generic((range), ATB_SPAN_GENERIC_ASSOCIATION(u64, _Fromu8, u8),            \
           ATB_SPAN_GENERIC_ASSOCIATION(u64, _Fromu16, u16),                   \
           ATB_SPAN_GENERIC_ASSOCIATION(u64, _Fromu32, u32),                   \
           ATB_SPAN_GENERIC_ASSOCIATION(u64, _Fromu64, u64))(range)

/**
 *  \return atb_Constu64Span from the given input range (const or not)
 *  \param[in] range Input arithmetic UNSIGNED range we wish to convert
 */
#define atb_Constu64Span_From(range)                                           \
  _Generic((range), ATB_SPAN_GENERIC_ASSOCIATION(Constu64, _Fromu8, u8),       \
           ATB_SPAN_GENERIC_ASSOCIATION(Constu64, _Fromu16, u16),              \
           ATB_SPAN_GENERIC_ASSOCIATION(Constu64, _Fromu32, u32),              \
           ATB_SPAN_GENERIC_ASSOCIATION(Constu64, _Fromu64, u64),              \
           ATB_SPAN_GENERIC_ASSOCIATION(Constu64, _FromConstu8, Constu8),      \
           ATB_SPAN_GENERIC_ASSOCIATION(Constu64, _FromConstu16, Constu16),    \
           ATB_SPAN_GENERIC_ASSOCIATION(Constu64, _FromConstu32, Constu32),    \
           ATB_SPAN_GENERIC_ASSOCIATION(Constu64, _FromConstu64, Constu64))(   \
      range)

#if defined(__cplusplus)
} /* extern "C" */
#endif
