# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

add_library(bx_core)
add_library(bx::core ALIAS bx_core)

target_sources(bx_core PRIVATE
  run.cpp
)
target_sources(bx_core PUBLIC
  FILE_SET HEADERS
  BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../.."
  FILES
    "run.hpp"
)

find_package(CLI11 REQUIRED)
find_package(spdlog REQUIRED)
target_link_libraries(bx_core PUBLIC
  spdlog::spdlog
  CLI11::CLI11
)
