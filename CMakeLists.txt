cmake_minimum_required(VERSION 3.12)

project(atb
  VERSION 0.0.1
  DESCRIPTION "C general purpose usefull code lib/snippets"
  LANGUAGES C
)

string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWERCASE)
string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPERCASE)

###############################################################################
#                         Disable IN-SOURCE build dir                         #
###############################################################################
get_filename_component(SRC_DIR "${CMAKE_SOURCE_DIR}" REALPATH)
get_filename_component(BIN_DIR "${CMAKE_BINARY_DIR}" REALPATH)
if("${SRC_DIR}" STREQUAL "${BIN_DIR}")
  message(FATAL_ERROR
    " In-source build is not allowed.\n"
    " Please make an independant build directory using:\n"
    " cmake -S <SOURCE_DIR> -B <BUILD_DIR>\n"
    " \n"
    " Feel free to:\n"
    " rm -rf ${SRC_DIR}/CMakeFiles ${SRC_DIR}/CMakeCache.txt\n"
  )
endif()

###############################################################################
#                               GLOBAL includes                               #
###############################################################################
# We use install paths CMAKE_INSTALL_* from standard GNU install dirs See:
# https://cmake.org/cmake/help/latest/module/GNUInstallDirs.html
include(GNUInstallDirs)

# cmake_print_variables()
include(CMakePrintHelpers)

# Enables other projects to depend on us, using find_package()
# configure_package_config_file() and write_basic_package_version_file()
include(CMakePackageConfigHelpers)

message(STATUS "========== Options ==========")
###############################################################################
#                            GLOBAL USEFULL OPTIONS                           #
###############################################################################
# C STANDARD ################################################################
set(CMAKE_C_STANDARD 11)
cmake_print_variables(CMAKE_C_STANDARD)

set(CMAKE_C_STANDARD_REQUIRED True)
cmake_print_variables(CMAKE_C_STANDARD_REQUIRED)

# CMAKE_INSTALL_PREFIX ########################################################
# Just print it for the user, do not enforce it since default value is GOOD
cmake_print_variables(CMAKE_INSTALL_PREFIX)

# BUILD_TYPE ##################################################################
# We default CMAKE_BUILD_TYPE to some value if not set, with a warning msg
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
    " ==> Default to \"RELEASE\"\n"
  )

  set(CMAKE_BUILD_TYPE RELEASE
    CACHE STRING
    "Build optimizations. One of Debug, Release, RelWithDebInfo or MinSizeRel."
    FORCE
  )
endif()
cmake_print_variables(CMAKE_BUILD_TYPE)

# BUILD_SHARED_LIBS ###########################################################
# When not defined, add a hint to user that this exists (already OFF by default)
if(NOT DEFINED BUILD_SHARED_LIBS)
  message(WARNING
    " BUILD_SHARED_LIBS has not been defined.\n"
    " Default to OFF\n"
  )
  option(BUILD_SHARED_LIBS "Create shared libs (.so)" OFF)
endif()
cmake_print_variables(BUILD_SHARED_LIBS)

###############################################################################
#                           PROJET SPECIFIC OPTIONS                           #
###############################################################################
# ENABLE_TESTING ##############################################################
# We create a single value independent of BUILD_TESTING in case we want to
# enable the unit tests only for this package. BUILD_TESTING enables unit tests
# globally.
option(${PROJECT_NAME_UPPERCASE}_ENABLE_TESTING
  "Enable unittests for project \"${PROJECT_NAME_UPPERCASE}\""
  OFF
)

if(BUILD_TESTING)
  set(${PROJECT_NAME_UPPERCASE}_ENABLE_TESTING ON)
endif()

cmake_print_variables(${PROJECT_NAME_UPPERCASE}_ENABLE_TESTING)

###############################################################################
#                               CODE GENERATION                               #
###############################################################################
# n/a

###############################################################################
#                                    BUILD                                    #
###############################################################################
add_subdirectory(src)

if(${PROJECT_NAME_UPPERCASE}_ENABLE_TESTING)
  if(CMAKE_CROSSCOMPILING)
    message(WARNING
      " Unit testing is automatically disabled when cross compiling"
    )
  else()
    message(STATUS "Unittests ENABLED")
    enable_testing()
    add_subdirectory(tests)
  endif()
endif()

###############################################################################
#                                 CONFIG FILES                                #
###############################################################################
# Construct the project global config.h
configure_file(
  include/${PROJECT_NAME}/config.h.in
  include/${PROJECT_NAME}/config.h
  @ONLY
)

# Construct the project global export.h
configure_file(
  include/${PROJECT_NAME}/export.h.in
  include/${PROJECT_NAME}/export.h
  @ONLY
)

###############################################################################
#                                   INSTALL                                   #
###############################################################################
# Public headers from include/
install(
  DIRECTORY
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include/>
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  PATTERN "*.in" EXCLUDE
  PATTERN "*.h"
  PATTERN "*.hpp"
)

# # Create and install Config and ConfigVersion.cmake  ###########################
# # Create ConfigVersion.cmake, used by find_package() for version checks
# write_basic_package_version_file(
#   "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
#   VERSION "${PROJECT_VERSION}"
#   COMPATIBILITY AnyNewerVersion
# )

# # Create the '${PROJECT_NAME}Config.cmake' using
# # '${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in'
# configure_package_config_file(
#   ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in
#   "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
#   INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
# )

# # Install files created above
# install(
#   FILES
#   "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
#   "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"

#   DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
# )
