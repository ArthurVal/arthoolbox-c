#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(__cplusplus)
#define ATB_COMPOUND_LITERAL(type) type
#else
#define ATB_COMPOUND_LITERAL(type) (struct type)
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif
