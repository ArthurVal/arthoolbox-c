#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#if defined(__cplusplus)
extern "C" {
#endif

/// Represent a NON-OWNING MODIFIABLE memory (agnostic of the underlying type)
/// range of \a size BYTES
struct atb_MemSpan {
  void *data;  /*!< Begin of the memory range */
  size_t size; /*!< Number of bytes of the span  */
};

/// An invalid MemSpan (static initializer)
#define K_ATB_MEMSPAN_INVALID_INIT \
  { .data = NULL, .size = 0 }

/// An invalid MemSpan (compound literal)
#define K_ATB_MEMSPAN_INVALID (struct atb_MemSpan) K_ATB_MEMSPAN_INVALID_INIT

/// Indicates if the MemSpan is VALID, i.e. data is not NULL
static inline bool atb_MemSpan_IsValid(struct atb_MemSpan span);

/* -- Ctor/Setters ***********************************************************/

/// Construct a MemSpan mapping to a STATIC array (static initializer)
#define atb_MemSpan_From_Array_INIT(array) \
  { .data = array, .size = sizeof(array), }

/// Construct a MemSpan mapping to a STATIC array (compound literal)
#define atb_MemSpan_From_Array(array) \
  (struct atb_MemSpan) atb_MemSpan_From_Array_INIT(array)

/// Construct a MemSpan mapping to a PTR (static initializer)
#define atb_MemSpan_From_Ptr_INIT(ptr) \
  { .data = ptr, .size = sizeof(*ptr), }

/// Construct a MemSpan mapping to a PTR (compound literal)
#define atb_MemSpan_From_Ptr(ptr) \
  (struct atb_MemSpan) atb_MemSpan_From_Ptr_INIT(ptr)

/// Construct a MemSpan mapping to a LVALUE (static initializer)
#define atb_MemSpan_From_Value_INIT(v) \
  { .data = &v, .size = sizeof(v), }

/// Construct a MemSpan mapping to a LVALUE (compound literal)
#define atb_MemSpan_From_Value(v) \
  (struct atb_MemSpan) atb_MemSpan_From_Value_INIT(v)

/// Construct a MemSpan from a given other/size
static inline struct atb_MemSpan atb_MemSpan_From(void *other, size_t size);

/* -- Operations *************************************************************/

/**
 *  \brief Create a new slice from the given MemSpan
 *
 *  \param[in] span Initial MemSpan we wish to slice
 *  \param[in] begin Offset in BYTES from the begin of the slice INSIDE the
 *                    initial \a span
 *  \param[in] new_size Size of the slice in BYTES
 *
 *  \return struct atb_MemSpan A new span corresponding to the slice
 *
 *  \important The resulting slice will NEVER goes out of bound of the initial
 *             span given, i.e. the given \a offset and \a new_size will be
 *             clamped accordingly to fit the initial span.
 *
 *  \pre atb_MemSpan_IsValid(span) == true
 */
static inline struct atb_MemSpan atb_MemSpan_Slice(struct atb_MemSpan span,
                                                   size_t begin,
                                                   size_t new_size);

/**
 *  \brief Shrink a span by the front
 *
 *  \param[in] span Initial MemSpan we wish to shrink
 *  \param[in] n Number of bytes removed (clamped to the initial span size)
 *
 *  \return struct atb_MemSpan A new span shrinked
 *
 *  \pre atb_MemSpan_IsValid(span) == true
 */
static inline struct atb_MemSpan atb_MemSpan_ShrinkFront(
    struct atb_MemSpan span, size_t n);

/**
 *  \brief Shrink a span by the back
 *
 *  \param[in] span Initial MemSpan we wish to shrink
 *  \param[in] n Number of bytes removed (clamped to the initial span size)
 *
 *  \return struct atb_MemSpan A new span shrinked
 *
 *  \pre atb_MemSpan_IsValid(span) == true
 */
static inline struct atb_MemSpan atb_MemSpan_ShrinkBack(struct atb_MemSpan span,
                                                        size_t n);

/**
 *  \brief Fill a span with the byte
 *
 *  \param[in] span MemSpan we wish to fill
 *  \param[in] value Byte used to fill the span
 *
 *  \pre atb_MemSpan_IsValid(span) == true
 */
static inline void atb_MemSpan_Fill(struct atb_MemSpan span, uint8_t value);

/*****************************************************************************/
/*                         STATIC INLINE DEFINITIONS */
/*****************************************************************************/

static inline bool atb_MemSpan_IsValid(struct atb_MemSpan span) {
  return span.data != NULL;
}

static inline struct atb_MemSpan atb_MemSpan_From(void *other, size_t size) {
  struct atb_MemSpan span;
  span.data = other;
  span.size = size;
  return span;
}

static inline struct atb_MemSpan atb_MemSpan_Slice(struct atb_MemSpan span,
                                                   size_t begin,
                                                   size_t new_size) {
  assert(atb_MemSpan_IsValid(span));

  span = atb_MemSpan_ShrinkFront(span, begin);
  span.size = (new_size < span.size ? new_size : span.size);

  return span;
}

static inline struct atb_MemSpan atb_MemSpan_ShrinkFront(
    struct atb_MemSpan span, size_t n) {
  assert(atb_MemSpan_IsValid(span));

  n = (n < span.size ? n : span.size);

  span.data = ((uint8_t *)span.data + n);
  span.size -= n;

  return span;
}

static inline struct atb_MemSpan atb_MemSpan_ShrinkBack(struct atb_MemSpan span,
                                                        size_t n) {
  assert(atb_MemSpan_IsValid(span));

  n = (n < span.size ? n : span.size);
  span.size -= n;

  return span;
}

static inline void atb_MemSpan_Fill(struct atb_MemSpan span, uint8_t value) {
  assert(atb_MemSpan_IsValid(span));
  memset(span.data, value, span.size);
}

#if defined(__cplusplus)
}
#endif
