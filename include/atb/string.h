
#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "atb/error.h"
#include "atb/export.h"
#include "atb/span/string.h"

#if defined(__cplusplus)
extern "C" {
#endif

/// Use to qualify the integer BASE for string representation
typedef enum {
  K_ATB_INT_BIN = 2,  /*!< Binary */
  K_ATB_INT_OCT = 8,  /*!< Octal */
  K_ATB_INT_DEC = 10, /*!< Decimal */
  K_ATB_INT_HEX = 16, /*!< Hexadecimal */
} ATB_INT_BASE;

/**
 *  \brief Try to deduce the base from a str.
 *
 *  Expected base prefix are the following:
 *  - '0x' | '0X': K_ATB_INT_HEX;
 *  - '0b' | '0B': K_ATB_INT_BIN;
 *  - '0'  | '0o': K_ATB_INT_OCT;
 *  - Any digits (optionally starting with '-'): K_ATB_INT_DEC
 *
 *  If none of the previous is found at the begin of the input view, this is
 *  considered as an error.
 *
 *  \param[in] str The input str to check for an INT base qualifier
 *  \param[out] base The base found
 *  \param[out] remaining Optional Part of the input striped of the base
 *  \param[out] err Optional. Error set whenever the operation failed.
 *                  Possible values are:
 *                  - GENERIC_INVALID_ARGUMENT: The \a str is too smal (size 0)
 *                  - GENERIC_ARGUMENT_OUT_OF_DOMAIN: The \a str doesn't start
 *                    with a base qualifier NOR a digit
 *
 * \returns bool True on success. False otherwise, \a err is set accordingly and
 *               both \a base and \a remaining remain untouched.
 *
 *  \pre base != NULL
 */
ATB_PUBLIC bool atb_String_ToIntBase(struct atb_StrView str,
                                     ATB_INT_BASE *const base,
                                     struct atb_StrView *const remaining,
                                     struct atb_Error *const err);

/**@{*/
/**
 * \brief Convert the given \a value to its string repr into \a dest.
 *
 * \param[in] value Int value we wish to convert to string
 * \param[in] base Base use to represent the int
 * \param[in] dest Destination str range (may be invalid)
 * \param[out] remaining Optional.
 *                       When \a dest is a VALID StrSpan, set to the remaining
 *                       space in \a dest after writing in it.
 *                       Otherwise (\a dest is INVALID) set remaining->size to
 *                       the minimum size required to represent the given
 *                       integer.
 * \param[out] err Optional. Error set whenever the operation failed.
 *                 Possible values are:
 *                 - GENERIC_VALUE_TOO_LARGE: When the provided \a dest is too
 *                   small to fit the int string repr;
 *
 * \returns bool True on success. False otherwise, \a err is set accordingly and
 *               both dest and remaining remains untouched.
 *
 * \important Signess ONLY appears when selecting K_ATB_INT_DEC base
 *
 * \pre base is in [2;37) interval
 */
ATB_PUBLIC bool atb_String_FromInt_u(uintmax_t value, ATB_INT_BASE base,
                                     struct atb_StrSpan dest,
                                     struct atb_StrSpan *const remaining,
                                     struct atb_Error *const err);

ATB_PUBLIC bool atb_String_FromInt_i(intmax_t value, ATB_INT_BASE base,
                                     struct atb_StrSpan dest,
                                     struct atb_StrSpan *const remaining,
                                     struct atb_Error *const err);
/**@}*/

#if defined(__cplusplus)
}
#endif
