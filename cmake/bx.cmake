# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
cmake_minimum_required(VERSION 3.24)

include_guard(GLOBAL)

function(bx_print_help)
  list(APPEND bx_help_lines
    "An end-to-end build system for C, C++, and Fortran projects."
    ""
    "Commands:"
    "  cmake  Run a cmake command configured by `bx`"
    "  help   Show this help message"
    ""
    "Options:"
    "  -h, --help   Show this help message"
    "  --log-level  Configure message verbosity for the `bx` command"
    "               [possible values: trace, debug, status, warning, error, fatal_error]"
    ""
    "Environment variables:"
    "  BX_CACHE_DIR  The directory to use for caching `bx` releases."
    "  BX_INFRA_DIR  The directory to use for the `bx` implementation."
    "  BX_HASH       The hash of the `bx` release to use."
    "  BX_VERSION    The version of `bx` to use. Use with BX_HASH."
    ""
    "Examples:"
    "  Configure a CMake project with bx:"
    "      bx cmake -DCMAKE_BUILD_TYPE=Release -S . -B build"
  )
  string(REPLACE ";" "\n" bx_help_string "${bx_help_lines}")
  execute_process(
    COMMAND ${CMAKE_COMMAND} -E echo "${bx_help_string}"
  )
endfunction()

function(bx_parse_subcommand)
  set(bx_options )
  set(bx_singleargs REST SUB_ARGV SUB_COMMAND SUB_NAME)
  set(bx_multiargs ARGV)
  cmake_parse_arguments(PARSE_ARGV
    0 bx_arg "${bx_options}" "${bx_singleargs}" "${bx_multiargs}"
  )

  list(APPEND CMAKE_MESSAGE_INDENT " [parse_subcommand] ")
  set(bx_subname "subname-NOTFOUND")
  set(bx_subcommand "subcommand-NOTFOUND")
  set(bx_argv ) # Arguments to `bx`
  set(bx_subargv ) # Arguments to the subcommand
  foreach(bx_arg ${bx_arg_ARGV})
    # Get the basename of the argument
    get_filename_component(bx_arg_basename "${bx_arg}" NAME)
    # Used to detect if the arg looks starts with a `-` or not;
    # this is used to determine if the arg is a subcommand or not.
    string(SUBSTRING "${bx_arg_basename}" 0 1 bx_arg_first_char)
    if(NOT bx_argv)
      # First argv is the `bx` command itself
      list(APPEND bx_argv "${bx_arg}")
    elseif(bx_subname)
      list(APPEND bx_subargv "${bx_arg}")
    elseif(bx_arg_basename STREQUAL "help" OR
           bx_arg STREQUAL "--help" OR
           bx_arg STREQUAL "-h")
      bx_print_help()
      return()
    elseif(bx_arg_basename STREQUAL "cmake")
      set(bx_subname "cmake")
      set(bx_subcommand "${bx_arg}")
    # TODO: Add sections for more subcommands here... until a more
    #       natural design is implemented.
    else()
      list(APPEND bx_argv "${bx_arg}")
    endif()
  endforeach()

  message(TRACE "bx_argv=\"${bx_argv}\"")
  message(TRACE "bx_subargv=\"${bx_subargv}\"")
  message(TRACE "bx_subcommand=\"${bx_subcommand}\"")
  message(TRACE "bx_subname=\"${bx_subname}\"")

  if(NOT bx_subname)
    string(APPEND bx_message "Failed to parse a subcommand name. ")
    string(
      APPEND bx_message "See `bx --help` for a list of available subcommands."
    )
    message(FATAL_ERROR "${bx_message}")
  endif()

  set("${bx_arg_SUB_ARGV}" "${bx_subargv}" PARENT_SCOPE)
  set("${bx_arg_SUB_COMMAND}" "${bx_subcommand}" PARENT_SCOPE)
  set("${bx_arg_SUB_NAME}" "${bx_subname}" PARENT_SCOPE)
  set("${bx_arg_REST}" "${bx_argv}" PARENT_SCOPE)

  list(POP_BACK CMAKE_MESSAGE_INDENT)
