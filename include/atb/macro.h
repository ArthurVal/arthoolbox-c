#pragma once

#include "internal/macro.h"

/**
 *  \return The size of the given __VA_ARGS__ (up to 200 tokens)
 */
#define ATB_VA_ARGN(...) \
  ATB_INTERNAL_ARGN(_, ##__VA_ARGS__, ATB_INTERNAL_ARGN_200)

/**
 *  \brief Create a token by concatenating the given args AFTER evaluation (up
 *         to 10 tokens)
 */
#define ATB_TKN_CAT(...) \
  ATB_INTERNAL_TKN_CAT_N(ATB_VA_ARGN(__VA_ARGS__), __VA_ARGS__)

/**
 *  \brief Stringify a token AFTER evaluation
 */
#define ATB_TKN_STR(a) ATB_INTERNAL_TKN_STR(a)

/**
 *  \brief Evaluate to 1 if the T is 'void', 0 otherwise.
 *
 *  \note This doesn't work if T is either 'void *' or 'void const *'.
 *        In this case, use a typedef `typedef void * void_ptr`.
 */
#define ATB_IS_VOID(T) ATB_INTERNAL_IS_VOID(T)
