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

/**
 *  \brief Statically initialize a atb_NRangeView data structure
 */
#define atb_AnyRangeView_INITIALIZER()                                         \
  { NULL, 0 }

/**
 *  \brief Map a STATIC array (size known at compile time) into the range
 *
 *  \param[inout] range Range used to map the array
 *  \param[in] array Compile time array
 */
#define atb_AnyRangeView_MapStaticArray(range, array)                          \
  do {                                                                         \
    range.data = array;                                                        \
    range.size = atb_StaticArray_GetSize(array);                               \
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

#define atb_AnyRangeView_Fmt "{.data = %p, .size = %zu (%zu bytes)}"
#define atb_AnyRangeView_FmtArg(range)                                         \
  ((void *)(range).data), (range).size, ((range).size * sizeof(*(range).data))

#define atb_AnyRangeView_Begin(range) (range).data
#define atb_AnyRangeView_End(range)                                            \
  (atb_AnyRangeView_Begin(range) + (range).size)
#define atb_AnyRangeView_ForEach(elem, range)                                  \
  for (elem = atb_AnyRangeView_Begin(range);                                   \
       elem != atb_AnyRangeView_End(range); elem += 1)

#define atb_AnyRangeView_RBegin(range) atb_AnyRangeView_End(range) - 1
#define atb_AnyRangeView_REnd(range) atb_AnyRangeView_Begin(range) - 1
#define atb_AnyRangeView_RForEach(elem, range)                                 \
  for (elem = atb_AnyRangeView_RBegin(range);                                  \
       elem != atb_AnyRangeView_REnd(range); elem -= 1)

#define ATB_RANGEVIEW_DECLARE(prefix, type)                                    \
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
  static inline struct atb_Const##prefix##RangeView                            \
      atb_##prefix##RangeView_ToConst(struct atb_##prefix##RangeView range) {  \
    return ATB_COMPOUND_LITERAL(atb_Const##prefix##RangeView){                 \
        /* .data = */ (const type *)range.data,                                \
        /* .size = */ range.size,                                              \
    };                                                                         \
  }                                                                            \
                                                                               \
  static inline struct atb_Const##prefix##RangeView                            \
      atb_Const##prefix##RangeView_From##prefix##RangeView(                    \
          struct atb_##prefix##RangeView range) {                              \
    return atb_##prefix##RangeView_ToConst(range);                             \
  }                                                                            \
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
            atb_##prefix##RangeView_ToConst(range), other)) {                  \
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

ATB_RANGEVIEW_DECLARE(8, int8_t)
ATB_RANGEVIEW_DECLARE(16, int16_t)
ATB_RANGEVIEW_DECLARE(32, int32_t)
ATB_RANGEVIEW_DECLARE(64, int64_t)

ATB_RANGEVIEW_DECLARE(U8, uint8_t)
ATB_RANGEVIEW_DECLARE(U16, uint16_t)
ATB_RANGEVIEW_DECLARE(U32, uint32_t)
ATB_RANGEVIEW_DECLARE(U64, uint64_t)

ATB_RANGEVIEW_DECLARE(Char, char)

/**
 *  \return atb_ConstRangeView From a STRING LITERAL
 *  \param[in] str String literal
 */
