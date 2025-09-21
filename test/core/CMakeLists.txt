# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

find_package(GTest REQUIRED)

add_executable(bx.core.test)
# args.cpp must have a preprocessor definition for BX_VERSION_STRING
# so that it can test that the version string matches the CMake version.
set_source_files_properties(args.cpp
    PROPERTIES
        COMPILE_DEFINITIONS "BX_VERSION_STRING=\"${PROJECT_VERSION}\""
)
target_sources(bx.core.test PRIVATE
    args.cpp
    core.cpp
)
target_link_libraries(bx.core.test PRIVATE
    bx::core
    GTest::gtest
    GTest::gtest_main
)
add_test(
    NAME bx.core.test
    COMMAND bx.core.test
)
