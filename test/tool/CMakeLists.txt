
add_test(
  NAME bx.run
  COMMAND bx "tool" "run"
)
set_tests_properties(bx.run PROPERTIES
  WILL_FAIL TRUE
  PASS_REGULAR_EXPRESSION "No command was provided to run. See `bx run --help` for usage."
)

add_test(
  NAME bx.run_true
  COMMAND bx "tool" "run" "true"
)
set_tests_properties(bx.run_true PROPERTIES
  PASS_REGULAR_EXPRESSION "Ran command"
)

add_test(
  NAME bx.lower_verbosity
  COMMAND bx "--verbose=warning" "tool" "run" "true"
)
set_tests_properties(bx.lower_verbosity PROPERTIES
  FAIL_REGULAR_EXPRESSION "Ran command"
)

find_package(GTest REQUIRED)
add_executable(bx.tool.test)
target_sources(bx.tool.test PRIVATE tool.cpp)
target_link_libraries(bx.tool.test PRIVATE bx::core GTest::gtest GTest::gmock GTest::gmock_main)

add_test(
  NAME bx.tool.test
  COMMAND bx.tool.test
)
