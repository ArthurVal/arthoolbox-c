cmake_minimum_required(VERSION 3.10)

project(atb
  VERSION 0.0.1
  DESCRIPTION "C general purpose usefull code lib/snippets"
  LANGUAGES C CXX
  )

include(cmake/DisableInSourceBuildDir.cmake)

include(CMakePrintHelpers)
include(GNUInstallDirs)

configure_file(
  include/${PROJECT_NAME}/version.h.in
  include/${PROJECT_NAME}/version.h
  @ONLY
  )

add_subdirectory(src)

install(
  DIRECTORY
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include/>
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  PATTERN "*.in" EXCLUDE
  PATTERN "*.h"
  )

include(cmake/TestingOptions.cmake)
if(${PROJECT_NAME}_ENABLE_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()