#pragma once

#include "atb/allocator.h"
#include "atb/export.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 *  \return struct atb_Allocator Corresponding to the default system heap
 *          allocator, using malloc/realloc/free.
 */
extern struct atb_Allocator const *atb_DefaultAllocator(void) ATB_PUBLIC;

#if defined(__cplusplus)
}
#endif
