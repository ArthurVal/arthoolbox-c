#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stddef.h> /* NULL */

#include "atb/export.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 *  \brief Represents a memory block view (constant data)
 */
struct atb_MemView {
  const void *data; /*!< Begin mem addr of the view */
  size_t size;      /*!< Size in bytes of the view */
};

typedef struct atb_MemView atb_MemView;

#define atb_MemView_FMT "{.data=%p, .size=%zu}"
#define atb_MemView_FMT_VA_ARG(mem) ((mem).data), ((mem).size)

/***************************************************************************/
/*                                Construct                                */
/***************************************************************************/

/**
 *  \brief Initialize the MemView data struct (NULL)
 */
static inline void atb_MemView_Init(struct atb_MemView *const self);

/**
 *  \return An empty memory view
 */
static inline struct atb_MemView atb_MemView_MakeEmpty();

/**
 *  \return A memory view that maps an entire static C array
 */
#define atb_MemView_MakeFromStaticArray(arr) \
  atb_MemView { (arr), sizeof((arr)) }

/***************************************************************************/
/*                                Operations                               */
/***************************************************************************/
/**
 *  \return The size of \a mem for a given type \a T
 */
#define atb_MemView_SizeAs(T, mem) ((mem).size / (sizeof(T)))

/* Iterators ***************************************************************/

// Forward
/**
 *  \return A const pointer to the first valid element of \a mem as \a T
 */
#define atb_MemView_BeginAs(T, mem) ((const T *)(mem).data)

/**
 *  \return A const pointer ONE PAST \a mem, as \a T
 *
 *  \note This ptr is not dereferenceable
 */
#define atb_MemView_EndAs(T, mem) \
  (atb_MemView_BeginAs(T, mem) + atb_MemView_SizeAs(T, mem))

/**
 *  \brief Forward iterate over the memory view
 *
 *  \param[in] T The expected type of the underlying mem block (not const)
 *  \param[inout] elem An iterator of type 'const T*'
 *  \param[in] mem The mem view we wish to iterator over
 */
#define atb_MemView_ForEachAs(T, elem, mem)  \
  for ((elem) = atb_MemView_BeginAs(T, mem); \
       (elem) != atb_MemView_EndAs(T, mem); ++(elem))

// Backward
/**
 *  \return A const pointer to the last valid element of \a mem as \a T
 */
#define atb_MemView_BeginAsR(T, mem) (atb_MemView_EndAs(T, mem) - 1)

/**
 *  \return A const pointer ONE BEFORE \a mem, as \a T
 *
 *  \note This ptr is not dereferenceable
 */
#define atb_MemView_EndAsR(T, mem) \
  (atb_MemView_BeginAsR(T, mem) - atb_MemView_SizeAs(T, mem))

/**
 *  \brief Backward iterate over the memory view
 *
 *  \param[in] T The expected type of the underlying mem block (not const)
 *  \param[inout] elem An iterator of type 'const T*'
 *  \param[in] mem The mem view we wish to iterator over
 */
#define atb_MemView_ForEachAsR(T, elem, mem)  \
  for ((elem) = atb_MemView_BeginAsR(T, mem); \
       (elem) != atb_MemView_EndAsR(T, mem); --(elem))

/* Checks ******************************************************************/

/**
 *  \return True if the given MemView is invalid (nullptr and size > 0)
 */
static inline bool atb_MemView_IsInvalid(struct atb_MemView mem);

/* Comparisons *************************************************************/

/**
 *  \return True if the start of lhs is strictly lower than rhs
 *
 *  \note Asserts on invalid mem view
 */
static inline bool atb_MemView_IsLower(struct atb_MemView lhs,
                                       struct atb_MemView rhs);

/**
 *  \return True if the start of lhs is lower or equal than rhs
 *
 *  \note Asserts on invalid mem view
 */
static inline bool atb_MemView_IsLowerOrEqual(struct atb_MemView lhs,
                                              struct atb_MemView rhs);

/**
 *  \return True if the start of lhs is strictly higher than rhs
 *
 *  \note Asserts on invalid mem view
 */
static inline bool atb_MemView_IsHigher(struct atb_MemView lhs,
                                        struct atb_MemView rhs);

