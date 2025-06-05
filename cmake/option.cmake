# .rst:
# ----- option.cmake -----
# This module provide utils functionallity to declare more complexe options
# (other than ON/OFF provided by cmake's 'option()').
#
# ~~~
# Variables:
#   * n/a
# Functions:
#   * option_choice(<NAME> [CHOICES] [DESCRIPTION] [DEFAULT])
# Macros:
#   * n/a
# ~~~

# ~~~
# Create a new option NAME that is expected to be contained within an underlying
# set of values.
#
# Arguments:
#   * NAME (in):
#       Option variable name.
#   * CHOICES [VALUE_1, VALUE_2, ...] (optional - in):
#       The list of expected value for this option.
#   * DEFAULT <VALUE> (optional - in):
#       Optional default value for the option (will be automatically appended to
#       the expected value list).
#   * DESCRIPTION <DESCR>  (optional - in):
#       Optional description string used for this option. The description
#       will be automatically appended with the list of expected values.
#   * ...:
#       Any unparsed arguments will be appended to CHOICES
# ~~~
function(option_choice NAME)
  cmake_parse_arguments(arg
    ""                    # <- Flags
    "DESCRIPTION;DEFAULT" # <- One value
    "CHOICES"             # <- Multi values
    ${ARGN}
  )

  if(arg_UNPARSED_ARGUMENTS)
    list(APPEND arg_CHOICES ${arg_UNPARSED_ARGUMENTS})
  endif()

  if(NOT arg_DEFAULT)
    # Default the default value to _NOTSET when not provided
    set(arg_DEFAULT ${NAME}_NOTSET)
  elseif(NOT arg_DEFAULT IN_LIST arg_CHOICES)
    # Append default to CHOICES, if default is not already inside CHOICES
    list(APPEND arg_CHOICES ${arg_DEFAULT})
  endif()

  # arg_CHOICES_STR is a 'prettier' string used for messages
  list(JOIN arg_CHOICES ", " arg_CHOICES_STR)

  # Giving only one choice is not very usefull...
  list(LENGTH arg_CHOICES _size)
  if(_size LESS_EQUAL 1)
    message(FATAL_ERROR
      " Did not provided enough CHOICES values for option '${NAME}'.\n"
      " Got ${_size} values ([${arg_CHOICES_STR}]), expecting at least 2.\n"
    )
  endif()

  # Update the description with the choices...
  if(arg_DESCRIPTION)
    string(APPEND arg_DESCRIPTION " (Must be one of: [${arg_CHOICES_STR}])")
  else()
    set(arg_DESCRIPTION "One of: [${arg_CHOICES_STR}]")
  endif()

  # ... and the default value if provided
  if(NOT ${arg_DEFAULT} STREQUAL ${NAME}_NOTSET)
    string(APPEND arg_DESCRIPTION " (Default: ${arg_DEFAULT})")
  endif()

  set(${NAME} ${arg_DEFAULT}
    CACHE STRING ${arg_DESCRIPTION}
  )

  # This is for cmake-gui users
  set_property(CACHE ${NAME}
    PROPERTY STRINGS
    ${arg_CHOICES}
  )

  if(NOT ${NAME} IN_LIST arg_CHOICES)
    message(FATAL_ERROR
      " Unknown value for ${NAME} (= ${${NAME}}).\n"
      " Expecting to be one of [${arg_CHOICES_STR}].\n"
      " Set it by either:\n"
      " - Configure cmake using '-D${NAME}=<VALUE>'\n"
      " - Update ${NAME} inside CMakeCache.txt manually\n"
    )
  endif()
endfunction()
