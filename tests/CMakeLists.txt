project(${PROJECT_NAME}-test DESCRIPTION "ATB UnitTests")

message(STATUS "Building unittests ...")

message(STATUS "Finding GTest: ...")
find_gtest()
message(STATUS "Finding GTest: DONE")

add_subdirectory(atb)
