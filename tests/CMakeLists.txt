###############################################################################
#                               TESTING OPTIONS                               #
###############################################################################
# CXX STANDARD ##############################################################
# We depend on GTest, a C++ lib
enable_language(CXX)

set(CMAKE_CXX_STANDARD 17)
cmake_print_variables(CMAKE_CXX_STANDARD)

set(CMAKE_CXX_STANDARD_REQUIRED True)
cmake_print_variables(CMAKE_CXX_STANDARD_REQUIRED)

# ${PROJECT_PREFIX}_TESTING_GTEST_URL ###############################
# Only when not using find_package...
set(${PROJECT_PREFIX}_TESTING_GTEST_URL ""
  CACHE STRING
  "GTest location that may be fetched (e.g. https://github.com/google/googletest/archive/v1.17.0.zip). By default or when empty, use `find_package(GTest REQUIRED)`."
)
cmake_print_variables(${PROJECT_PREFIX}_TESTING_GTEST_URL)

###############################################################################
#                                 TESTING DEPS                                #
###############################################################################
# GTest #######################################################################
if("${${PROJECT_PREFIX}_TESTING_GTEST_URL}" STREQUAL "")
  message(STATUS "Using GTest installed in the system: ...")
  find_package(GTest REQUIRED)
else()
  message(STATUS "Fetching GTest: ...")

  include(FetchContent)
  FetchContent_Declare(googletest
    URL "${${PROJECT_PREFIX}_TESTING_GTEST_URL}"
    DOWNLOAD_EXTRACT_TIMESTAMP ON
  )

  # GTest customizations ######################################################
  set(INSTALL_GTEST OFF)
  FetchContent_MakeAvailable(googletest)
endif()

include(GoogleTest)
add_subdirectory(atb)
