#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#include <assert.h>
#include <stdbool.h> /* bool */
#include <stddef.h>  /* size_t */
#include <stdint.h>  /* uint8_t */
#include <string.h>  /* strlen, memcmp */

#include "atb/MacroUtils.h"  /* ATB_COMPOUND_LITERAL */
#include "atb/StaticArray.h" /* GetSize */

/// This declare a CONVERSION function to swap from one range to an other
/// It is suppose to be use for INTEGER TYPES only or the ToConst conversions
#define _ATB_RANGEVIEW_DECLARE_CONVERSION(to, from)                            \
  static inline struct atb_##to##RangeView atb_##to##RangeView_From##from(     \
      struct atb_##from##RangeView range) {                                    \
                                                                               \
    struct atb_##to##RangeView out;                                            \
    out.data = (__typeof(out.data))range.data;                                 \
    out.size = (range.size * (sizeof(*range.data) / sizeof(*out.data)));       \
    return out;                                                                \
  }

#define _ATB_RANGEVIEW_DECLARE_ALL_CONVERSION(to, from)                        \
  _ATB_RANGEVIEW_DECLARE_CONVERSION(to, from)                                  \
  _ATB_RANGEVIEW_DECLARE_CONVERSION(Const##to, from)                           \
  _ATB_RANGEVIEW_DECLARE_CONVERSION(Const##to, Const##from)

#define ATB_RANGEVIEW_DECLARE_NEW(prefix, type)                                \
  struct atb_##prefix##RangeView {                                             \
    type *data;                                                                \
    size_t size;                                                               \
  };                                                                           \
                                                                               \
  struct atb_Const##prefix##RangeView {                                        \
    const type *data;                                                          \
    size_t size;                                                               \
  };                                                                           \
                                                                               \
  static inline bool atb_Const##prefix##RangeView_IsOverlapping(               \
      struct atb_Const##prefix##RangeView lhs,                                 \
      struct atb_Const##prefix##RangeView rhs) {                               \
                                                                               \
    if (atb_AnyRangeView_Begin(lhs) == atb_AnyRangeView_Begin(rhs)) {          \
      return (lhs.size != 0) && (rhs.size != 0);                               \
    } else if (atb_AnyRangeView_Begin(lhs) < atb_AnyRangeView_Begin(rhs)) {    \
      return (atb_AnyRangeView_End(lhs) <= atb_AnyRangeView_Begin(rhs));       \
    } else {                                                                   \
      return (atb_AnyRangeView_End(rhs) <= atb_AnyRangeView_Begin(lhs));       \
    }                                                                          \
  }                                                                            \
                                                                               \
  static inline struct atb_Const##prefix##RangeView                            \
      atb_Const##prefix##RangeView_Shrink(                                     \
          struct atb_Const##prefix##RangeView range, size_t offset,            \
          bool front) {                                                        \
    assert(range.data != NULL);                                                \
    assert(range.size >= offset);                                              \
                                                                               \
    if (front) {                                                               \
      range.data += offset;                                                    \
    }                                                                          \
                                                                               \
    range.size -= offset;                                                      \
    return range;                                                              \
  }                                                                            \
                                                                               \
  static inline struct atb_##prefix##RangeView atb_##prefix##RangeView_Shrink( \
      struct atb_##prefix##RangeView range, size_t offset, bool front) {       \
    assert(range.data != NULL);                                                \
                                                                               \
    offset = (range.size < offset ? range.size : offset);                      \
                                                                               \
    if (front) {                                                               \
      range.data += offset;                                                    \
    }                                                                          \
                                                                               \
    range.size -= offset;                                                      \
    return range;                                                              \
  }                                                                            \
                                                                               \
  static inline struct atb_Const##prefix##RangeView                            \
      atb_Const##prefix##RangeView_Shift(                                      \
          struct atb_Const##prefix##RangeView range, size_t offset,            \
          bool right) {                                                        \
    assert(range.data != NULL);                                                \
                                                                               \
    if (right) {                                                               \
      range.data += offset;                                                    \
    } else {                                                                   \
      range.data -= offset;                                                    \
    }                                                                          \
                                                                               \
    return range;                                                              \
  }                                                                            \
                                                                               \
  static inline struct atb_##prefix##RangeView atb_##prefix##RangeView_Shift(  \
      struct atb_##prefix##RangeView range, size_t offset, bool right) {       \
    assert(range.data != NULL);                                                \
                                                                               \
    if (right) {                                                               \
      range.data += offset;                                                    \
    } else {                                                                   \
      range.data -= offset;                                                    \
    }                                                                          \
                                                                               \
    return range;                                                              \
  }                                                                            \
                                                                               \
  _ATB_RANGEVIEW_DECLARE_CONVERSION(Const##prefix, prefix)                     \
                                                                               \
  static inline struct atb_##prefix##RangeView                                 \
      atb_##prefix##RangeView_CopyInto(                                        \
          struct atb_##prefix##RangeView range,                                \
          struct atb_Const##prefix##RangeView other) {                         \
                                                                               \
    assert(range.data != NULL);                                                \
    assert(other.data != NULL);                                                \
                                                                               \
    if (range.size < other.size)                                               \
      other.size = range.size;                                                 \
                                                                               \
    if (atb_Const##prefix##RangeView_IsOverlapping(                            \
            atb_Const##prefix##RangeView_From##prefix(range), other)) {        \
      memmove(range.data, other.data, (other.size * sizeof(type)));            \
    } else {                                                                   \
      memcpy(range.data, other.data, (other.size * sizeof(type)));             \
    }                                                                          \
                                                                               \
    atb_##prefix##RangeView_Shrink(range, other.size, true);                   \
    return range;                                                              \
  }                                                                            \
                                                                               \
  static inline bool atb_Const##prefix##RangeView_IsEqualTo(                   \
      struct atb_Const##prefix##RangeView lhs,                                 \
      struct atb_Const##prefix##RangeView rhs) {                               \
    if (lhs.size != rhs.size) {                                                \
      return false;                                                            \
    }                                                                          \
                                                                               \
    if ((lhs.data != NULL) && (rhs.data != NULL)) {                            \
      return (memcmp(lhs.data, rhs.data, (rhs.size * sizeof(type))) == 0);     \
    } else {                                                                   \
      return lhs.data == rhs.data;                                             \
    }                                                                          \
  }

