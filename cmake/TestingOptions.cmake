#.rst:
# TestingOptions
# --------------
#
# This module is meant to be included by the main CMake of the arthoolbox-c
# project.
#
# It declares the following options, prefixed by ${PROJECT_NAME}:
#   ${PROJECT_NAME}_ENABLE_TESTING    - BOOL   - Enable unittests.
#   ${PROJECT_NAME}_TESTING_GTEST_URL - STRING - GTest URL location

# include(CMakeDependentOption)   #for cmake_dependent_option
include(CMakePrintHelpers)      #for cmake_print_variables & cmake_print_properties

message(STATUS "${PROJECT_NAME} TestingOptions.cmake ...")

option(${PROJECT_NAME}_ENABLE_TESTING "Enable the unit tests build" OFF)
cmake_print_variables(${PROJECT_NAME}_ENABLE_TESTING)

set(${PROJECT_NAME}_TESTING_GTEST_URL
  "https://github.com/google/googletest/archive/v1.14.0.zip"
  CACHE STRING
  "Points towards the googletest.zip URL that will be fetch.")
cmake_print_variables(${PROJECT_NAME}_TESTING_GTEST_URL)

function(find_gtest)
  find_package(GTest)
  if(NOT GTest_FOUND)
    message(STATUS "Fetching GTest @ \"${${PROJECT_NAME}_TESTING_GTEST_URL}\": ...")

    # This is use to force CMake to build gtest with std=c++17, I don't know how
    # to use target_compile_feature() with FetchContent
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    include(FetchContent)
    FetchContent_Declare(
      googletest
      URL ${${PROJECT_NAME}_TESTING_GTEST_URL}
    )
    FetchContent_MakeAvailable(googletest)
    message(STATUS "Fetching GTest @ \"${${PROJECT_NAME}_TESTING_GTEST_URL}\": DONE")
  endif()
endfunction()