/**
 *  \return True if the start of lhs is higher or equal than rhs
 *
 *  \note Asserts on invalid mem view
 */
static inline bool atb_MemView_IsHigherOrEqual(struct atb_MemView lhs,
                                               struct atb_MemView rhs);

/**
 *  \brief Simple pair of 2 mem blocks
 */
struct atb_MemViewPair {
  struct atb_MemView first;
  struct atb_MemView second;
};

/**
 *  \return [lhs, rhs] if \a predicate(lhs, rhs) returns true, else [rhs, lhs]
 *
 *  \note Asserts on invalid mem view
 *
 *  \param[in] predicate A predicate function use to sort lhs/rhs
 *  \param[in] lhs, rhs Mem view to compare
 */
static inline struct atb_MemViewPair atb_MemView_Sort(
    bool (*predicate)(struct atb_MemView, struct atb_MemView),
    struct atb_MemView lhs, struct atb_MemView rhs);

/**
 *  \return True if the given views overlap each other memory-wise
 *
 *  \note Asserts on invalid mem view
 */
ATB_PUBLIC bool atb_MemView_IsOverlapping(struct atb_MemView lhs,
                                          struct atb_MemView rhs);

/**
 *  \brief Merge 2 OVERLAPPING memory view into one
 *
 *  \note Asserts on invalid mem views and when lhs/rhs are not overlapping
 *
 *  \param[in] lhs, rhs Mem view to merge
 *
 *  \return atb_MemView of merge operation result
 */
ATB_PUBLIC struct atb_MemView atb_MemView_Merge(struct atb_MemView lhs,
                                                struct atb_MemView rhs);

/***************************************************************************/
/*                            Static definitions                           */
/***************************************************************************/
static inline void atb_MemView_Init(struct atb_MemView *const self) {
  self->data = NULL;
  self->size = 0;
}

static inline struct atb_MemView atb_MemView_MakeEmpty() {
  struct atb_MemView out;
  atb_MemView_Init(&out);
  return out;
}

static inline bool atb_MemView_IsInvalid(struct atb_MemView mem) {
  return (mem.data == NULL) && (mem.size > 0);
}

static inline bool atb_MemView_IsLower(struct atb_MemView lhs,
                                       struct atb_MemView rhs) {
  assert(!atb_MemView_IsInvalid(lhs));
  assert(!atb_MemView_IsInvalid(rhs));

  return atb_MemView_BeginAs(char, lhs) < atb_MemView_BeginAs(char, rhs);
}

static inline bool atb_MemView_IsHigher(struct atb_MemView lhs,
                                        struct atb_MemView rhs) {
  assert(!atb_MemView_IsInvalid(lhs));
  assert(!atb_MemView_IsInvalid(rhs));

  return atb_MemView_IsLower(rhs, lhs);
}

static inline bool atb_MemView_IsLowerOrEqual(struct atb_MemView lhs,
                                              struct atb_MemView rhs) {
  assert(!atb_MemView_IsInvalid(lhs));
  assert(!atb_MemView_IsInvalid(rhs));

  return !atb_MemView_IsHigher(lhs, rhs);
}

static inline bool atb_MemView_IsHigherOrEqual(struct atb_MemView lhs,
                                               struct atb_MemView rhs) {
  assert(!atb_MemView_IsInvalid(lhs));
  assert(!atb_MemView_IsInvalid(rhs));

  return !atb_MemView_IsLower(lhs, rhs);
}

static inline struct atb_MemViewPair atb_MemView_Sort(
    bool (*predicate)(struct atb_MemView, struct atb_MemView),
    struct atb_MemView lhs, struct atb_MemView rhs) {
  assert(predicate != NULL);
  assert(!atb_MemView_IsInvalid(lhs));
  assert(!atb_MemView_IsInvalid(rhs));

  struct atb_MemViewPair out;

  if (predicate(lhs, rhs)) {
    out.first = lhs;
    out.second = rhs;
  } else {
    out.first = rhs;
    out.second = lhs;
  }

  return out;
}

#if defined(__cplusplus)
} /* extern "C" */
#endif
