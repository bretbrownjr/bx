# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 3.24)
project(bx
  LANGUAGES NONE
)
include(CTest)

add_subdirectory(bin)
add_subdirectory(cmake)
set(BX_COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/bin/bx")
set(BX_INFRA_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
add_subdirectory(tests)
