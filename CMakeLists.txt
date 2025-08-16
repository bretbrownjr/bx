# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 3.28)
project(bx
  LANGUAGES CXX
  VERSION 0.0.1
)
include(CTest)

add_subdirectory(bx)
add_subdirectory(test/core)
