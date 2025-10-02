#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

/* Format string helper ******************************************************/

#define K_ATB_FMT_STR "%.*s"
#define ATB_FMT_VA_ARG_STR(str) (int)(str).size, (str).data

#define K_ATB_FMT_STR_QUOTED "\"%.*s\""
#define ATB_FMT_VA_ARG_STR_QUOTED(str) (int)(str).size, (str).data

#define K_ATB_FMT_STR_RAW "{.data=%p, .size=%zu}"
#define ATB_FMT_VA_ARG_STR_RAW(str) (str).data, (str).size

#if defined(__cplusplus)
} /* extern "C" */
#endif
