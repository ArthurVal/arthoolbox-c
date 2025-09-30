add_executable(${PROJECT_NAME}-test
  test_macro.cpp
  test_bits.cpp
  test_list.cpp
  test_array.cpp
  test_error.cpp
  test_ints.cpp
  test_ratio.cpp
  test_time.cpp
  test_string.cpp
)

target_include_directories(${PROJECT_NAME}-test
  PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

target_link_libraries(${PROJECT_NAME}-test
  PRIVATE ${PROJECT_NAME}::${PROJECT_NAME}
  PRIVATE GTest::gtest_main
  PRIVATE GTest::gmock_main
)

target_compile_options(${PROJECT_NAME}-test
  PRIVATE
  -Wall
  -Wextra
  -Wshadow
  -Wnon-virtual-dtor
  # -Wpedantic
)

target_compile_features(${PROJECT_NAME}-test
  PRIVATE cxx_std_17
)

gtest_discover_tests(${PROJECT_NAME}-test)
