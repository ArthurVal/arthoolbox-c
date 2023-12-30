add_executable(${PROJECT_NAME}-test
  test_StaticArray.cpp
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
  PRIVATE -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic
)

include(GoogleTest)

if(${CMAKE_VERSION} VERSION_LESS "3.10.0")
  gtest_add_tests(TARGET ${PROJECT_NAME}-test)
else()
  gtest_discover_tests(${PROJECT_NAME}-test)
endif()
