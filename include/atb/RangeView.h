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

#define _DECLARE_NEW_RANGEVIEW(prefix, type)                                   \
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

_DECLARE_NEW_RANGEVIEW(8, int8_t)
_DECLARE_NEW_RANGEVIEW(16, int16_t)
_DECLARE_NEW_RANGEVIEW(32, int32_t)
_DECLARE_NEW_RANGEVIEW(64, int64_t)

_DECLARE_NEW_RANGEVIEW(u8, uint8_t)
_DECLARE_NEW_RANGEVIEW(u16, uint16_t)
_DECLARE_NEW_RANGEVIEW(u32, uint32_t)
_DECLARE_NEW_RANGEVIEW(u64, uint64_t)

/* Used as string view */
_DECLARE_NEW_RANGEVIEW(Char, char)

#undef _DECLARE_NEW_RANGEVIEW

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

/**
 * Custom macro helper to declare a single generic association for one
 * RangeViews, used inside _Generic()
 */
#define _ADD_GENERIC_ASSOCIATION(prefix, fn_postfix)                           \
  struct atb_##prefix##RangeView : atb_##prefix##RangeView##fn_postfix

/**
 * Custom macro helper to declare ALL generic association of generated
 * RangeViews (NRangeView, ConstNRangeView, const NRangeView and const
 * ConstNRangeView), used inside _Generic() (all pointing to the same function)
 */
#define _ADD_GENERIC_ASSOCIATION_ALL_CV(prefix, fn_postfix)                    \
  _ADD_GENERIC_ASSOCIATION(prefix, fn_postfix),                                \
      const _ADD_GENERIC_ASSOCIATION(prefix, fn_postfix),                      \
      _ADD_GENERIC_ASSOCIATION(Const##prefix, fn_postfix)                      \
          const _ADD_GENERIC_ASSOCIATION(Const##prefix, fn_postfix)

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
#define atb_AnyRangeView_Shrink(range, offset, front)                          \
  _Generic((range), _ADD_GENERIC_ASSOCIATION_ALL_CV(8, _Shrink),               \
           _ADD_GENERIC_ASSOCIATION_ALL_CV(16, _Shrink),                       \
           _ADD_GENERIC_ASSOCIATION_ALL_CV(32, _Shrink),                       \
           _ADD_GENERIC_ASSOCIATION_ALL_CV(64, _Shrink),                       \
           _ADD_GENERIC_ASSOCIATION_ALL_CV(u8, _Shrink),                       \
           _ADD_GENERIC_ASSOCIATION_ALL_CV(u16, _Shrink),                      \
           _ADD_GENERIC_ASSOCIATION_ALL_CV(u32, _Shrink),                      \
           _ADD_GENERIC_ASSOCIATION_ALL_CV(u64, _Shrink),                      \
           _ADD_GENERIC_ASSOCIATION_ALL_CV(Char, _Shrink))(range, offset,      \
                                                           front)

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
#define atb_AnyRangeView_Shift(range, offset, right)                           \
  _Generic((range), _ADD_GENERIC_ASSOCIATION_ALL_CV(8, _Shift),                \
           _ADD_GENERIC_ASSOCIATION_ALL_CV(16, _Shift),                        \
           _ADD_GENERIC_ASSOCIATION_ALL_CV(32, _Shift),                        \
           _ADD_GENERIC_ASSOCIATION_ALL_CV(64, _Shift),                        \
           _ADD_GENERIC_ASSOCIATION_ALL_CV(u8, _Shift),                        \
           _ADD_GENERIC_ASSOCIATION_ALL_CV(u16, _Shift),                       \
           _ADD_GENERIC_ASSOCIATION_ALL_CV(u32, _Shift),                       \
           _ADD_GENERIC_ASSOCIATION_ALL_CV(u64, _Shift),                       \
           _ADD_GENERIC_ASSOCIATION_ALL_CV(Char, _Shift))(range, offset,       \
                                                          right)

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
#define atb_AnyRangeView_CopyInto(range, other)                                \
  _Generic((range), _ADD_GENERIC_ASSOCIATION(8, _CopyInto),                    \
           _ADD_GENERIC_ASSOCIATION(16, _CopyInto),                            \
           _ADD_GENERIC_ASSOCIATION(32, _CopyInto),                            \
           _ADD_GENERIC_ASSOCIATION(64, _CopyInto),                            \
           _ADD_GENERIC_ASSOCIATION(u8, _CopyInto),                            \
           _ADD_GENERIC_ASSOCIATION(u16, _CopyInto),                           \
           _ADD_GENERIC_ASSOCIATION(u32, _CopyInto),                           \
           _ADD_GENERIC_ASSOCIATION(u64, _CopyInto),                           \
           _ADD_GENERIC_ASSOCIATION(Char, _CopyInto))(range, other)

/**
 *  \return True when both range are equals (data), false otherwise.
 *
 *  \note If both ranges have NULL data, it actually compares the size of both
 * ranges, hence 2 NULL ranges with the same sizes, are considered equal.
 *
 *  \param[in] lhs, rhs Ranges we wish to compare
 */
#define atb_AnyRangeView_IsEqualTo(lhs, rhs)                                   \
  _Generic((lhs), _ADD_GENERIC_ASSOCIATION(Const8, _IsEqualTo),                \
           _ADD_GENERIC_ASSOCIATION(Const16, _IsEqualTo),                      \
           _ADD_GENERIC_ASSOCIATION(Const32, _IsEqualTo),                      \
           _ADD_GENERIC_ASSOCIATION(Const64, _IsEqualTo),                      \
           _ADD_GENERIC_ASSOCIATION(Constu8, _IsEqualTo),                      \
           _ADD_GENERIC_ASSOCIATION(Constu16, _IsEqualTo),                     \
           _ADD_GENERIC_ASSOCIATION(Constu32, _IsEqualTo),                     \
           _ADD_GENERIC_ASSOCIATION(Constu64, _IsEqualTo),                     \
           _ADD_GENERIC_ASSOCIATION(ConstChar, _IsEqualTo))(lhs, rhs)

#undef _ADD_GENERIC_ASSOCIATION_ALL_CV
#undef _ADD_GENERIC_ASSOCIATION

#if defined(__cplusplus)
} /* extern "C" */
#endif
