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
