# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 3.24)
project(bx.basic LANGUAGES CXX)
include(CTest)

add_executable(bx.basic)
target_sources(bx.basic PRIVATE main.cpp)
add_test(NAME bx.basic COMMAND bx.basic)
