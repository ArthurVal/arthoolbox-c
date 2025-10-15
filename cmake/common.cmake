# .rst:
# ----- common.cmake -----
# This module provide utils/common generic functionallities for cmake.
#
# ~~~
# Variables:
#   * n/a
# Functions:
#   * common_disable_in_src_builds()
#   * common_warn_build_type([DEFAULT])
# Macros:
#   * n/a
# ~~~

# ~~~
# Disable in-source builds.
#
# By default, in-source builds are disabled in order to not add compilation
# artifacts into the source directory which is most likely tracked by the CVS.
#
# In case you are FORCED to do in-src builds (for a distro, docker, ... ? IDK
# ...), set IN_SRC_BUILDS variable to ON in order to bypass the in-src build
# check.
#
# Arguments:
#   * N/A
# ~~~
function(common_disable_in_src_builds)
  option(IN_SRC_BUILDS
    "Enable/Disable In-source builds (Disabled by default)"
    OFF
  )

  if(NOT IN_SRC_BUILDS)
    get_filename_component(_SRC_DIR "${CMAKE_SOURCE_DIR}" REALPATH)
    get_filename_component(_BIN_DIR "${CMAKE_BINARY_DIR}" REALPATH)
    if("${_SRC_DIR}" STREQUAL "${_BIN_DIR}")
      message(FATAL_ERROR
        " In-source builds are not allowed.\n"
        " Please make an independant build directory using:\n"
        " cmake -S <SOURCE_DIR> -B <BUILD_DIR>\n"
        " \n"
        " Feel free to:\n"
        " rm -rf ${_SRC_DIR}/CMakeFiles ${_SRC_DIR}/CMakeCache.txt\n"
        " \n"
        " \n"
        " If in-source builds are mandatory, use '-DIN_SRC_BUILDS=ON' option"
      )
    endif()
  endif()
endfunction()

# ~~~
# Warn when CMAKE_BUILD_TYPE has not been set and force it to a default value.
#
# Arguments:
#   * DEFAULT (optional - in):
#       Optional default CMAKE_BUILD_TYPE value used when CMAKE_BUILD_TYPE is
#       not defined by the user.
# ~~~
function(common_warn_build_type)
  cmake_parse_arguments(_args
    ""        # <- Flags
    "DEFAULT" # <- One value
    ""        # <- Multi values
    ${ARGN}
  )

  if(NOT _args_DEFAULT)
    set(_args_DEFAULT RELEASE)
  endif()

  if(
      NOT DEFINED CMAKE_BUILD_TYPE
      OR "${CMAKE_BUILD_TYPE}" STREQUAL ""
    )
    message(WARNING
      " CMAKE_BUILD_TYPE has not been defined. It should be set to one of: \n"
      " - Debug:          ${CMAKE_C_FLAGS_DEBUG}\n"
      " - Release:        ${CMAKE_C_FLAGS_RELEASE}\n"
      " - RelWithDebInfo: ${CMAKE_C_FLAGS_RELWITHDEBINFO})\n"
      " - MinSizeRel:     ${CMAKE_C_FLAGS_MINSIZEREL}\n"
      " - ... (Other based on the generator used)\n"
      " ==> Default to \"${_args_DEFAULT}\"\n"
    )

    set(CMAKE_BUILD_TYPE ${_args_DEFAULT}
      CACHE STRING
      "Build optimizations. One of Debug, Release, RelWithDebInfo, MinSizeRel..."
      FORCE
    )
  endif()
endfunction()
