#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h> /* strlen */

#include "atb/array.h"

#if defined(__cplusplus)
extern "C" {
#endif

/// Non-owning view of a MODIFIABLE string (meant to be modified)
struct atb_StrSpan {
  char *data;  /*!< Begin of the span */
  size_t size; /*!< Size of the span (in bytes) */
};

/// An invalid StrSpan (static initializer)
#define K_ATB_STRSPAN_INVALID_INIT \
  { .data = NULL, .size = 0 }

/// An invalid StrSpan (compound literal)
#define K_ATB_STRSPAN_INVALID (struct atb_StrSpan) K_ATB_STRSPAN_INVALID_INIT

/* -- Ctor/Setters ***********************************************************/

/// Construct a StrSpan mapping to a STATIC array (static initializer)
#define atb_StrSpan_From_Array_INIT(char_array) \
  { .data = char_array, .size = atb_Array_Size(char_array) }

/// Construct a StrSpan mapping to a STATIC array (compound expression)
#define atb_StrSpan_From_Array(char_array) \
  (struct atb_StrSpan) atb_StrSpan_From_Array_INIT(char_array)

/// Construct a StrSpan from a given other/size
static inline struct atb_StrSpan atb_StrSpan_From(char *other, size_t size);

/// Construct a StrSpan from a NULL terminated string
static inline struct atb_StrSpan atb_StrSpan_From_NullTerminated(char *other);

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
static inline struct atb_StrSpan atb_StrSpan_Slice(struct atb_StrSpan span,
                                                   size_t offset,
                                                   size_t new_size);

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
static inline struct atb_StrSpan atb_StrSpan_ShrinkFront(
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
static inline struct atb_StrSpan atb_StrSpan_ShrinkBack(struct atb_StrSpan span,
                                                        size_t n);

/*****************************************************************************/
/*                         STATIC INLINE DEFINITIONS                         */
/*****************************************************************************/

static inline struct atb_StrSpan atb_StrSpan_From(char *other, size_t size) {
  struct atb_StrSpan span;
  span.data = other;
  span.size = size;
  return span;
}

static inline struct atb_StrSpan atb_StrSpan_From_NullTerminated(char *other) {
  return atb_StrSpan_From(other, strlen(other));
}

static inline bool atb_StrSpan_IsValid(struct atb_StrSpan span) {
  return span.data != NULL;
}

static inline struct atb_StrSpan atb_StrSpan_Slice(struct atb_StrSpan span,
                                                   size_t begin,
                                                   size_t new_size) {
  assert(atb_StrSpan_IsValid(span));

  span = atb_StrSpan_ShrinkFront(span, begin);
  span.size = (new_size < span.size ? new_size : span.size);

  return span;
}

static inline struct atb_StrSpan atb_StrSpan_ShrinkFront(
    struct atb_StrSpan span, size_t n) {
  assert(atb_StrSpan_IsValid(span));

  n = (n < span.size ? n : span.size);

  span.data += n;
  span.size -= n;

  return span;
}

static inline struct atb_StrSpan atb_StrSpan_ShrinkBack(struct atb_StrSpan span,
                                                        size_t n) {
  assert(atb_StrSpan_IsValid(span));

  n = (n < span.size ? n : span.size);
  span.size -= n;

  return span;
}

#if defined(__cplusplus)
} /* extern "C" */
#endif