/**
 * Custom macro helper to declare a single generic association for one
 * RangeView, used inside _Generic()
 */
#define ATB_RANGEVIEW_GENERIC_ASSOCIATION(fn_prefix, fn_postfix, arg_type)     \
  struct atb_##arg_type##RangeView : atb_##fn_prefix##RangeView##fn_postfix

/**
 * Custom macro helper to declare ALL generic association of generated
 * RangeViews (NRangeView, ConstNRangeView), used inside _Generic() (all
 * pointing to the same function)
 */
#define ATB_RANGEVIEW_ALL_GENERIC_ASSOCIATION(fn_postfix, arg_type)            \
  ATB_RANGEVIEW_GENERIC_ASSOCIATION(arg_type, fn_postfix, arg_type),           \
      ATB_RANGEVIEW_GENERIC_ASSOCIATION(Const##arg_type, fn_postfix,           \
                                        Const##arg_type)

/**
 *  \brief Statically initialize a atb_**RangeView data structure
 */
#define atb_AnyRangeView_INITIALIZER()                                         \
  { NULL, 0 }

/**
 *  \brief Printf format string helper
 */
#define atb_AnyRangeView_Fmt "{.data = %p, .size = %zu (%zu bytes)}"

/**
 *  \brief Printf argument helper
 *
 *  \warning DO NOT use any side effect statement as range input
 */
#define atb_AnyRangeView_FmtArg(range)                                         \
  ((void *)((range).data)), (range).size,                                      \
      ((range).size * sizeof(*((range).data)))

/**
 *  \brief Map a STATIC array (size known at compile time) into the range
 *
 *  \param[inout] range Range used to map the array
 *  \param[in] array Compile time array
 *
 *  \warning DO NOT use any side effect statement as range input
 */
#define atb_AnyRangeView_MapStaticArray(range, array)                          \
  do {                                                                         \
    (range).data = array;                                                      \
    (range).size = atb_StaticArray_GetSize(array);                             \
  } while (0)

/**
 *  \return atb_**RangeView Maping a STATIC array (size known at compile time)
 *
 *  \param[in] prefix The prefix name of the range view
 *  \param[in] array Compile time array
 */
#define atb_RangeView_FromStaticArray(prefix, array)                           \
  ATB_COMPOUND_LITERAL(atb_##prefix##RangeView) {                              \
    /* .data = */ array, /* .size = */ atb_StaticArray_GetSize(array),         \
  }

/**
 *  \return atb_**ConstRangeNView Maping a STATIC array (size known at compile
 *                                time)
 *
 *  \param[in] prefix The prefix name of the range view
 *  \param[in] array Compile time array
 */
#define atb_ConstRangeView_FromStaticArray(prefix, array)                      \
  ATB_COMPOUND_LITERAL(atb_##prefix##ConstRangeView) {                         \
    /* .data = */ array, /* .size = */ atb_StaticArray_GetSize(array),         \
  }

/**
 *  \return Pointer to the begin of the range
 */
#define atb_AnyRangeView_Begin(range) (range).data

/**
 *  \return Pointer to the end of the range (one past the last valid element)
 *
 *  \warning DO NOT use any side effect statement as range input
 */
#define atb_AnyRangeView_End(range)                                            \
  (atb_AnyRangeView_Begin(range) + (range).size)

/**
 *  \brief Loop helper used to iterate over the range, using elem as iterator
 *
 *  \warning DO NOT use any side effect statement as range input
 */
#define atb_AnyRangeView_ForEach(elem, range)                                  \
  for ((elem) = atb_AnyRangeView_Begin(range);                                 \
       (elem) != atb_AnyRangeView_End(range); (elem) += 1)

/**
 *  \return Pointer to the rbegin of the range. (Last valid element)
 *
 *  \warning DO NOT use any side effect statement as range input
 */
#define atb_AnyRangeView_RBegin(range) atb_AnyRangeView_End(range) - 1

/**
 *  \return Pointer to the rend of the range (one before the first valid
 * element)
 */
#define atb_AnyRangeView_REnd(range) atb_AnyRangeView_Begin(range) - 1

/**
 *  \brief Loop helper used to REVERSE iterate over the range, using elem as
 * iterator
 *
 *  \warning DO NOT use any side effect statement as range input
 */
#define atb_AnyRangeView_RForEach(elem, range)                                 \
  for (elem = atb_AnyRangeView_RBegin(range);                                  \
       elem != atb_AnyRangeView_REnd(range); elem -= 1)

ATB_RANGEVIEW_DECLARE_NEW(8, int8_t)
ATB_RANGEVIEW_DECLARE_NEW(16, int16_t)
ATB_RANGEVIEW_DECLARE_NEW(32, int32_t)
ATB_RANGEVIEW_DECLARE_NEW(64, int64_t)

ATB_RANGEVIEW_DECLARE_NEW(u8, uint8_t)
ATB_RANGEVIEW_DECLARE_NEW(u16, uint16_t)
ATB_RANGEVIEW_DECLARE_NEW(u32, uint32_t)
ATB_RANGEVIEW_DECLARE_NEW(u64, uint64_t)

ATB_RANGEVIEW_DECLARE_NEW(Flt, float)
ATB_RANGEVIEW_DECLARE_NEW(Dbl, double)

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
 *  \return atb_**RangeView corresponding to the same range, shrinked
 * by the requested amount
 */
#define atb_AnyArithmeticRangeView_Shrink(range, offset, front)                \
  _Generic((range), ATB_RANGEVIEW_ALL_GENERIC_ASSOCIATION(_Shrink, 8),         \
           ATB_RANGEVIEW_ALL_GENERIC_ASSOCIATION(_Shrink, 16),                 \
           ATB_RANGEVIEW_ALL_GENERIC_ASSOCIATION(_Shrink, 32),                 \
           ATB_RANGEVIEW_ALL_GENERIC_ASSOCIATION(_Shrink, 64),                 \
           ATB_RANGEVIEW_ALL_GENERIC_ASSOCIATION(_Shrink, u8),                 \
           ATB_RANGEVIEW_ALL_GENERIC_ASSOCIATION(_Shrink, u16),                \
           ATB_RANGEVIEW_ALL_GENERIC_ASSOCIATION(_Shrink, u32),                \
           ATB_RANGEVIEW_ALL_GENERIC_ASSOCIATION(_Shrink, u64),                \
           ATB_RANGEVIEW_ALL_GENERIC_ASSOCIATION(_Shrink, Flt),                \
           ATB_RANGEVIEW_ALL_GENERIC_ASSOCIATION(_Shrink, Dbl))(range, offset, \
                                                                front)

/**
 *  \brief Shift/translate a range to the right or left
 *
 *  \param[in] range Range we wish to shift
 *  \param[in] offset Number of elements we wish to shitf
 *  \param[in] right Shift the range to the right if true, the left
 * otherwise
 *
 *  \warning range is EXPECTED to be defined (non null data ptr)
 *
 *  \return atb_**RangeView corresponding to the same range, shifted
 * by the requested amount
 */
#define atb_AnyArithmeticRangeView_Shift(range, offset, right)                 \
  _Generic((range), ATB_RANGEVIEW_ALL_GENERIC_ASSOCIATION(_Shift, 8),          \
           ATB_RANGEVIEW_ALL_GENERIC_ASSOCIATION(_Shift, 16),                  \
           ATB_RANGEVIEW_ALL_GENERIC_ASSOCIATION(_Shift, 32),                  \
           ATB_RANGEVIEW_ALL_GENERIC_ASSOCIATION(_Shift, 64),                  \
           ATB_RANGEVIEW_ALL_GENERIC_ASSOCIATION(_Shift, u8),                  \
           ATB_RANGEVIEW_ALL_GENERIC_ASSOCIATION(_Shift, u16),                 \
           ATB_RANGEVIEW_ALL_GENERIC_ASSOCIATION(_Shift, u32),                 \
           ATB_RANGEVIEW_ALL_GENERIC_ASSOCIATION(_Shift, u64),                 \
           ATB_RANGEVIEW_ALL_GENERIC_ASSOCIATION(_Shift, Flt),                 \
           ATB_RANGEVIEW_ALL_GENERIC_ASSOCIATION(_Shift, Dbl))(range, offset,  \
                                                               right)

/**
 *  \brief Copy other into range, up to the range size
 *
 *  \param[in] range Range we wish to copy value into
 *  \param[in] other Range we wish to copy from
 *
 *  \warning range and other are EXPECTED to be defined (non null
 * data ptr)
 *
 *  \return atb_**RangeView corresponding to the input range, shifted
 * by the amount of elements written into it
 */
#define atb_AnyArithmeticRangeView_CopyInto(range, other)                      \
  _Generic((range), ATB_RANGEVIEW_GENERIC_ASSOCIATION(8, _CopyInto, 8),        \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(16, _CopyInto, 16),               \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(32, _CopyInto, 32),               \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(64, _CopyInto, 64),               \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(u8, _CopyInto, u8),               \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(u16, _CopyInto, u16),             \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(u32, _CopyInto, u32),             \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(u64, _CopyInto, u64),             \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Flt, _CopyInto, Flt),             \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Dbl, _CopyInto, Dbl))(range,      \
                                                                   other)

