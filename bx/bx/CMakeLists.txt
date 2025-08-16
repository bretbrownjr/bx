# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

add_executable(bx)
target_sources(bx PRIVATE
  bx.cpp
)
target_link_libraries(bx PRIVATE
  bx::core
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
