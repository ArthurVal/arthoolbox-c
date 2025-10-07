#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h> /* strlen */

#include "atb/memory/span.h"

#if defined(__cplusplus)
extern "C" {
#endif

/// Implement default Span functionnalities for atb_StrSpan
ATB_MEM_DEFINE_SPAN(atb_Str, , char);
ATB_MEM_DEFINE_SPAN_FN(static inline, atb_Str, , char);

/// An invalid StrSpan (static initializer)
#define K_ATB_STRSPAN_INVALID_INIT K_ATB_MEMSPAN_INVALID_INIT

/// An invalid StrSpan (compound literal)
#define K_ATB_STRSPAN_INVALID (struct atb_StrSpan) K_ATB_STRSPAN_INVALID_INIT

/// Construct a StrSpan mapping to a STATIC array (static initializer)
#define atb_StrSpan_From_Array_INIT(char_array) \
  atb_MemSpan_From_Array_INIT(char_array)

/// Construct a StrSpan mapping to a STATIC array (compound expression)
#define atb_StrSpan_From_Array(char_array) \
  (struct atb_StrSpan) atb_StrSpan_From_Array_INIT(char_array)

/// Construct a StrSpan from a NULL terminated string
static inline struct atb_StrSpan atb_StrSpan_From_NullTerminated(char *other);

/*****************************************************************************/
/*                         STATIC INLINE DEFINITIONS                         */
/*****************************************************************************/

static inline struct atb_StrSpan atb_StrSpan_From_NullTerminated(char *other) {
  return atb_StrSpan_From(other, strlen(other));
}

#if defined(__cplusplus)
} /* extern "C" */
#endif
