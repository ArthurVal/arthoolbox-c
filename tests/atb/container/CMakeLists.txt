add_executable(${PROJECT_NAME}_container
  test_array.cpp  
  test_list.cpp
)

target_include_directories(${PROJECT_NAME}_container
  PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

target_link_libraries(${PROJECT_NAME}_container
  PRIVATE arthoolbox-c::arthoolbox-c
  PRIVATE GTest::gtest_main
  PRIVATE GTest::gmock_main
)

target_compile_options(${PROJECT_NAME}_container
  PRIVATE -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic
)

target_compile_features(${PROJECT_NAME}_container
  PRIVATE cxx_std_20
)

include(GoogleTest)

if(${CMAKE_VERSION} VERSION_LESS "3.10.0")
  gtest_add_tests(TARGET ${PROJECT_NAME}_container)
else()
  gtest_discover_tests(${PROJECT_NAME}_container)
endif()
