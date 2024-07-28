add_subdirectory(container)

# add_executable(${PROJECT_NAME}
#   # test_StaticArray.cpp
#   test_list.cpp
#   # Span/test_Arithmetic.cpp
#   # test_String.cpp
#   # test_AnsiTerm.cpp
# )

# target_include_directories(${PROJECT_NAME}
#   PRIVATE
#   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
#   $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
# )

# target_link_libraries(${PROJECT_NAME}
#   PRIVATE arthoolbox-c::arthoolbox-c
#   PRIVATE GTest::gtest_main
#   PRIVATE GTest::gmock_main
# )

# target_compile_options(${PROJECT_NAME}
#   PRIVATE -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic
# )

# target_compile_features(${PROJECT_NAME}
#   PRIVATE cxx_std_20
# )

# include(GoogleTest)

# if(${CMAKE_VERSION} VERSION_LESS "3.10.0")
#   gtest_add_tests(TARGET ${PROJECT_NAME})
# else()
#   gtest_discover_tests(${PROJECT_NAME})
# endif()