#define atb_CharConstRangeView_FromStringLiteral(str)                          \
  ATB_COMPOUND_LITERAL(atb_CharConstRangeView) {                               \
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
 *  \return struct atb_ConstCharRangeView From a given const null terminated str
 *  \param[in] str Const null terminated string
 */
static inline struct atb_ConstCharRangeView
atb_ConstCharRangeView_FromNullTerminatedStr(char const *const str) {
  return ATB_COMPOUND_LITERAL(atb_ConstCharRangeView){
      /* .data = */ str,
      /* .size = */ strlen(str),
  };
}

#define ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION(prefix, fn_postfix)              \
  struct atb_##prefix##RangeView : atb_##prefix##RangeView##fn_postfix

#define ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION_FOR_ALL(prefix, fn_postfix)      \
  ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION(prefix, fn_postfix),                   \
      const ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION(prefix, fn_postfix),         \
      ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION(Const##prefix, fn_postfix)         \
          const ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION(Const##prefix,           \
                                                      fn_postfix)

/**
 *  \brief Shrink a range (reduce it's size) from the front OR the back
 *
 *  \param[in] range Range we wish to shrink
 *  \param[in] offset Number of elements we wish to remove
 *  \param[in] front Shrink the range from the front if true, the back otherwise
 *
 *  \warning range is EXPECTED to be defined (non null data ptr)
 *  \note If the offset is bigger than the range size, the range will be shrink
 *        by its size instead
 *
 *  \return atb_**RangeView corresponding to the same range, shrinked by the
 *          requested amount
 */
#define atb_RangeView_Shrink(range, offset, front)                             \
  _Generic((range), ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION_FOR_ALL(8, _Shrink), \
           ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION_FOR_ALL(16, _Shrink),         \
           ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION_FOR_ALL(32, _Shrink),         \
           ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION_FOR_ALL(64, _Shrink),         \
           ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION_FOR_ALL(U8, _Shrink),         \
           ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION_FOR_ALL(U16, _Shrink),        \
           ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION_FOR_ALL(U32, _Shrink),        \
           ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION_FOR_ALL(U64, _Shrink),        \
           ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION_FOR_ALL(Char, _Shrink))(      \
      range, offset, front)

/**
 *  \brief Shift/translate a range to the right or left
 *
 *  \param[in] range Range we wish to shift
 *  \param[in] offset Number of elements we wish to shitf
 *  \param[in] right Shift the range to the right if true, the left otherwise
 *
 *  \warning range is EXPECTED to be defined (non null data ptr)
 *
 *  \return atb_**RangeView corresponding to the same range, shifted by the
 *          requested amount
 */
#define atb_RangeView_Shift(range, offset, right)                              \
  _Generic((range), ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION_FOR_ALL(8, _Shift),  \
           ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION_FOR_ALL(16, _Shift),          \
           ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION_FOR_ALL(32, _Shift),          \
           ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION_FOR_ALL(64, _Shift),          \
           ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION_FOR_ALL(U8, _Shift),          \
           ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION_FOR_ALL(U16, _Shift),         \
           ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION_FOR_ALL(U32, _Shift),         \
           ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION_FOR_ALL(U64, _Shift),         \
           ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION_FOR_ALL(Char, _Shift))(       \
      range, offset, right)

/**
 *  \brief Copy other into range, up to the range size
 *
 *  \param[in] range Range we wish to copy value into
 *  \param[in] other Range we wish to copy from
 *
 *  \warning range and other are EXPECTED to be defined (non null data ptr)
 *
 *  \return atb_**RangeView corresponding to the input range, shifted by the
 *          amount of elements written into it
 */
#define atb_RangeView_CopyInto(range, other)                                   \
  _Generic((range), ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION(8, _CopyInto),       \
           ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION(16, _CopyInto),               \
           ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION(32, _CopyInto),               \
           ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION(64, _CopyInto),               \
           ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION(U8, _CopyInto),               \
           ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION(U16, _CopyInto),              \
           ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION(U32, _CopyInto),              \
           ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION(U64, _CopyInto),              \
           ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION(Char, _CopyInto))(range,      \
                                                                   other)

#define atb_RangeView_IsEqualTo(lhs, rhs)                                      \
  _Generic((lhs), ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION(Const8, _IsEqualTo),   \
           ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION(Const16, _IsEqualTo),         \
           ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION(Const32, _IsEqualTo),         \
           ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION(Const64, _IsEqualTo),         \
           ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION(ConstU8, _IsEqualTo),         \
           ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION(ConstU16, _IsEqualTo),        \
           ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION(ConstU32, _IsEqualTo),        \
           ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION(ConstU64, _IsEqualTo),        \
           ATB_RANGEVIEW_ADD_GENERIC_ASSOCIATION(ConstChar, _IsEqualTo))(lhs,  \
                                                                         rhs)

#if defined(__cplusplus)
} /* extern "C" */
#endif