/**
 *  \return True when both range are equals (data), false otherwise.
 *
 *  \note If both ranges have NULL data, it actually compares the
 * size of both ranges, hence 2 NULL ranges with the same sizes, are
 * considered equal.
 *
 *  \param[in] lhs, rhs Ranges we wish to compare
 */
#define atb_AnyArithmeticRangeView_IsEqualTo(lhs, rhs)                         \
  _Generic((lhs),                                                              \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Const8, _IsEqualTo, Const8),      \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Const16, _IsEqualTo, Const16),    \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Const32, _IsEqualTo, Const32),    \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Const64, _IsEqualTo, Const64),    \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Constu8, _IsEqualTo, Constu8),    \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Constu16, _IsEqualTo, Constu16),  \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Constu32, _IsEqualTo, Constu32),  \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Constu64, _IsEqualTo, Constu64),  \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(ConstFlt, _IsEqualTo, ConstFlt),  \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(ConstDbl, _IsEqualTo, ConstDbl))( \
      lhs, rhs)

_ATB_RANGEVIEW_DECLARE_CONVERSION(8, 8)
_ATB_RANGEVIEW_DECLARE_CONVERSION(Const8, Const8)
_ATB_RANGEVIEW_DECLARE_ALL_CONVERSION(8, 16)
_ATB_RANGEVIEW_DECLARE_ALL_CONVERSION(8, 32)
_ATB_RANGEVIEW_DECLARE_ALL_CONVERSION(8, 64)

