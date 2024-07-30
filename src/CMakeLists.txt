add_subdirectory(atb)

add_executable(${PROJECT_NAME}-print-version print-version.c)

target_link_libraries(${PROJECT_NAME}-print-version
  PRIVATE
  ${PROJECT_NAME}::${PROJECT_NAME}
  )