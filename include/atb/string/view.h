#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h> /* strlen, memcmp, memcpy, ... */

#include "atb/array.h"
#include "atb/compare.h"
#include "atb/string/span.h"

#if defined(__cplusplus)
extern "C" {
#endif

/// Non-owning view of a CONSTANT string (not meant to be modified)
struct atb_StrView {
  char const *data; /*!< Begin  of the view */
  size_t size;      /*!< Size of the view (in bytes) */
};

/* -- Ctor/Setters ***********************************************************/

/// An invalid StrView (static initializer)
#define K_ATB_STRVIEW_INVALID_INIT \
  { .data = NULL, .size = 0 }

/// An invalid StrView (compound literal)
#define K_ATB_STRVIEW_INVALID (struct atb_StrView) K_ATB_STRVIEW_INVALID_INIT

/// Construct a StrView from a string literal (static initializer)
#define atb_StrView_From_Literal_INIT(str_literal) \
  { .data = str_literal, .size = (atb_Array_Size(str_literal) - 1) }

/// Construct a StrView from a string literal (compound expresion)
#define atb_StrView_From_Literal(str_literal) \
  (struct atb_StrView) atb_StrView_From_Literal_INIT(str_literal)

/// Construct a StrView mapping to a STATIC array (static initializer)
#define atb_StrView_From_Array_INIT(char_array) \
  { .data = char_array, .size = atb_Array_Size(char_array) }

/// Construct a StrView mapping to a STATIC array (compound expression)
#define atb_StrView_From_Array(char_array) \
  (struct atb_StrView) atb_StrView_From_Array_INIT(char_array)

/// Construct a StrView from a given other/size
static inline struct atb_StrView atb_StrView_From(const char *other,
                                                  size_t size);

/// Construct a StrView from a NULL terminated string
static inline struct atb_StrView atb_StrView_From_NullTerminated(
    const char *other);

/// Construct a StrView from a StrSpan
static inline struct atb_StrView atb_StrView_From_Span(
    struct atb_StrSpan other);

/* -- Operations *************************************************************/

/// Indicates if the StrView is VALID, i.e. the ptr is not NULL
static inline bool atb_StrView_IsValid(struct atb_StrView view);

/// Options used to configure atb_StrView_CopyInto
typedef struct {
  unsigned truncate : 1; /*!< Truncate the input view to fit the dest buffer */
  unsigned overlap : 1;  /*!< Indicate that both view/dest are overlapping */
} atb_StrView_CopyInto_Opt_t;

/**
 *  \brief Copy \a view into \a dest
 *
 *  \param[in] view The string view we wish to copy
 *  \param[out] dest The string buffer destination
 *  \param[in] opt Options used when copying (truncate/overlap/...)
 *
 *  \return bool True when copying suceeded. False whenever the dest buffer is
 *               too small and truncate is False.
 *
 *  \pre atb_StrView_IsValid(view) == true
 *  \pre atb_StrSpan_IsValid(dest) == true
 */
static inline bool atb_StrView_CopyInto(struct atb_StrView view,
                                        struct atb_StrSpan dest,
                                        atb_StrView_CopyInto_Opt_t opt);

/**
 *  \brief Create a new slice from the given StrView
 *
 *  \param[in] view Initial StrView we wish to slice
 *  \param[in] offset Offset of the begin of the slice INSIDE the initial view
 *  \param[in] new_size Size of the slice
 *
 *  \return struct atb_StrView A new view corresponding to the slice
 *
 *  \important The resulting slice will NEVER goes out of bound of the initial
 *             view given, i.e. the given \a offset and \a new_size will be
 *             clamped accordingly to fit the initial view.
 *
 *  \pre atb_StrView_IsValid(view) == true
 */
static inline struct atb_StrView atb_StrView_Slice(struct atb_StrView view,
                                                   size_t offset,
                                                   size_t new_size);

/**
 *  \brief Shrink a view by the front
 *
 *  \param[in] view Initial StrView we wish to shrink
 *  \param[in] n Number of bytes removed (clamped to the initial view size)
 *
 *  \return struct atb_StrView A new view shrinked
 *
 *  \pre atb_StrView_IsValid(view) == true
 */
static inline struct atb_StrView atb_StrView_ShrinkFront(
    struct atb_StrView view, size_t n);

/**
 *  \brief Shrink a view by the back
 *
 *  \param[in] view Initial StrView we wish to shrink
 *  \param[in] n Number of bytes removed (clamped to the initial view size)
 *
 *  \return struct atb_StrView A new view shrinked
 *
 *  \pre atb_StrView_IsValid(view) == true
 */
static inline struct atb_StrView atb_StrView_ShrinkBack(struct atb_StrView view,
                                                        size_t n);

/**
 *  \brief Compare (lexicographically) 2 StrView with each other
 *
 *  \param[in] lhs, rhs StrView we wish to compare
 *
 *  \return atb_Compare_t The result of the three-way comparison, i.e. an int
 *          value that:
 *              - > 0 when lhs > rhs;
 *              - < 0 when lhs < rhs;
 *              - == 0 when lhs == rhs;
 *
 *  \pre atb_StrView_IsValid(lhs) == true
 *  \pre atb_StrView_IsValid(rhs) == true
 */
static inline atb_Cmp_t atb_StrView_Compare(struct atb_StrView lhs,
                                            struct atb_StrView rhs);

/**@{*/
/**
 * \returns bool True when \a lhs is [Eq, Ne, Lt, Gt, Le, Ge] than \a rhs.
 *
 * \param[in] lhs, rhs StrView we wish to compare
 *
 *  \pre atb_StrView_IsValid(lhs) == true
 *  \pre atb_StrView_IsValid(rhs) == true
 *
 * \note
 * \li Eq = Equals (lhs == rhs)
 * \li Ne = Not Equals (lhs != rhs)
 * \li Lt = Less than (lhs < rhs)
 * \li Gt = Greater than (lhs > rhs)
 * \li Le = Less or equals (lhs <= rhs)
 * \li Ge = Greater or equals (lhs >= rhs)
 */
ATB_CMP_DEFINE_ALL_FROM_UNSAFE_COMPARE(static inline, atb_StrView_,
                                       struct atb_StrView);
/**@}*/

/*****************************************************************************/
/*                         STATIC INLINE DEFINITIONS                         */
/*****************************************************************************/

static inline struct atb_StrView atb_StrView_From(const char *other,
                                                  size_t size) {
  struct atb_StrView view;
  view.data = other;
  view.size = size;
  return view;
}

static inline struct atb_StrView atb_StrView_From_NullTerminated(
    const char *other) {
  return atb_StrView_From(other, strlen(other));
}

static inline struct atb_StrView atb_StrView_From_Span(
    struct atb_StrSpan other) {
  return atb_StrView_From(other.data, other.size);
}

static inline bool atb_StrView_IsValid(struct atb_StrView view) {
  return view.data != NULL;
}

static inline struct atb_StrView atb_StrView_Slice(struct atb_StrView view,
                                                   size_t begin,
                                                   size_t new_size) {
  assert(atb_StrView_IsValid(view));

  view = atb_StrView_ShrinkFront(view, begin);
  view.size = (new_size < view.size ? new_size : view.size);

  return view;
}

static inline struct atb_StrView atb_StrView_ShrinkFront(
    struct atb_StrView view, size_t n) {
  assert(atb_StrView_IsValid(view));

  n = (n < view.size ? n : view.size);

  view.data += n;
  view.size -= n;

  return view;
}

static inline struct atb_StrView atb_StrView_ShrinkBack(struct atb_StrView view,
                                                        size_t n) {
  assert(atb_StrView_IsValid(view));

  n = (n < view.size ? n : view.size);
  view.size -= n;

  return view;
}

static inline bool atb_StrView_CopyInto(struct atb_StrView view,
                                        struct atb_StrSpan dest,
                                        atb_StrView_CopyInto_Opt_t opt) {
  assert(atb_StrView_IsValid(view));
  assert(atb_StrSpan_IsValid(dest));

  if (opt.truncate) {
    view = atb_StrView_Slice(view, 0, dest.size);
  } else if (view.size > dest.size) {
    return false;
  }

  if (opt.overlap) {
    memmove(dest.data, view.data, view.size);
  } else {
    memcpy(dest.data, view.data, view.size);
  }

  return true;
}

static inline atb_Cmp_t atb_StrView_Compare(struct atb_StrView lhs,
                                            struct atb_StrView rhs) {
  assert(atb_StrView_IsValid(lhs));
  assert(atb_StrView_IsValid(rhs));

  if (lhs.size > rhs.size) {
    return K_ATB_CMP_GREATER;
  } else if (lhs.size < rhs.size) {
    return K_ATB_CMP_LESS;
  } else {
    return (atb_Cmp_t)(memcmp(lhs.data, rhs.data, lhs.size));
  }
}

#if defined(__cplusplus)
} /* extern "C" */
#endif