/**
 *  \return atb_8RangeView from the given input range (non-const only)
 *  \param[in] range Input arithmetic SIGNED range we wish to convert
 */
#define atb_8RangeView_From(range)                                             \
  _Generic((range), ATB_RANGEVIEW_GENERIC_ASSOCIATION(8, _From8, 8),           \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(8, _From16, 16),                  \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(8, _From32, 32),                  \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(8, _From64, 64))(range)

/**
 *  \return atb_Const8RangeView from the given input range (const or not)
 *  \param[in] range Input arithmetic SIGNED range we wish to convert
 */
#define atb_Const8RangeView_From(range)                                        \
  _Generic((range), ATB_RANGEVIEW_GENERIC_ASSOCIATION(Const8, _From8, 8),      \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Const8, _From16, 16),             \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Const8, _From32, 32),             \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Const8, _From64, 64),             \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Const8, _FromConst8, Const8),     \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Const8, _FromConst16, Const16),   \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Const8, _FromConst32, Const32),   \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Const8, _FromConst64, Const64))(  \
      range)

_ATB_RANGEVIEW_DECLARE_ALL_CONVERSION(16, 8)
_ATB_RANGEVIEW_DECLARE_CONVERSION(16, 16)
_ATB_RANGEVIEW_DECLARE_CONVERSION(Const16, Const16)
_ATB_RANGEVIEW_DECLARE_ALL_CONVERSION(16, 32)
_ATB_RANGEVIEW_DECLARE_ALL_CONVERSION(16, 64)

