#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "atb/array.h"
#include "atb/compare.h"

#if defined(__cplusplus)
extern "C" {
#endif

/// Format string use to introspect any span content
#define K_ATB_FMT_ANYSPAN_RAW "{.data=%p, .size=%zu}"

/// Format string argument. Must be used in conjunction of K_ATB_FMT_SPAN_RAW
#define ATB_FMT_VA_ARG_ANYSPAN_RAW(span) (span).data, (span).size

/// An invalid span (static initializer)
#define K_ATB_ANYSPAN_INVALID \
  { .data = NULL, .size = 0 }

/// Initialize ANY span from a STATIC array (static initializer)
#define atb_AnySpan_From_Array(arr) \
  { .data = arr, .size = atb_Array_Size(arr), }

/// Pointer to the begin of the span
#define atb_AnySpan_Begin(span) (span).data

/// Pointer one past the last element of the span (canno't be dereferenced)
#define atb_AnySpan_End(span) (atb_AnySpan_Begin(span) + (span).size)

/// Helper macro to iterator over a span using \a elem ptr
#define atb_AnySpan_ForEach(elem, span) \
  for (elem = atb_AnySpan_Begin(span); elem != atb_AnySpan_End(span); ++elem)

typedef enum {
  K_ATB_SPAN_SHRINK_FRONT, /*!< Truncate the BEGIN of the span */
  K_ATB_SPAN_SHRINK_BACK,  /*!< Truncate the END of the span */
} ATB_SPAN_SHRINK_DIR;

/// True whenever the enum value is in its defined range.
static inline bool ATB_SPAN_SHRINK_DIR_IsValid(ATB_SPAN_SHRINK_DIR v) {
  switch (v) {
    case K_ATB_SPAN_SHRINK_FRONT:
    case K_ATB_SPAN_SHRINK_BACK:
      return true;
  }
  return false;
}

/// Options used to configure VIEW_Copy
typedef struct atb_View_Copy_Opt {
  unsigned truncate : 1; /*!< Truncate the input view to fit the dest buffer */
  unsigned overlap : 1;  /*!< Indicate that both view/dest are overlapping */
} atb_View_Copy_Opt_t;

/// Declare a span struct named \a NAME, representing a contiguous, NON-OWNING,
/// range of values of type \a T, and all its associated functions.
/// All functions are declared using \a SPECIFIER as specifiers
///
/// Functions declared are the following:
/// - `_From(ptr, size) -> SPAN`: Create a span from the given ptr/size;
/// - `_IsValid(span) -> bool`: Indicates if the span is VALID;
/// - `_SizeBytes(span) -> size_t`: Returns the number of BYTES of the span;
/// - `_IsOverlapping(span, span) -> bool`: True whenever both spans are
///   overlapping memory-wise ;
/// - `_First(span, n) -> span`: Create a new span containing the firsts N
///   elements of the initial one;
/// - `_Last(span, n) -> span`: Create a new span containing the lasts N
///   elements of the initial one;
/// - `_Shrink(span, n, dir) -> span`: Create a new span corresponding to a
///   shrinked version of the initial span;
/// - `_Slice(span, offset, count) -> span`: Create a new span corresponding to
///   a slice, starting from offset elements of the initial span and containing
///   count elements;
/// - `_Compare(span, span) -> int`: Three-way comparison (lexicographically) of
///   2 spans;
/// - `_[Eq, Ne, Lt, Gt, Le, Ge](span, span) -> bool`: Compare 2 spans;
#define ATB_SPAN_DECLARE(SPECIFIER, NAME, T)                       \
  struct NAME;                                                     \
  SPECIFIER struct NAME NAME##_From(T *, size_t);                  \
  SPECIFIER bool NAME##_IsValid(struct NAME);                      \
  SPECIFIER size_t NAME##_SizeBytes(struct NAME);                  \
  SPECIFIER bool NAME##_IsOverlapping(struct NAME, struct NAME);   \
  SPECIFIER struct NAME NAME##_First(struct NAME, size_t);         \
  SPECIFIER struct NAME NAME##_Last(struct NAME, size_t);          \
  SPECIFIER struct NAME NAME##_Shrink(struct NAME, size_t,         \
                                      ATB_SPAN_SHRINK_DIR);        \
  SPECIFIER struct NAME NAME##_Slice(struct NAME, size_t, size_t); \
  SPECIFIER atb_Cmp_t NAME##_Compare(struct NAME, struct NAME);    \
  ATB_CMP_DECLARE_ALL(SPECIFIER, NAME##_, struct NAME)

/// Define a span struct named \a NAME, representing a contiguous, NON-OWNING,
/// range of values of type \a T, and all its associated functions.
/// All functions are defined using \a SPECIFIER as specifiers.
///
/// Functions defined are the following:
/// - `_From(ptr, size) -> SPAN`: Create a span from the given ptr/size;
/// - `_IsValid(span) -> bool`: Indicates if the span is VALID;
/// - `_SizeBytes(span) -> size_t`: Returns the number of BYTES of the span;
/// - `_IsOverlapping(span, span) -> bool`: True whenever both spans are
///   overlapping memory-wise ;
/// - `_First(span, n) -> span`: Create a new span containing the firsts N
///   elements of the initial one;
/// - `_Last(span, n) -> span`: Create a new span containing the lasts N
///   elements of the initial one;
/// - `_Shrink(span, n, dir) -> span`: Create a new span corresponding to a
///   shrinked version of the initial span;
/// - `_Slice(span, offset, count) -> span`: Create a new span corresponding to
///   a slice, starting from offset elements of the initial span and containing
///   count elements;
/// - `_Compare(span, span) -> int`: Three-way comparison (lexicographically) of
///   2 spans;
/// - `_[Eq, Ne, Lt, Gt, Le, Ge](span, span) -> bool`: Compare 2 spans;
#define ATB_SPAN_DEFINE(SPECIFIER, NAME, T)                                  \
  struct NAME {                                                              \
    T *data;                                                                 \
    size_t size;                                                             \
  };                                                                         \
                                                                             \
  SPECIFIER struct NAME NAME##_From(T *other, size_t size) {                 \
    struct NAME s;                                                           \
    s.data = other;                                                          \
    s.size = size;                                                           \
    return s;                                                                \
  }                                                                          \
                                                                             \
  SPECIFIER bool NAME##_IsValid(struct NAME span) {                          \
    return span.data != NULL;                                                \
  }                                                                          \
                                                                             \
  SPECIFIER size_t NAME##_SizeBytes(struct NAME span) {                      \
    assert(NAME##_IsValid(span));                                            \
    return span.size * sizeof(*span.data);                                   \
  }                                                                          \
                                                                             \
  SPECIFIER bool NAME##_IsOverlapping(struct NAME lhs, struct NAME rhs) {    \
    assert(NAME##_IsValid(lhs));                                             \
    assert(NAME##_IsValid(rhs));                                             \
                                                                             \
    struct NAME const *hi = NULL;                                            \
    struct NAME const *lo = NULL;                                            \
                                                                             \
    if (lhs.data >= rhs.data) {                                              \
      hi = &lhs;                                                             \
      lo = &rhs;                                                             \
    } else {                                                                 \
      hi = &rhs;                                                             \
      lo = &lhs;                                                             \
    }                                                                        \
                                                                             \
    return atb_AnySpan_End(*lo) > atb_AnySpan_Begin(*hi);                    \
  }                                                                          \
                                                                             \
  SPECIFIER struct NAME NAME##_First(struct NAME span, size_t n) {           \
    assert(NAME##_IsValid(span));                                            \
                                                                             \
    n = (n > span.size ? span.size : n);                                     \
    span.size = n;                                                           \
    return span;                                                             \
  }                                                                          \
                                                                             \
  SPECIFIER struct NAME NAME##_Last(struct NAME span, size_t n) {            \
    assert(NAME##_IsValid(span));                                            \
                                                                             \
    n = (n > span.size ? span.size : n);                                     \
    span.data += (span.size - n);                                            \
    span.size = n;                                                           \
    return span;                                                             \
  }                                                                          \
                                                                             \
  SPECIFIER struct NAME NAME##_Shrink(struct NAME span, size_t n,            \
                                      ATB_SPAN_SHRINK_DIR dir) {             \
    assert(NAME##_IsValid(span));                                            \
    assert(ATB_SPAN_SHRINK_DIR_IsValid(dir));                                \
                                                                             \
    n = (n > span.size ? span.size : n);                                     \
    span.size -= n;                                                          \
    if (dir == K_ATB_SPAN_SHRINK_FRONT) {                                    \
      span.data += n;                                                        \
    }                                                                        \
                                                                             \
    return span;                                                             \
  }                                                                          \
                                                                             \
  SPECIFIER struct NAME NAME##_Slice(struct NAME span, size_t offset,        \
                                     size_t count) {                         \
    assert(NAME##_IsValid(span));                                            \
                                                                             \
    span = NAME##_Shrink(span, offset, K_ATB_SPAN_SHRINK_FRONT);             \
    span.size = (count > span.size ? span.size : count);                     \
    return span;                                                             \
  }                                                                          \
                                                                             \
  SPECIFIER atb_Cmp_t NAME##_Compare(struct NAME lhs, struct NAME rhs) {     \
    assert(NAME##_IsValid(lhs));                                             \
    assert(NAME##_IsValid(rhs));                                             \
                                                                             \
    if (lhs.size > rhs.size) {                                               \
      return K_ATB_CMP_GREATER;                                              \
    } else if (lhs.size < rhs.size) {                                        \
      return K_ATB_CMP_LESS;                                                 \
    } else {                                                                 \
      return (atb_Cmp_t)(memcmp(lhs.data, rhs.data, NAME##_SizeBytes(lhs))); \
    }                                                                        \
  }                                                                          \
                                                                             \
  ATB_CMP_DEFINE_ALL_FROM_UNSAFE_COMPARE(SPECIFIER, NAME##_, struct NAME)

/// Declare a PAIR of struct corresponding to a span (modifiable) and a view
/// (constant) (respectively named \a SPAN and \a VIEW), representing a
/// contiguous, NON-OWNING, range of values of type \a T, and all their
/// associated functions. All functions are declared using \a SPECIFIER as
/// specifiers
///
/// Functions declared for EACH structs individually (i.e. SPAN and VIEW):
/// - `_From(ptr, size) -> span`: Create a span from the given ptr/size;
/// - `_IsValid(span) -> bool`: Indicates if the span is VALID;
/// - `_SizeBytes(span) -> size_t`: Returns the number of BYTES of the span;
/// - `_IsOverlapping(span, span) -> bool`: True whenever both spans are
///   overlapping memory-wise ;
/// - `_First(span, n) -> span`: Create a new span containing the firsts N
///   elements of the initial one;
/// - `_Last(span, n) -> span`: Create a new span containing the lasts N
///   elements of the initial one;
/// - `_Shrink(span, n, dir) -> span`: Create a new span corresponding to a
///   shrinked version of the initial span;
/// - `_Slice(span, offset, count) -> span`: Create a new span corresponding to
///   a slice, starting from offset elements of the initial span and containing
///   count elements;
/// - `_Compare(span, span) -> int`: Three-way comparison (lexicographically) of
///   2 spans;
/// - `_[Eq, Ne, Lt, Gt, Le, Ge](span, span) -> bool`: Compare 2 spans;
///
/// Functions declared only for SPAN ONLY:
/// - `_Fill(span, byte) -> void`: Fill the span with byte;
///
/// Functions declared only for VIEW ONLY:
/// - `_From_Span(span) -> view`: Create a view from given span;
/// - `_Copy(view, span, opt) -> span`: Copy elements of view into span and
///   returns the remaining space inside the destination span if succeeded to
///   copy. Otherwise returns an INVALID span;
#define ATB_SPAN_VIEW_DECLARE(SPECIFIER, SPAN, VIEW, T)       \
  ATB_SPAN_DECLARE(SPECIFIER, SPAN, T);                       \
  SPECIFIER void SPAN##_Fill(struct SPAN, char);              \
  ATB_SPAN_DECLARE(SPECIFIER, VIEW, const T);                 \
  SPECIFIER struct VIEW VIEW##_From_Span(struct SPAN);        \
  SPECIFIER struct SPAN VIEW##_Copy(struct VIEW, struct SPAN, \
                                    atb_View_Copy_Opt_t)

/// Define a PAIR of struct corresponding to a span (modifiable) and a view
/// (constant) (respectively named \a SPAN and \a VIEW), representing a
/// contiguous, NON-OWNING, range of values of type \a T, and all their
/// associated functions. All functions are declared using \a SPECIFIER as
/// specifiers
///
/// Functions defined for EACH structs individually (i.e. SPAN and VIEW):
/// - `_From(ptr, size) -> span`: Create a span from the given ptr/size;
/// - `_IsValid(span) -> bool`: Indicates if the span is VALID;
/// - `_SizeBytes(span) -> size_t`: Returns the number of BYTES of the span;
/// - `_IsOverlapping(span, span) -> bool`: True whenever both spans are
///   overlapping memory-wise ;
/// - `_First(span, n) -> span`: Create a new span containing the firsts N
///   elements of the initial one;
/// - `_Last(span, n) -> span`: Create a new span containing the lasts N
///   elements of the initial one;
/// - `_Shrink(span, n, dir) -> span`: Create a new span corresponding to a
///   shrinked version of the initial span;
/// - `_Slice(span, offset, count) -> span`: Create a new span corresponding to
///   a slice, starting from offset elements of the initial span and containing
///   count elements;
/// - `_Compare(span, span) -> int`: Three-way comparison (lexicographically) of
///   2 spans;
/// - `_[Eq, Ne, Lt, Gt, Le, Ge](span, span) -> bool`: Compare 2 spans;
///
/// Functions defined only for SPAN ONLY:
/// - `_Fill(span, byte) -> void`: Fill the span with byte;
///
/// Functions defined for VIEW ONLY:
/// - `_From_Span(span) -> view`: Create a view from given span;
/// - `_Copy(view, dest, opt, remaining) -> bool`: Copy elements of view
///   into dest. If succeeded (returned true), remaining is set to the remaining
///   space inside the destination span;
#define ATB_SPAN_VIEW_DEFINE(SPECIFIER, SPAN, VIEW, T)                    \
  ATB_SPAN_DEFINE(SPECIFIER, SPAN, T);                                    \
                                                                          \
  SPECIFIER void SPAN##_Fill(struct SPAN span, char v) {                  \
    assert(SPAN##_IsValid(span));                                         \
    memset(span.data, v, SPAN##_SizeBytes(span));                         \
  }                                                                       \
                                                                          \
  ATB_SPAN_DEFINE(SPECIFIER, VIEW, const T);                              \
                                                                          \
  SPECIFIER struct VIEW VIEW##_From_Span(struct SPAN span) {              \
    struct VIEW v;                                                        \
    v.data = span.data;                                                   \
    v.size = span.size;                                                   \
    return v;                                                             \
  }                                                                       \
                                                                          \
  SPECIFIER bool VIEW##_Copy(struct VIEW view, struct SPAN dest,          \
                             atb_View_Copy_Opt_t opt,                 \
                             struct SPAN *const remaining) {              \
    assert(VIEW##_IsValid(view));                                         \
    assert(SPAN##_IsValid(dest));                                         \
    assert(remaining != NULL);                                            \
                                                                          \
    if (opt.truncate) {                                                   \
      view = VIEW##_First(view, dest.size);                               \
    } else if (view.size > dest.size) {                                   \
      return false;                                                       \
    }                                                                     \
                                                                          \
    if (opt.overlap) {                                                    \
      memmove(dest.data, view.data, VIEW##_SizeBytes(view));              \
    } else {                                                              \
      assert(!VIEW##_IsOverlapping(view, VIEW##_From_Span(dest)));        \
      memcpy(dest.data, view.data, VIEW##_SizeBytes(view));               \
    }                                                                     \
                                                                          \
    *remaining = SPAN##_Shrink(dest, view.size, K_ATB_SPAN_SHRINK_FRONT); \
    return true;                                                          \
  }

#if defined(__cplusplus)
}
#endif
