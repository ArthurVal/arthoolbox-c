cmake_minimum_required(VERSION 3.9)

project(arthoolbox-c
  VERSION 0.1.0
  DESCRIPTION "C general purpose usefull code lib/snippets."
  LANGUAGES C CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

include(cmake/DisableInSourceBuildDir.cmake)
include(cmake/TestingOptions.cmake)

add_subdirectory(src)

if(${PROJECT_NAME}_ENABLE_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()
