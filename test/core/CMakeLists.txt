# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

find_package(GTest REQUIRED)

add_executable(bx.core.test)
target_sources(bx.core.test PRIVATE
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
