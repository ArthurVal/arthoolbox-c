#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "atb/compare.h"
#include "atb/memory/span.h"

#if defined(__cplusplus)
extern "C" {
#endif

/// Represent a NON-OWNING CONSTANT memory (agnostic of the underlying type)
/// range of \a size BYTES
struct atb_MemView {
  void const *data; /*!< Begin of the memory range */
  size_t size;      /*!< Number of bytes of the view  */
};

/// An invalid MemView (static initializer)
#define K_ATB_MEMVIEW_INVALID_INIT \
  { .data = NULL, .size = 0 }

/// An invalid MemView (compound literal)
#define K_ATB_MEMVIEW_INVALID (struct atb_MemView) K_ATB_MEMVIEW_INVALID_INIT

/* -- Ctor/Setters ***********************************************************/

/// Construct a MemView mapping to a STATIC array (static initializer)
#define atb_MemView_From_Array_INIT(array) \
  { .data = array, .size = sizeof(array), }

/// Construct a MemView mapping to a STATIC array (compound literal)
#define atb_MemView_From_Array(array) \
  (struct atb_MemView) atb_MemView_From_Array_INIT(array)

/// Construct a MemView mapping to a PTR (static initializer)
#define atb_MemView_From_Ptr_INIT(ptr) \
  { .data = ptr, .size = sizeof(*ptr), }

/// Construct a MemView mapping to a PTR (compound literal)
#define atb_MemView_From_Ptr(ptr) \
  (struct atb_MemView) atb_MemView_From_Ptr_INIT(ptr)

/// Construct a MemView mapping to a LVALUE (static initializer)
#define atb_MemView_From_Value_INIT(v) \
  { .data = &v, .size = sizeof(v), }

/// Construct a MemView mapping to a LVALUE (compound literal)
#define atb_MemView_From_Value(v) \
  (struct atb_MemView) atb_MemView_From_Value_INIT(v)

/// Construct a MemView from a given other/size
static inline struct atb_MemView atb_MemView_From(void const *other,
                                                  size_t size);

/// Construct a MemView from an other MemSpan
static inline struct atb_MemView atb_MemView_From_Span(struct atb_MemSpan span);

/* -- Operations *************************************************************/

/// Indicates if the MemView is VALID, i.e. data is not NULL
static inline bool atb_MemView_IsValid(struct atb_MemView view);

/**
 *  \brief Indicates if 2 memory blocks overlaps
 *
 *  \param[in] lhs, rhs MemView we wish to check for overlaps
 *
 *  \return bool True whenever lhs and rhs are overlapping block of memory.
 *
 *  \pre atb_MemView_IsValid(lhs) == true
 *  \pre atb_MemView_IsValid(rhs) == true
 */
static inline bool atb_MemView_IsOverlapping(struct atb_MemView lhs,
                                             struct atb_MemView rhs);

/**
 *  \brief Create a new slice from the given MemView
 *
 *  \param[in] view Initial MemView we wish to slice
 *  \param[in] begin Offset in BYTES from the begin of the slice INSIDE the
 *                    initial \a view
 *  \param[in] new_size Size of the slice in BYTES
 *
 *  \return struct atb_MemView A new view corresponding to the slice
 *
 *  \important The resulting slice will NEVER goes out of bound of the initial
 *             view given, i.e. the given \a offset and \a new_size will be
 *             clamped accordingly to fit the initial view.
 *
 *  \pre atb_MemView_IsValid(view) == true
 */
static inline struct atb_MemView atb_MemView_Slice(struct atb_MemView view,
                                                   size_t begin,
                                                   size_t new_size);

/**
 *  \brief Shrink a view by the front
 *
 *  \param[in] view Initial MemView we wish to shrink
 *  \param[in] n Number of bytes removed (clamped to the initial view size)
 *
 *  \return struct atb_MemView A new view shrinked
 *
 *  \pre atb_MemView_IsValid(view) == true
 */
static inline struct atb_MemView atb_MemView_ShrinkFront(
    struct atb_MemView view, size_t n);

/**
 *  \brief Shrink a view by the back
 *
 *  \param[in] view Initial MemView we wish to shrink
 *  \param[in] n Number of bytes removed (clamped to the initial view size)
 *
 *  \return struct atb_MemView A new view shrinked
 *
 *  \pre atb_MemView_IsValid(view) == true
 */
static inline struct atb_MemView atb_MemView_ShrinkBack(struct atb_MemView view,
                                                        size_t n);

/// Options used to configure atb_StrView_CopyInto
typedef struct {
  unsigned truncate : 1; /*!< Truncate the input view to fit the dest buffer */
  unsigned overlap : 1;  /*!< Indicate that both view/dest are overlapping */
} atb_MemView_CopyInto_Opt_t;

/**
 *  \brief Copy \a view into \a dest
 *
 *  \param[in] view The mem view we wish to copy
 *  \param[in] dest The mem span buffer destination
 *  \param[in] opt Options used when copying (truncate/overlap/...)
 *
 *  \return atb_MemSpan K_ATB_MEMSPAN_INVALID in case of failure. Otherwise,
 *                      dest buffer shrinked by the number of byte written
 *                      (i.e. remaining space in the dest buffer).
 *
 *  \pre atb_MemView_IsValid(view) == true
 *  \pre atb_MemSpan_IsValid(dest) == true
 *  \pre When overlap is true, atb_MemSpan_IsOverlapping(view, dest) == false
 */
static inline struct atb_MemSpan atb_MemView_CopyInto(
    struct atb_MemView view, struct atb_MemSpan dest,
    atb_MemView_CopyInto_Opt_t opt);

/**
 *  \brief Compare (lexicographically) 2 MemView with each other
 *
 *  \param[in] lhs, rhs MemView we wish to compare
 *
 *  \return atb_Cmp_t The result of the three-way comparison, i.e. an int
 *          value that:
 *              - > 0 when lhs > rhs;
 *              - < 0 when lhs < rhs;
 *              - == 0 when lhs == rhs;
 *
 *  \pre atb_MemView_IsValid(lhs) == true
 *  \pre atb_MemView_IsValid(rhs) == true
 */
static inline atb_Cmp_t atb_MemView_Compare(struct atb_MemView lhs,
                                            struct atb_MemView rhs);

/**@{*/
/**
 *  \returns bool True when \a lhs is [Eq, Ne, Lt, Gt, Le, Ge] than \a rhs.
 *
 *  \param[in] lhs, rhs MemView we wish to compare
 *
 *  \pre atb_MemView_IsValid(lhs) == true
 *  \pre atb_MemView_IsValid(rhs) == true
 *
 *  \note
 *  \li Eq = Equals (lhs == rhs)
 *  \li Ne = Not Equals (lhs != rhs)
 *  \li Lt = Less than (lhs < rhs)
 *  \li Gt = Greater than (lhs > rhs)
 *  \li Le = Less or equals (lhs <= rhs)
 *  \li Ge = Greater or equals (lhs >= rhs)
 */
ATB_CMP_DEFINE_ALL_FROM_UNSAFE_COMPARE(static inline, atb_MemView_,
                                       struct atb_MemView);
/**@}*/

/*****************************************************************************/
/*                         STATIC INLINE DEFINITIONS                         */
/*****************************************************************************/

static inline struct atb_MemView atb_MemView_From(void const *other,
                                                  size_t size) {
  struct atb_MemView view;
  view.data = other;
  view.size = size;
  return view;
}

static inline struct atb_MemView atb_MemView_From_Span(
    struct atb_MemSpan span) {
  return atb_MemView_From(span.data, span.size);
}

static inline bool atb_MemView_IsValid(struct atb_MemView view) {
  return view.data != NULL;
}

static inline bool atb_MemView_IsOverlapping(struct atb_MemView lhs,
                                             struct atb_MemView rhs) {
  assert(atb_MemView_IsValid(lhs));
  assert(atb_MemView_IsValid(rhs));

  struct atb_MemView const *hi = NULL;
  struct atb_MemView const *lo = NULL;

  if (lhs.data >= rhs.data) {
    hi = &lhs;
    lo = &rhs;
  } else {
    hi = &rhs;
    lo = &lhs;
  }

  /*
   * We have the following diagram:
   *
   *                                end of lo
   *                                   |
   *                                   v
   * lo:      [------------------------[
   * hi:                          [-----------[
   *                              ^
   *                              |
   *                        begin of hi
   *
   * Since End is not part of the memory view (one past the valid range) we dont
   * check for equality (use > vs >=)
   */
  return (((uint8_t *)lo->data + lo->size) > ((uint8_t *)hi->data));
}

static inline struct atb_MemView atb_MemView_Slice(struct atb_MemView view,
                                                   size_t begin,
                                                   size_t new_size) {
  assert(atb_MemView_IsValid(view));

  view = atb_MemView_ShrinkFront(view, begin);
  view.size = (new_size < view.size ? new_size : view.size);

  return view;
}

static inline struct atb_MemView atb_MemView_ShrinkFront(
    struct atb_MemView view, size_t n) {
  assert(atb_MemView_IsValid(view));

  n = (n < view.size ? n : view.size);

  view.data = ((uint8_t *)view.data + n);
  view.size -= n;

  return view;
}

static inline struct atb_MemView atb_MemView_ShrinkBack(struct atb_MemView view,
                                                        size_t n) {
  assert(atb_MemView_IsValid(view));

  n = (n < view.size ? n : view.size);
  view.size -= n;

  return view;
}

static inline struct atb_MemSpan atb_MemView_CopyInto(
    struct atb_MemView view, struct atb_MemSpan dest,
    atb_MemView_CopyInto_Opt_t opt) {
  assert(atb_MemView_IsValid(view));
  assert(atb_MemSpan_IsValid(dest));

  if (opt.truncate) {
    view = atb_MemView_Slice(view, 0, dest.size);
  } else if (view.size > dest.size) {
    return K_ATB_MEMSPAN_INVALID;
  }

  if (opt.overlap) {
    memmove(dest.data, view.data, view.size);
  } else {
    assert(!atb_MemView_IsOverlapping(view, atb_MemView_From_Span(dest)));
    memcpy(dest.data, view.data, view.size);
  }

  return atb_MemSpan_ShrinkFront(dest, view.size);
}

static inline atb_Cmp_t atb_MemView_Compare(struct atb_MemView lhs,
                                            struct atb_MemView rhs) {
  assert(atb_MemView_IsValid(lhs));
  assert(atb_MemView_IsValid(rhs));

  if (lhs.size > rhs.size) {
    return K_ATB_CMP_GREATER;
  } else if (lhs.size < rhs.size) {
    return K_ATB_CMP_LESS;
  } else {
    return (atb_Cmp_t)(memcmp(lhs.data, rhs.data, lhs.size));
  }
}

#if defined(__cplusplus)
}
#endif
