add_library(${PROJECT_NAME}-utils INTERFACE)

target_link_libraries(${PROJECT_NAME}-utils
  INTERFACE
  atb::atb
  GTest::gtest_main
  GTest::gmock_main
)

target_include_directories(${PROJECT_NAME}-utils
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

target_compile_options(${PROJECT_NAME}-utils
  INTERFACE -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic
)

target_compile_features(${PROJECT_NAME}-test
  INTERFACE cxx_std_17
)
