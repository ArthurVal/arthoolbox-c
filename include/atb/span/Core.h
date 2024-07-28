#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#include <assert.h>
#include <stdbool.h> /* bool */
#include <stddef.h>  /* size_t */
#include <string.h>  /* strlen, memcmp */

#include "atb/generic/MacroUtils.h"  /* ATB_COMPOUND_LITERAL */
#include "atb/generic/StaticArray.h" /* GetSize */

#define ATB_SPAN_DECLARE_NEW_FROM(to, from)                                    \
  static inline struct atb_##to##Span atb_##to##Span_From##from(               \
      struct atb_##from##Span range) {                                         \
                                                                               \
    struct atb_##to##Span out;                                                 \
    out.data = (__typeof(out.data))range.data;                                 \
    out.size =                                                                 \
        (range.size * ((double)sizeof(*range.data) / sizeof(*out.data)));      \
    return out;                                                                \
  }

#define ATB_SPAN_DECLARE_NEW_FROM_ALL(to, from)                                \
  ATB_SPAN_DECLARE_NEW_FROM(to, from)                                          \
  ATB_SPAN_DECLARE_NEW_FROM(Const##to, from)                                   \
  ATB_SPAN_DECLARE_NEW_FROM(Const##to, Const##from)

#define ATB_SPAN_DECLARE_NEW(prefix, type)                                     \
  struct atb_##prefix##Span {                                                  \
    type *data;                                                                \
    size_t size;                                                               \
  };                                                                           \
                                                                               \
  struct atb_Const##prefix##Span {                                             \
    const type *data;                                                          \
    size_t size;                                                               \
  };                                                                           \
                                                                               \
  ATB_SPAN_DECLARE_NEW_FROM_ALL(prefix, prefix)                                \
                                                                               \
  static inline bool atb_Const##prefix##Span_IsOverlapping(                    \
      struct atb_Const##prefix##Span lhs,                                      \
      struct atb_Const##prefix##Span rhs) {                                    \
                                                                               \
    if (atb_AnySpan_Begin(lhs) == atb_AnySpan_Begin(rhs)) {                    \
      return (lhs.size != 0) && (rhs.size != 0);                               \
    } else if (atb_AnySpan_Begin(lhs) < atb_AnySpan_Begin(rhs)) {              \
      return (atb_AnySpan_End(lhs) <= atb_AnySpan_Begin(rhs));                 \
    } else {                                                                   \
      return (atb_AnySpan_End(rhs) <= atb_AnySpan_Begin(lhs));                 \
    }                                                                          \
  }                                                                            \
                                                                               \
  static inline struct atb_Const##prefix##Span atb_Const##prefix##Span_Shrink( \
      struct atb_Const##prefix##Span range, size_t offset, bool front) {       \
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
  static inline struct atb_##prefix##Span atb_##prefix##Span_Shrink(           \
      struct atb_##prefix##Span range, size_t offset, bool front) {            \
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
  static inline struct atb_Const##prefix##Span atb_Const##prefix##Span_Slide(  \
      struct atb_Const##prefix##Span range, long offset) {                     \
    assert(range.data != NULL);                                                \
    range.data += offset;                                                      \
    return range;                                                              \
  }                                                                            \
                                                                               \
  static inline struct atb_##prefix##Span atb_##prefix##Span_Slide(            \
      struct atb_##prefix##Span range, long offset) {                          \
    assert(range.data != NULL);                                                \
    range.data += offset;                                                      \
    return range;                                                              \
  }                                                                            \
                                                                               \
  static inline struct atb_##prefix##Span atb_##prefix##Span_CopyInto(         \
      struct atb_##prefix##Span range, struct atb_Const##prefix##Span other) { \
                                                                               \
    assert(range.data != NULL);                                                \
    assert(other.data != NULL);                                                \
                                                                               \
    if (range.size < other.size)                                               \
      other.size = range.size;                                                 \
                                                                               \
    if (atb_Const##prefix##Span_IsOverlapping(                                 \
            atb_Const##prefix##Span_From##prefix(range), other)) {             \
      memmove(range.data, other.data, (other.size * sizeof(type)));            \
    } else {                                                                   \
      memcpy(range.data, other.data, (other.size * sizeof(type)));             \
    }                                                                          \
                                                                               \
    atb_##prefix##Span_Shrink(range, other.size, true);                        \
    return range;                                                              \
  }                                                                            \
                                                                               \
  static inline bool atb_Const##prefix##Span_IsEqualTo(                        \
      struct atb_Const##prefix##Span lhs,                                      \
      struct atb_Const##prefix##Span rhs) {                                    \
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
 * Span, used inside _Generic()
 */
