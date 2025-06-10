add_executable(${PROJECT_NAME}-test
    test_list.cpp
    test_array.cpp
    test_str_view.cpp
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
  -pedantic
)

target_compile_features(${PROJECT_NAME}-test
  PRIVATE cxx_std_17
)

gtest_discover_tests(${PROJECT_NAME}-test)
