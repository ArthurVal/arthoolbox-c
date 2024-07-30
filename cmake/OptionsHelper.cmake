#.rst
# OptionsHelper
# -------------
#
# This module is meant to provide helper function to declare compile options.

function(${PROJECT_NAME}_add_option)
  cmake_parse_arguments(
    # Prefix used
    OPT
    # Bool options
    ""
    # Single value options
    "NAME;DESCRIPTION;TYPE;DEFAULT"
    # Multiple values options
    "CHOICE"
    ${ARGN}
    )

  foreach(id NAME CHOICE)
    if(NOT DEFINED OPT_${id})
      message(FATAL_ERROR "${PROJECT_NAME}_add_option(...): Missing REQUIRED ${id} option argument")
    endif()
  endforeach()

  if(NOT DEFINED OPT_DEFAULT)
    list(GET OPT_CHOICE 0 OPT_DEFAULT)
  endif()

  if(NOT DEFINED OPT_TYPE)
    set(OPT_TYPE STRING)
  endif()

  if(NOT DEFINED OPT_DESCRIPTION)
    set(OPT_DESCRIPTION "")
  endif()

  set(${OPT_NAME} ${OPT_DEFAULT} CACHE ${OPT_TYPE} ${OPT_DESCRIPTION})
  set_property(CACHE ${OPT_NAME} PROPERTY STRINGS
    ${OPT_CHOICE}
    )

  list(FIND OPT_CHOICE ${${OPT_NAME}} index)
  if(index EQUAL -1)
    message(FATAL_ERROR
      "${OPT_NAME} (= ${${OPT_NAME}}) must be one of [${OPT_CHOICE}]"
      )
  endif()

  message(STATUS "${OPT_NAME}=\"${${OPT_NAME}}\" [${OPT_CHOICE}]")
endfunction()
