#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "atb/array.h"
#include "atb/compare.h"
#include "atb/export.h"

#if defined(__cplusplus)
extern "C" {
#endif

/* Format string helper ******************************************************/

#define K_ATB_STR_FMT_RAW "{.data=%p, .size=%zu}"
#define atb_Str_FMT_RAW_VA_ARG(str) (str).data, (str).size

#define K_ATB_STR_FMT "%.*s"
#define atb_Str_FMT_VA_ARG(str) (int)(str).size, (str).data

/// Non-owing view of a MODIFIABLE string (meant to be modified)
struct atb_StrSpan {
  char *data;
  size_t size;
};

/// An invalid StrSpan (usable as static initializer)
#define K_ATB_STRSPAN_INVALID_INIT \
  { .data = NULL, .size = 0 }

/// An invalid StrSpan (compound literal)
#define K_ATB_STRSPAN_INVALID (struct atb_StrSpan) K_ATB_STRSPAN_INVALID_INIT

/* StrSpan *******************************************************************/

/* -- Ctor/Setters ***********************************************************/

/// Construct a StrSpan mapping to a STATIC array (usable as STATIC initializer)
#define atb_StrSpan_From_Array_INIT(char_array) \
  { .data = char_array, .size = atb_Array_Size(char_array) }

/// Construct a StrSpan mapping to a STATIC array (compound expression)
#define atb_StrSpan_From_Array(char_array) \
  (struct atb_StrSpan) atb_StrSpan_From_Array_INIT(char_array)

/// Construct a StrSpan from a given other/size
static inline struct atb_StrSpan atb_StrSpan_From(char *other, size_t size);

/// Construct a StrSpan from a NULL terminated string
extern struct atb_StrSpan atb_StrSpan_From_NullTerminated(char *other)
    ATB_PUBLIC;

/* -- Operations *************************************************************/

/// Indicates if the StrSpan is VALID, i.e. the ptr is not NULL
static inline bool atb_StrSpan_IsValid(struct atb_StrSpan span);

/**
 *  \brief Create a new slice from the given StrSpan
 *
 *  \param[in] span Initial StrSpan we wish to slice
 *  \param[in] offset Offset of the begin of the slice INSIDE the initial span
 *  \param[in] new_size Size of the slice
 *
 *  \return struct atb_StrSpan A new span corresponding to the slice
 *
 *  \important The resulting slice will NEVER goes out of bound of the initial
 *             span given, i.e. the given \a offset and \a new_size will be
 *             clamped accordingly to fit the initial span.
 *
 *  \pre atb_StrSpan_IsValid(span) == true
 */
extern struct atb_StrSpan atb_StrSpan_Slice(struct atb_StrSpan span,
                                            size_t offset,
                                            size_t new_size) ATB_PUBLIC;

/**
 *  \brief Shrink a span by the front
 *
 *  \param[in] span Initial StrSpan we wish to shrink
 *  \param[in] n Number of bytes removed (clamped to the initial span size)
 *
 *  \return struct atb_StrSpan A new span shrinked
 *
 *  \pre atb_StrSpan_IsValid(span) == true
 */
inline static struct atb_StrSpan atb_StrSpan_ShrinkFront(
    struct atb_StrSpan span, size_t n);

/**
 *  \brief Shrink a span by the back
 *
 *  \param[in] span Initial StrSpan we wish to shrink
 *  \param[in] n Number of bytes removed (clamped to the initial span size)
 *
 *  \return struct atb_StrSpan A new span shrinked
 *
 *  \pre atb_StrSpan_IsValid(span) == true
 */
inline static struct atb_StrSpan atb_StrSpan_ShrinkBack(struct atb_StrSpan span,
                                                        size_t n);

/* StrView *******************************************************************/

/// Non-owing view of a CONSTANT string (not meant to be modified)
struct atb_StrView {
  char const *data;
  size_t size;
};

/* -- Ctor/Setters ***********************************************************/

/// An invalid StrView (usable as static initializer)
#define K_ATB_STRVIEW_INVALID_INIT \
  { .data = NULL, .size = 0 }

/// An invalid StrView (compound literal)
#define K_ATB_STRVIEW_INVALID (struct atb_StrView) K_ATB_STRVIEW_INVALID_INIT

/// Construct a StrView from a string literal (usable as STATIC initializer)
#define atb_StrView_From_Literal_INIT(str_literal) \
  { .data = str_literal, .size = (atb_Array_Size(str_literal) - 1) }

/// Construct a StrView from a string literal (compound expresion)
#define atb_StrView_From_Literal(str_literal) \
  (struct atb_StrView) atb_StrView_From_Literal_INIT(str_literal)

/// Construct a StrView from a given other/size
static inline struct atb_StrView atb_StrView_From(const char *other,
                                                  size_t size);

/// Construct a StrView from a NULL terminated string
extern struct atb_StrView atb_StrView_From_NullTerminated(const char *other)
    ATB_PUBLIC;

/// Construct a StrView from a StrSpan
static inline struct atb_StrView atb_StrView_From_Span(
    struct atb_StrSpan other);

/* -- Operations *************************************************************/

/// Indicates if the StrView is VALID, i.e. the ptr is not NULL
static inline bool atb_StrView_IsValid(struct atb_StrView view);

/// Options used to configure atb_StrView_CopyInto
typedef struct atb_StrView_CopyInto_Opt {
  unsigned truncate : 1; /*!< Whenever we want to truncate the input view to fit
                            the dest buffer */
  unsigned overlap : 1;  /*!< Whenever both view/dest are overlapping range */
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
extern bool atb_StrView_CopyInto(struct atb_StrView view,
                                 struct atb_StrSpan dest,
                                 atb_StrView_CopyInto_Opt_t opt) ATB_PUBLIC;

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
extern struct atb_StrView atb_StrView_Slice(struct atb_StrView view,
                                            size_t offset,
                                            size_t new_size) ATB_PUBLIC;

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
inline static struct atb_StrView atb_StrView_ShrinkFront(
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
inline static struct atb_StrView atb_StrView_ShrinkBack(struct atb_StrView view,
                                                        size_t n);

/**
 *  \brief Compare (lexicographically) 2 StrView with each other
 *
 *  \param[in] lhs, rhs StrView we wish to compare
 *
 *  \return int The result of the three-way comparison, i.e. an int value that:
 *              - > 0 when lhs > rhs;
 *              - < 0 when lhs < rhs;
 *              - == 0 when lhs == rhs;
 *
 *  \pre atb_StrView_IsValid(lhs) == true
 *  \pre atb_StrView_IsValid(rhs) == true
 */
extern int atb_StrView_Compare(struct atb_StrView lhs,
                               struct atb_StrView rhs) ATB_PUBLIC;

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
/*                         STATIC INLINE DEFINITIONS */
/*****************************************************************************/

/* StrSpan *****************************************************************/

static inline struct atb_StrSpan atb_StrSpan_From(char *other, size_t size) {
  struct atb_StrSpan span;
  span.data = other;
  span.size = size;
  return span;
}

static inline bool atb_StrSpan_IsValid(struct atb_StrSpan span) {
  return span.data != NULL;
}

inline static struct atb_StrSpan atb_StrSpan_ShrinkFront(
    struct atb_StrSpan span, size_t n) {
  return atb_StrSpan_Slice(span, n, span.size - n);
}

inline static struct atb_StrSpan atb_StrSpan_ShrinkBack(struct atb_StrSpan span,
                                                        size_t n) {
  return atb_StrSpan_Slice(span, 0, span.size <= n ? 0 : (span.size - n));
}

/* StrView *******************************************************************/

static inline struct atb_StrView atb_StrView_From(const char *other,
                                                  size_t size) {
  struct atb_StrView view;
  view.data = other;
  view.size = size;
  return view;
}

static inline struct atb_StrView atb_StrView_From_Span(
    struct atb_StrSpan other) {
  return atb_StrView_From(other.data, other.size);
}

static inline bool atb_StrView_IsValid(struct atb_StrView view) {
  return view.data != NULL;
}

inline static struct atb_StrView atb_StrView_ShrinkFront(
    struct atb_StrView view, size_t n) {
  return atb_StrView_Slice(view, n, view.size - n);
}

inline static struct atb_StrView atb_StrView_ShrinkBack(struct atb_StrView view,
                                                        size_t n) {
  return atb_StrView_Slice(view, 0, view.size < n ? 0 : (view.size - n));
}

#if defined(__cplusplus)
} /* extern "C" */
#endif