endfunction()

function(bx_cmake_subcommand)
  set(bx_options )
  set(bx_singleargs )
  set(bx_multiargs ARGV)
  cmake_parse_arguments(PARSE_ARGV
    0 bx_arg "${bx_options}" "${bx_singleargs}" "${bx_multiargs}"
  )

  list(APPEND CMAKE_MESSAGE_INDENT " [cmake_subcommand] ")

  message(TRACE "Running the cmake subcommand with ARGV=\"${bx_arg_ARGV}\"")

  message(STATUS "Configuring FetchContent provisioning mode.")
  set(fetch_content_shim "${CMAKE_CURRENT_LIST_DIR}/use-fetch-content.cmake")

  # Why? Because escaping semicolons is a PITA. Instead, just parse out the
  # top level includes and include them in order, appending the bx file to
  # inject FetchContent at the end of the list.
  # This will keep the below relatively simple.
  message(WARNING "TODO: Use a bx_top_level_includes.cmake and include everything.")
  set(bx_shim_applied FALSE) # Has the fetch content shim been applied yet?
  set(bx_command ) # The command line to run
  foreach(bx_arg ${bx_arg_ARGV})
    if (bx_arg MATCHES "-DCMAKE_PROJECT_TOP_LEVEL_INCLUDES=")
      # NOTE: This doesn't actually work because of semicolon escaping and such.
      #       See the WARNING message above.
      set(bx_shimarg "${bx_arg};${fetch_content_shim}")
      message(TRACE
        "Replacing argument \"${bx_arg}\" with \"${bx_shimarg}\"."
      )
      list(APPEND bx_command "${bx_shimarg}")
      set(bx_shim_applied TRUE)
    else()
      # Otherwise, use the next argument as-is
      list(APPEND bx_command "${bx_arg}")
      message(TRACE "... bx_command=\"${bx_command}\"")
    endif()
  endforeach()
  message(TRACE "bx_command=\"${bx_command}\"")

  if(NOT bx_shim_applied)
    message(TRACE "Appending -DCMAKE_PROJECT_TOP_LEVEL_INCLUDES to command line.")
    list(APPEND bx_command "-DCMAKE_PROJECT_TOP_LEVEL_INCLUDES=${fetch_content_shim}")
  endif()

  # make a space delimited list of the command line arguments
  string(REPLACE ";" " " command_line_str "${bx_command}")
  message(STATUS "Running: ${command_line_str}")
  execute_process(COMMAND ${bx_command} COMMAND_ERROR_IS_FATAL ANY)

  list(POP_BACK CMAKE_MESSAGE_INDENT)
endfunction()

function(bx_run)
  set(bx_options )
  set(bx_singleargs )
  set(bx_multiargs ARGV)
  cmake_parse_arguments(PARSE_ARGV
    0 bx_arg "${bx_options}" "${bx_singleargs}" "${bx_multiargs}"
  )

  list(APPEND CMAKE_MESSAGE_INDENT "bx -- ")

  message(DEBUG "BX_ARGV=\"${bx_arg_ARGV}\"")

  bx_parse_subcommand(
    ARGV ${bx_arg_ARGV} REST bx_main_argv SUB_NAME bx_sub SUB_ARGV bx_subargv
  )
  message(TRACE "bx_main_argv=\"${bx_main_argv}\"")
  message(TRACE "bx_sub=\"${bx_sub}\"")
  message(TRACE "bx_subargv=\"${bx_subargv}\"")
  message(TRACE "bx_subcommand=\"${bx_subcommand}\"")

  if (bx_sub STREQUAL "cmake")
    bx_cmake_subcommand(ARGV "${bx_subcommand}" ${bx_subargv})
  endif()
  list(POP_BACK CMAKE_MESSAGE_INDENT)
endfunction()

bx_run(ARGV ${BX_ARGV})
