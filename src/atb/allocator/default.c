#include "atb/allocator/default.h"

#include <stdlib.h>

static bool DefaultAllocator_Alloc(void *data, struct atb_MemSpan orig,
                                   size_t size, struct atb_MemSpan *const mem,
                                   struct atb_Error *const err) {
  if (atb_MemSpan_IsValid(orig)) {
    data = realloc(orig.data, size);
  } else {
    data = malloc(size);
  }

  if (data != NULL) {
    mem->data = data;
    mem->size = size;
    return true;
  } else {
    atb_GenericError_Set(err, K_ATB_ERROR_GENERIC_NOT_ENOUGH_MEMORY);
    return false;
  }
}

static bool DefaultAllocator_Release(void *data, struct atb_MemSpan mem,
                                     struct atb_Error *const err) {
  (void)data;
  (void)err;
  free(mem.data);
  return true;
}

struct atb_Allocator const *atb_DefaultAllocator(void) {
  static struct atb_Allocator const m_default_allocator = {
      .data = NULL,
      .Alloc = DefaultAllocator_Alloc,
      .Release = DefaultAllocator_Release,
  };

  return &(m_default_allocator);
}
