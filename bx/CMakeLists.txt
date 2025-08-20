# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception


add_library(bx_core)
add_library(bx::core ALIAS bx_core)

target_sources(bx_core PRIVATE
  "core/args.cpp"
  "core/run.cpp"
  "core/subprocess.cpp"
  "core/user.cpp"
  "core/verbosity.cpp"
)
target_sources(bx_core PUBLIC
  FILE_SET HEADERS
  BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/.."
  FILES
    "core/args.hpp"
    "core/run.hpp"
    "core/subprocess.hpp"
    "core/user.hpp"
    "core/verbosity.hpp"
)

find_package(CLI11 REQUIRED)
find_package(spdlog REQUIRED)
find_package(tl-expected REQUIRED)

target_link_libraries(bx_core PUBLIC
  CLI11::CLI11
  spdlog::spdlog
  tl::expected
)

add_subdirectory(bx)