/**
 *  \return atb_16RangeView from the given input range (non-const only)
 *  \param[in] range Input arithmetic SIGNED range we wish to convert
 */
#define atb_16RangeView_From(range)                                            \
  _Generic((range), ATB_RANGEVIEW_GENERIC_ASSOCIATION(16, _From8, 8),          \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(16, _From16, 16),                 \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(16, _From32, 32),                 \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(16, _From64, 64))(range)

/**
 *  \return atb_Const16RangeView from the given input range (const or not)
 *  \param[in] range Input arithmetic SIGNED range we wish to convert
 */
#define atb_Const16RangeView_From(range)                                       \
  _Generic((range), ATB_RANGEVIEW_GENERIC_ASSOCIATION(Const16, _From8, 8),     \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Const16, _From16, 16),            \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Const16, _From32, 32),            \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Const16, _From64, 64),            \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Const16, _FromConst8, Const8),    \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Const16, _FromConst16, Const16),  \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Const16, _FromConst32, Const32),  \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Const16, _FromConst64, Const64))( \
      range)

_ATB_RANGEVIEW_DECLARE_ALL_CONVERSION(32, 8)
_ATB_RANGEVIEW_DECLARE_ALL_CONVERSION(32, 16)
_ATB_RANGEVIEW_DECLARE_CONVERSION(32, 32)
_ATB_RANGEVIEW_DECLARE_CONVERSION(Const32, Const32)
_ATB_RANGEVIEW_DECLARE_ALL_CONVERSION(32, 64)

/**
 *  \return atb_32RangeView from the given input range (non-const only)
 *  \param[in] range Input arithmetic SIGNED range we wish to convert
 */
#define atb_32RangeView_From(range)                                            \
  _Generic((range), ATB_RANGEVIEW_GENERIC_ASSOCIATION(32, _From8, 8),          \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(32, _From16, 16),                 \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(32, _From32, 32),                 \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(32, _From64, 64))(range)

