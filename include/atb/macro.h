#pragma once

#include "internal/macro.h"

/**
 *  \return Get the size of the given __VA_ARGS__ (up to 20)
 */
#define atb_VA_ARGN(...) \
  atb_internal_Macro_ARGN(_, ##__VA_ARGS__, atb_internal_Macro_ARGN_20)

#define atb_
