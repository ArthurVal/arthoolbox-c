#pragma once

#include <assert.h>
#include <stddef.h>

#include "atb/error.h"

#if defined(__cplusplus)
extern "C" {
#endif

/// Represent a generic allocator interface
struct atb_Allocator {
  void *data; /*!< Internal allocator data forwarded at each call*/

  /// Optional interface in charge of desctructing/deleting the allocator
  void (*Delete)(void *data);

  /// Mandatory Interface in charge of allocating a memory block \a n
  /// bytes Whenever \a orgi is Valid, this interface perform a re-alloc.
  /// Returns NULL when failure.
  void *(*Alloc)(void *data, void *orig, size_t size,
                 struct atb_Error *const err);

  /// Mandatory Interface in charge of freeing/releasing a memory block
  bool (*Release)(void *data, void *mem, struct atb_Error *const err);
};

/**
 *  \brief Delete/desctruct the allocator
 *
 *  \post _Alloc/_Release function can't be used anymore
 */
static inline void atb_Allocator_Delete(struct atb_Allocator const *const self);

/**
 *  \brief (Re)Allocate \a n bytes of memory.
 *
 *  \param[in] orig If valid, indicates that we wish to RE-allocate memory
 *  \param[in] n Number of bytes we wish to (re-)allocate
 *  \param[out] err Error set when failure occurs
 *
 *  \return void* The memory block allocated. NULL in case of failure.
 *
 *  \pre self != NULL
 *  \pre self->Alloc != NULL
 */
static inline void *atb_Allocator_Alloc(struct atb_Allocator const *const self,
                                        void *orig, size_t size,
                                        struct atb_Error *const err);

/**
 *  \brief Free/release the underlying memory block allocated.
 *
 *  \param[inout] mem Memory block to de-allocate. Will be set to NULL when
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
                                         void **mem,
                                         struct atb_Error *const err);

/*****************************************************************************/
/*                         STATIC INLINE DEFINITIONS                         */
/*****************************************************************************/

static inline void atb_Allocator_Delete(
    struct atb_Allocator const *const self) {
  assert(self != NULL);
  if (self->Delete) self->Delete(self->data);
}

static inline void *atb_Allocator_Alloc(struct atb_Allocator const *const self,
                                        void *orig, size_t size,
                                        struct atb_Error *const err) {
  assert(self != NULL);
  assert(self->Alloc != NULL);
  return self->Alloc(self->data, orig, size, err);
}

static inline bool atb_Allocator_Release(struct atb_Allocator const *const self,
                                         void **mem,
                                         struct atb_Error *const err) {
  assert(self != NULL);
  assert(self->Release != NULL);
  assert(mem != NULL);

  bool success = true;

  if (*mem != NULL) {
    if (self->Release(self->data, *mem, err)) {
      *mem = NULL;
    } else {
      success = false;
    }
  }

  return success;
}

#if defined(__cplusplus)
}
#endif