/**
 *  \return atb_Const32RangeView from the given input range (const or not)
 *  \param[in] range Input arithmetic SIGNED range we wish to convert
 */
#define atb_Const32RangeView_From(range)                                       \
  _Generic((range), ATB_RANGEVIEW_GENERIC_ASSOCIATION(Const32, _From8, 8),     \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Const32, _From16, 16),            \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Const32, _From32, 32),            \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Const32, _From64, 64),            \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Const32, _FromConst8, Const8),    \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Const32, _FromConst16, Const16),  \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Const32, _FromConst32, Const32),  \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Const32, _FromConst64, Const64))( \
      range)

_ATB_RANGEVIEW_DECLARE_ALL_CONVERSION(64, 8)
_ATB_RANGEVIEW_DECLARE_ALL_CONVERSION(64, 16)
_ATB_RANGEVIEW_DECLARE_ALL_CONVERSION(64, 32)
_ATB_RANGEVIEW_DECLARE_CONVERSION(64, 64)
_ATB_RANGEVIEW_DECLARE_CONVERSION(Const64, Const64)

/**
 *  \return atb_64RangeView from the given input range (non-const only)
 *  \param[in] range Input arithmetic SIGNED range we wish to convert
 */
#define atb_64RangeView_From(range)                                            \
  _Generic((range), ATB_RANGEVIEW_GENERIC_ASSOCIATION(64, _From8, 8),          \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(64, _From16, 16),                 \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(64, _From32, 32),                 \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(64, _From64, 64))(range)

/**
 *  \return atb_Const64RangeView from the given input range (const or not)
 *  \param[in] range Input arithmetic SIGNED range we wish to convert
 */
#define atb_Const64RangeView_From(range)                                       \
  _Generic((range), ATB_RANGEVIEW_GENERIC_ASSOCIATION(Const64, _From8, 8),     \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Const64, _From16, 16),            \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Const64, _From32, 32),            \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Const64, _From64, 64),            \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Const64, _FromConst8, Const8),    \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Const64, _FromConst16, Const16),  \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Const64, _FromConst32, Const32),  \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(Const64, _FromConst64, Const64))( \
      range)

_ATB_RANGEVIEW_DECLARE_CONVERSION(u8, u8)
_ATB_RANGEVIEW_DECLARE_CONVERSION(Constu8, Constu8)
_ATB_RANGEVIEW_DECLARE_ALL_CONVERSION(u8, u16)
_ATB_RANGEVIEW_DECLARE_ALL_CONVERSION(u8, u32)
_ATB_RANGEVIEW_DECLARE_ALL_CONVERSION(u8, u64)

/**
 *  \return atb_u8RangeView from the given input range (non-const only)
 *  \param[in] range Input arithmetic UNSIGNED range we wish to convert
 */
#define atb_u8RangeView_From(range)                                            \
  _Generic((range), ATB_RANGEVIEW_GENERIC_ASSOCIATION(u8, _Fromu8, u8),        \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(u8, _Fromu16, u16),               \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(u8, _Fromu32, u32),               \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(u8, _Fromu64, u64))(range)

/**
 *  \return atb_Constu8RangeView from the given input range (const or not)
 *  \param[in] range Input arithmetic UNSIGNED range we wish to convert
 */
#define atb_Constu8RangeView_From(range)                                       \
  _Generic(                                                                    \
      (range), ATB_RANGEVIEW_GENERIC_ASSOCIATION(Constu8, _Fromu8, u8),        \
      ATB_RANGEVIEW_GENERIC_ASSOCIATION(Constu8, _Fromu16, u16),               \
      ATB_RANGEVIEW_GENERIC_ASSOCIATION(Constu8, _Fromu32, u32),               \
      ATB_RANGEVIEW_GENERIC_ASSOCIATION(Constu8, _Fromu64, u64),               \
      ATB_RANGEVIEW_GENERIC_ASSOCIATION(Constu8, _FromConstu8, Constu8),       \
      ATB_RANGEVIEW_GENERIC_ASSOCIATION(Constu8, _FromConstu16, Constu16),     \
      ATB_RANGEVIEW_GENERIC_ASSOCIATION(Constu8, _FromConstu32, Constu32),     \
      ATB_RANGEVIEW_GENERIC_ASSOCIATION(Constu8, _FromConstu64, Constu64))(    \
      range)

