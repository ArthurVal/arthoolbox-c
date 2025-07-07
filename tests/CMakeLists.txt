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

# ${PROJECT_NAME_UPPERCASE}_TESTING_GTEST_URL ###############################
# Only when not using find_package...
set(${PROJECT_NAME_UPPERCASE}_TESTING_GTEST_URL
  "https://github.com/google/googletest/archive/v1.14.0.zip"
  CACHE STRING
  "GTest archive location that will be fetched (if empty, use find_package() instead)"
)
cmake_print_variables(${PROJECT_NAME_UPPERCASE}_TESTING_GTEST_URL)

###############################################################################
#                                 TESTING DEPS                                #
###############################################################################
# GTest #######################################################################
if("${${PROJECT_NAME_UPPERCASE}_TESTING_GTEST_URL}" STREQUAL "")
  message(STATUS "Using GTest installed in the system: ...")
  find_package(GTest REQUIRED)
else()
  message(STATUS "Fetching GTest: ...")

  include(FetchContent)
  FetchContent_Declare(googletest
    URL "${${PROJECT_NAME_UPPERCASE}_TESTING_GTEST_URL}"
    DOWNLOAD_EXTRACT_TIMESTAMP
  )

  # GTest customizations ######################################################
  set(INSTALL_GTEST OFF)
  FetchContent_MakeAvailable(googletest)
endif()

include(GoogleTest)
add_subdirectory(atb)
