# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

add_executable(bx)
target_sources(bx PRIVATE
  bx.cpp
)
target_link_libraries(bx PRIVATE
  bx::core
)
install(
  TARGETS bx
  COMPONENT bx
)

add_test(
  NAME bx.help
  COMMAND bx "--help"
)
set_tests_properties(bx.help PROPERTIES
  PASS_REGULAR_EXPRESSION "A better build experience for C\\+\\+ projects"
)

add_test(
  NAME bx.help_short
  COMMAND bx "-h"
)
set_tests_properties(bx.help_short PROPERTIES
  PASS_REGULAR_EXPRESSION "A better build experience for C\\+\\+ projects"
)

add_test(
  NAME bx.verbose
  COMMAND bx "--verbose"
)
set_tests_properties(bx.verbose PROPERTIES
  PASS_REGULAR_EXPRESSION "Verbose mode enabled."
)

add_test(
  NAME bx.verbose_short
  COMMAND bx "-v"
)
set_tests_properties(bx.verbose_short PROPERTIES
  PASS_REGULAR_EXPRESSION "Verbose mode enabled."
)

add_test(
  NAME bx.no_args
  COMMAND bx
)
set_tests_properties(bx.no_args PROPERTIES
  PASS_REGULAR_EXPRESSION "No subcommand provided."
)

add_test(
  NAME bx.version
  COMMAND bx "--version"
)
set_tests_properties(bx.version PROPERTIES
  PASS_REGULAR_EXPRESSION "bx version [0-9]+\\.[0-9]+\\.[0-9]+"
)

add_test(
  NAME bx.bad_subcommand
  COMMAND bx "nonexistent"
)
set_tests_properties(bx.bad_subcommand PROPERTIES
  WILL_FAIL TRUE
  PASS_REGULAR_EXPRESSION "Unknown subcommand: nonexistent"
)

add_test(
  NAME bx.run
  COMMAND bx "run"
)
set_tests_properties(bx.run PROPERTIES
  WILL_FAIL TRUE
  PASS_REGULAR_EXPRESSION "No command was provided to run. See `bx run --help` for usage."
)

add_test(
  NAME bx.run.true
  COMMAND bx "run" "true"
)
set_tests_properties(bx.run PROPERTIES
  PASS_REGULAR_EXPRESSION "Subcommand: run."
)

add_test(
  NAME bx.cmake
  COMMAND bx "cmake"
)
set_tests_properties(bx.cmake PROPERTIES
  PASS_REGULAR_EXPRESSION "Subcommand: cmake."
)

add_test(
  NAME bx.cmake.version
  COMMAND bx "cmake" "--version"
)
# Should print the version of CMake, not bx
set_tests_properties(bx.cmake.version PROPERTIES
  PASS_REGULAR_EXPRESSION "cmake version [0-9]+\\.[0-9]+\\.[0-9]+"
)
