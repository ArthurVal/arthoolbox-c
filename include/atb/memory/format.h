#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#define K_ATB_FMT_MEM "{.data=%p, .size=%zu}"
#define ATB_FMT_VA_ARG_MEM(mem) (mem).data, (mem).size

#if defined(__cplusplus)
}
#endif
