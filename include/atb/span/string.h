#pragma once

#include "atb/export.h"
#include "atb/span.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define K_ATB_FMT_STRSPAN "%.*s"
#define ATB_FMT_VA_ARG_STRSPAN(span) (int)(span).size, (span).data

#define K_ATB_FMT_STRSPAN_QUOTED "\"" K_ATB_FMT_STRSPAN "\""
#define ATB_FMT_VA_ARG_STRSPAN_QUOTED(span) ATB_FMT_VA_ARG_STRSPAN(span)

ATB_SPAN_VIEW_DECLARE(ATB_PUBLIC extern, atb_StrSpan, atb_StrView, char);

/// Constrcut a StrView from a string literal (static initializer)
#define atb_StrView_From_StrLiteral_INIT(str) \
  { .data = str, .size = atb_Array_Size(str) - 1, }

/// Constrcut a StrView from a string literal (compound literal)
#define atb_StrView_From_StrLiteral(str) \
  (struct atb_StrView) atb_StrView_From_StrLiteral_INIT(str)

/**@{*/
/**
 * \brief Construct a Str Span/View from a null terminated string
 *
 * \param[in] other Null terminated string
 *
 * \returns struct atb_[Span|View]> Span/View of the null terminated string
 *
 * \pre other !== NULL
 */
ATB_PUBLIC extern struct atb_StrSpan atb_StrSpan_From_NullTerminated(
    char *other);

ATB_PUBLIC extern struct atb_StrView atb_StrView_From_NullTerminated(
    const char *other);
/**@}*/

#if defined(__cplusplus)
}
#endif
