#pragma once

#include "internal/macro.h"

/**
 *  \return The size of the given __VA_ARGS__ (up to 200 tokens)
 */
#define atb_VA_ARGN(...) \
  atb_internal_ARGN(_, ##__VA_ARGS__, atb_internal_ARGN_200)

/**
 *  \brief Create a token by concatenating the given args AFTER evaluation (up
 *         to 10 tokens)
 */
#define atb_TKN_CONCAT(...) \
  atb_internal_TKN_CONCAT_N(atb_VA_ARGN(__VA_ARGS__), __VA_ARGS__)

/**
 *  \brief Stringify a token AFTER evaluation
 */
#define atb_TKN_STR(a) atb_internal_TKN_STR(a)