_ATB_RANGEVIEW_DECLARE_ALL_CONVERSION(u16, u8)
_ATB_RANGEVIEW_DECLARE_CONVERSION(u16, u16)
_ATB_RANGEVIEW_DECLARE_CONVERSION(Constu16, Constu16)
_ATB_RANGEVIEW_DECLARE_ALL_CONVERSION(u16, u32)
_ATB_RANGEVIEW_DECLARE_ALL_CONVERSION(u16, u64)

/**
 *  \return atb_u16RangeView from the given input range (non-const only)
 *  \param[in] range Input arithmetic UNSIGNED range we wish to convert
 */
#define atb_u16RangeView_From(range)                                           \
  _Generic((range), ATB_RANGEVIEW_GENERIC_ASSOCIATION(u16, _Fromu8, u8),       \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(u16, _Fromu16, u16),              \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(u16, _Fromu32, u32),              \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(u16, _Fromu64, u64))(range)

/**
 *  \return atb_Constu16RangeView from the given input range (const or not)
 *  \param[in] range Input arithmetic UNSIGNED range we wish to convert
 */
#define atb_Constu16RangeView_From(range)                                      \
  _Generic(                                                                    \
      (range), ATB_RANGEVIEW_GENERIC_ASSOCIATION(Constu16, _Fromu8, u8),       \
      ATB_RANGEVIEW_GENERIC_ASSOCIATION(Constu16, _Fromu16, u16),              \
      ATB_RANGEVIEW_GENERIC_ASSOCIATION(Constu16, _Fromu32, u32),              \
      ATB_RANGEVIEW_GENERIC_ASSOCIATION(Constu16, _Fromu64, u64),              \
      ATB_RANGEVIEW_GENERIC_ASSOCIATION(Constu16, _FromConstu8, Constu8),      \
      ATB_RANGEVIEW_GENERIC_ASSOCIATION(Constu16, _FromConstu16, Constu16),    \
      ATB_RANGEVIEW_GENERIC_ASSOCIATION(Constu16, _FromConstu32, Constu32),    \
      ATB_RANGEVIEW_GENERIC_ASSOCIATION(Constu16, _FromConstu64, Constu64))(   \
      range)

_ATB_RANGEVIEW_DECLARE_ALL_CONVERSION(u32, u8)
_ATB_RANGEVIEW_DECLARE_ALL_CONVERSION(u32, u16)
_ATB_RANGEVIEW_DECLARE_CONVERSION(u32, u32)
_ATB_RANGEVIEW_DECLARE_CONVERSION(Constu32, Constu32)
_ATB_RANGEVIEW_DECLARE_ALL_CONVERSION(u32, u64)

/**
 *  \return atb_u32RangeView from the given input range (non-const only)
 *  \param[in] range Input arithmetic UNSIGNED range we wish to convert
 */
#define atb_u32RangeView_From(range)                                           \
  _Generic((range), ATB_RANGEVIEW_GENERIC_ASSOCIATION(u32, _Fromu8, u8),       \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(u32, _Fromu16, u16),              \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(u32, _Fromu32, u32),              \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(u32, _Fromu64, u64))(range)

/**
 *  \return atb_Constu32RangeView from the given input range (const or not)
 *  \param[in] range Input arithmetic UNSIGNED range we wish to convert
 */
#define atb_Constu32RangeView_From(range)                                      \
  _Generic(                                                                    \
      (range), ATB_RANGEVIEW_GENERIC_ASSOCIATION(Constu32, _Fromu8, u8),       \
      ATB_RANGEVIEW_GENERIC_ASSOCIATION(Constu32, _Fromu16, u16),              \
      ATB_RANGEVIEW_GENERIC_ASSOCIATION(Constu32, _Fromu32, u32),              \
      ATB_RANGEVIEW_GENERIC_ASSOCIATION(Constu32, _Fromu64, u64),              \
      ATB_RANGEVIEW_GENERIC_ASSOCIATION(Constu32, _FromConstu8, Constu8),      \
      ATB_RANGEVIEW_GENERIC_ASSOCIATION(Constu32, _FromConstu16, Constu16),    \
      ATB_RANGEVIEW_GENERIC_ASSOCIATION(Constu32, _FromConstu32, Constu32),    \
      ATB_RANGEVIEW_GENERIC_ASSOCIATION(Constu32, _FromConstu64, Constu64))(   \
      range)