#define ATB_SPAN_GENERIC_ASSOCIATION(fn_prefix, fn_postfix, arg_type)          \
  struct atb_##arg_type##Span : atb_##fn_prefix##Span##fn_postfix

/**
 * Custom macro helper to declare ALL generic association of generated
 * Spans (NSpan, ConstNSpan), used inside _Generic() (all
 * pointing to the same function)
 */
#define ATB_SPAN_ALL_GENERIC_ASSOCIATION(fn_postfix, arg_type)                 \
  ATB_SPAN_GENERIC_ASSOCIATION(arg_type, fn_postfix, arg_type),                \
      ATB_SPAN_GENERIC_ASSOCIATION(Const##arg_type, fn_postfix,                \
                                   Const##arg_type)

/**
 *  \brief Statically initialize a atb_**Span data structure
 */
#define atb_AnySpan_INITIALIZER()                                              \
  { NULL, 0 }

/**
 *  \brief Printf format string helper
 */
#define atb_AnySpan_Fmt "{.data = %p, .size = %zu (%zu bytes)}"

/**
 *  \brief Printf argument helper
 *
 *  \warning DO NOT use any side effect statement as range input
 */
#define atb_AnySpan_FmtArg(range)                                              \
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
#define atb_AnySpan_MapStaticArray(range, array)                               \
  do {                                                                         \
    (range).data = array;                                                      \
    (range).size = atb_StaticArray_GetSize(array);                             \
  } while (0)

/**
 *  \return atb_**Span Maping a STATIC array (size known at compile time)
 *
 *  \param[in] prefix The prefix name of the range view
 *  \param[in] array Compile time array
 */
#define atb_Span_FromStaticArray(prefix, array)                                \
  ATB_COMPOUND_LITERAL(atb_##prefix##Span) {                                   \
    /* .data = */ array, /* .size = */ atb_StaticArray_GetSize(array),         \
  }

/**
 *  \return atb_**ConstRangeNView Maping a STATIC array (size known at compile
 *                                time)
 *
 *  \param[in] prefix The prefix name of the range view
 *  \param[in] array Compile time array
 */
#define atb_ConstSpan_FromStaticArray(prefix, array)                           \
  ATB_COMPOUND_LITERAL(atb_##prefix##ConstSpan) {                              \
    /* .data = */ array, /* .size = */ atb_StaticArray_GetSize(array),         \
  }

/**
 *  \return Pointer to the begin of the range
 */
#define atb_AnySpan_Begin(range) (range).data

/**
 *  \return Pointer to the end of the range (one past the last valid element)
 *
 *  \warning DO NOT use any side effect statement as range input
 */
#define atb_AnySpan_End(range) (atb_AnySpan_Begin(range) + (range).size)

/**
 *  \brief Loop helper used to iterate over the range, using elem as iterator
 *
 *  \warning DO NOT use any side effect statement as range input
 */
#define atb_AnySpan_ForEach(elem, range)                                       \
  for ((elem) = atb_AnySpan_Begin(range); (elem) != atb_AnySpan_End(range);    \
       (elem) += 1)

/**
 *  \return Pointer to the rbegin of the range. (Last valid element)
 *
 *  \warning DO NOT use any side effect statement as range input
 */
#define atb_AnySpan_RBegin(range) atb_AnySpan_End(range) - 1

/**
 *  \return Pointer to the rend of the range (one before the first valid
 * element)
 */
#define atb_AnySpan_REnd(range) atb_AnySpan_Begin(range) - 1

/**
 *  \brief Loop helper used to REVERSE iterate over the range, using elem as
 * iterator
 *
 *  \warning DO NOT use any side effect statement as range input
 */
#define atb_AnySpan_RForEach(elem, range)                                      \
  for (elem = atb_AnySpan_RBegin(range); elem != atb_AnySpan_REnd(range);      \
       elem -= 1)

#if defined(__cplusplus)
} /* extern "C" */
#endif
