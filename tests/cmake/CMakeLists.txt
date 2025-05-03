# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

set(BX_INFRA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../..")

file(GLOB bx_test_subdirs RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/test-*")
foreach(bx_subdir ${bx_test_subdirs})
  string(REGEX REPLACE "^test\-" "" bx_test_id "${bx_subdir}")
  list(APPEND bx_test_ids "${bx_test_id}")
endforeach()

set(bx-basic_will_fail FALSE)
set(bx-basic_regex "blahblahdoesnotmatch")
set(bx-compile-error_will_fail FALSE)
set(bx-compile-error_regex "abcdefghijklmnopqrstuvwxyz")
set(bx-configure-error_will_fail FALSE)
set(bx-configure-error_regex "abcdefghijklmnopqrstuvwxyz")

foreach (bx_test_id ${bx_test_ids})
  set(bx_test_name "bx-${bx_test_id}")
  set(bx_test_subdir "${CMAKE_CURRENT_SOURCE_DIR}/test-${bx_test_name}")
  set(bx_test_binary_dir "${CMAKE_CURRENT_BINARY_DIR}/build-${bx_test_name}")

  add_test(
    NAME "${bx_test_name}.setup"
    COMMAND "${CMAKE_COMMAND}" -E remove_directory "${bx_test_binary_dir}"
  )

  add_test(
    NAME "${bx_test_name}"
    COMMAND
      "${BX_COMMAND}"
      "--log-level=trace"
      "${CMAKE_COMMAND}"
      -B "${bx_test_binary_dir}"
      -S "${bx_test_subdir}"
  )

  set_tests_properties("${bx_test_name}"
    PROPERTIES
      ENVIRONMENT_MODIFICATION
        "BX_INFRA_DIR=set:${BX_INFRA_DIR}"
      FIXTURES_SETUP "${bx_test_name}.cleanup"
      PASS_REGULAR_EXPRESSION "${${bx_test_name}_regex}"
      WILL_FAIL "${bx_test_name}_will_fail"
  )
endforeach()
