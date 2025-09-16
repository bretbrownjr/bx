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
  NAME bx.no_args
  COMMAND bx
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
  NAME bx.cmake
  COMMAND bx "--verbose=warning" "cmake"
)
set_tests_properties(bx.cmake PROPERTIES
  FAIL_REGULAR_EXPRESSION "Ran command in .*: \\[\"cmake\"\\]"
)

add_test(
  NAME bx.cmake.version
  COMMAND bx "--verbose=warning" "cmake" "--version"
)
# Should print the version of CMake, not bx
set_tests_properties(bx.cmake.version PROPERTIES
  PASS_REGULAR_EXPRESSION "cmake version [0-9]+\\.[0-9]+\\.[0-9]+"
)