_ATB_RANGEVIEW_DECLARE_ALL_CONVERSION(u64, u8)
_ATB_RANGEVIEW_DECLARE_ALL_CONVERSION(u64, u16)
_ATB_RANGEVIEW_DECLARE_ALL_CONVERSION(u64, u32)
_ATB_RANGEVIEW_DECLARE_CONVERSION(u64, u64)
_ATB_RANGEVIEW_DECLARE_CONVERSION(Constu64, Constu64)

/**
 *  \return atb_u64RangeView from the given input range (non-const only)
 *  \param[in] range Input arithmetic UNSIGNED range we wish to convert
 */
#define atb_u64RangeView_From(range)                                           \
  _Generic((range), ATB_RANGEVIEW_GENERIC_ASSOCIATION(u64, _Fromu8, u8),       \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(u64, _Fromu16, u16),              \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(u64, _Fromu32, u32),              \
           ATB_RANGEVIEW_GENERIC_ASSOCIATION(u64, _Fromu64, u64))(range)

/**
 *  \return atb_Constu64RangeView from the given input range (const or not)
 *  \param[in] range Input arithmetic UNSIGNED range we wish to convert
 */
#define atb_Constu64RangeView_From(range)                                      \
  _Generic(                                                                    \
      (range), ATB_RANGEVIEW_GENERIC_ASSOCIATION(Constu64, _Fromu8, u8),       \
      ATB_RANGEVIEW_GENERIC_ASSOCIATION(Constu64, _Fromu16, u16),              \
      ATB_RANGEVIEW_GENERIC_ASSOCIATION(Constu64, _Fromu32, u32),              \
      ATB_RANGEVIEW_GENERIC_ASSOCIATION(Constu64, _Fromu64, u64),              \
      ATB_RANGEVIEW_GENERIC_ASSOCIATION(Constu64, _FromConstu8, Constu8),      \
      ATB_RANGEVIEW_GENERIC_ASSOCIATION(Constu64, _FromConstu16, Constu16),    \
      ATB_RANGEVIEW_GENERIC_ASSOCIATION(Constu64, _FromConstu32, Constu32),    \
      ATB_RANGEVIEW_GENERIC_ASSOCIATION(Constu64, _FromConstu64, Constu64))(   \
      range)

/* Used as string view */
ATB_RANGEVIEW_DECLARE_NEW(Char, char)

/**
 *  \return atb_ConstRangeView From a STRING LITERAL
 *  \param[in] str String literal
 */
#define atb_ConstCharRangeView_FromStringLiteral(str)                          \
  ATB_COMPOUND_LITERAL(atb_ConstCharRangeView) {                               \
    /* .data =  */ str, /* .size =  */ (atb_StaticArray_GetSize(str) - 1)      \
  }

/**
 *  \return struct atb_RangeView From a given null terminated str
 *  \param[in] str Null terminated string
 */
static inline struct atb_CharRangeView
atb_CharRangeView_FromNullTerminatedStr(char *const str) {
  return ATB_COMPOUND_LITERAL(atb_CharRangeView){
      /* .data = */ str,
      /* .size = */ strlen(str),
  };
}

/**
 *  \return struct atb_ConstCharRangeView From a given const null
 * terminated str \param[in] str Const null terminated string
 */
static inline struct atb_ConstCharRangeView
atb_ConstCharRangeView_FromNullTerminatedStr(char const *const str) {
  return ATB_COMPOUND_LITERAL(atb_ConstCharRangeView){
      /* .data = */ str,
      /* .size = */ strlen(str),
  };
}

#undef _ATB_RANGEVIEW_DECLARE_ALL_CONVERSION
#undef _ATB_RANGEVIEW_DECLARE_CONVERSION

#if defined(__cplusplus)
} /* extern "C" */
#endif
