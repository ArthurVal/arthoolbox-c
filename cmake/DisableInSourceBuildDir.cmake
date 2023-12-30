#.rst:
# DisableInSourceBuildDir
# -----------------------
#
# This module is meant to be included by the main CMake project in order to
# disable IN SOURCE build dirs.

# make sure the user doesn't play dirty with symlinks
get_filename_component(srcdir "${CMAKE_SOURCE_DIR}" REALPATH)
get_filename_component(bindir "${CMAKE_BINARY_DIR}" REALPATH)

if("${srcdir}" STREQUAL "${bindir}")
  message(FATAL_ERROR
    " In-source build directory is not allowed.\n"
    " Please make an independant build directory (`mkdir <BUILD_DIR> && cmake -B <BUILD_DIR> -S <SOURCE_DIR>`)\n"
    " Feel free to remove:\n"
    " - ${srcdir}/CMakeCache.txt\n"
    " - ${srcdir}/CMakeFiles/")
endif()
