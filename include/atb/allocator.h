#pragma once

#include <assert.h>
#include <stddef.h>

#include "atb/error.h"
#include "atb/memory/span.h"

#if defined(__cplusplus)
extern "C" {
#endif

/// Represent a generic allocator interface
struct atb_Allocator {
  void *data; /*!< Internal allocator data forwarded at each call*/

  /// Interface in charge of allocating a memory block \a mem \a n bytes
  /// Whenever \a orgi is Valid, this interface perform a re-alloc
  bool (*Alloc)(void *data, struct atb_MemSpan orig, size_t size,
                struct atb_MemSpan *const out, struct atb_Error *const err);

  /// Interface in charge of freeing/releasing a memory block
  bool (*Release)(void *data, struct atb_MemSpan mem,
                  struct atb_Error *const err);
};

/**
 *  \brief (Re)Allocate \a n bytes of memory.
 *
 *  \param[in] orig If valid, indicates that we wish to RE-allocate memory
 *  \param[in] n Number of bytes we wish to (re-)allocate
 *  \param[out] out Memory block allocated
 *  \param[out] err Error set when failure occurs
 *
 *  \return bool True whenever the allocation succeeded, false otherwsie and err
 *               is set (if not ignored) accordingly.
 *
 *  \pre self != NULL
 *  \pre self->Alloc != NULL
 *  \pre mem != NULL
 */
static inline bool atb_Allocator_Alloc(struct atb_Allocator const *const self,
                                       struct atb_MemSpan orig, size_t size,
                                       struct atb_MemSpan *const out,
                                       struct atb_Error *const err);

/**
 *  \brief Free/release the underlying memory block allocated.
 *
 *  \param[inout] mem Memory block to de-allocate. Will be invalidated when
 *                    success.
 *
 *  \return bool True whenever the operation succeeded. Otherwise false and err
 *               is set (if not ignored) accordinlgy.
 *
 *  \pre self != NULL
 *  \pre self->Free != NULL
 *  \pre mem != NULL
 */
static inline bool atb_Allocator_Release(struct atb_Allocator const *const self,
                                         struct atb_MemSpan *const mem,
                                         struct atb_Error *const err);

/*****************************************************************************/
/*                         STATIC INLINE DEFINITIONS */
/*****************************************************************************/

static inline bool atb_Allocator_Alloc(struct atb_Allocator const *const self,
                                       struct atb_MemSpan orig, size_t size,
                                       struct atb_MemSpan *const out,
                                       struct atb_Error *const err) {
  assert(self != NULL);
  assert(self->Alloc != NULL);
  assert(out != NULL);

  bool success = self->Alloc(self->data, orig, size, out, err);

  /* DEBUG Safety check */
  if (success) {
    assert(out->size == size);
  }

  return success;
}

static inline bool atb_Allocator_Release(struct atb_Allocator const *const self,
                                         struct atb_MemSpan *const mem,
                                         struct atb_Error *const err) {
  assert(self != NULL);
  assert(self->Release != NULL);
  assert(mem != NULL);

  bool success = true;

  if (atb_MemSpan_IsValid(*mem)) {
    if (self->Release(self->data, *mem, err)) {
      *mem = K_ATB_MEMSPAN_INVALID;
    } else {
      success = false;
    }
  }

  return success;
}

#if defined(__cplusplus)
}
#endif
