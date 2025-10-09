#include "atb/allocator/default.h"

#include <stdlib.h>

static void *DefaultAllocator_Alloc(void *data, void *orig, size_t size,
                                    struct atb_Error *const err) {
  data = realloc(orig, size);

  if (data == NULL) {
    atb_GenericError_Set(err, K_ATB_ERROR_GENERIC_NOT_ENOUGH_MEMORY);
  }

  return data;
}

static bool DefaultAllocator_Release(void *data, void *mem,
                                     struct atb_Error *const err) {
  (void)data;
  (void)err;
  free(mem);
  return true;
}

struct atb_Allocator const *atb_DefaultAllocator(void) {
  static struct atb_Allocator const m_default_allocator = {
      .data = NULL,
      .Delete = NULL,
      .Alloc = DefaultAllocator_Alloc,
      .Release = DefaultAllocator_Release,
  };

  return &(m_default_allocator);
}
