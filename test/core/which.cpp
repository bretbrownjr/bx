// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <bx/core/which.hpp>

#include <gtest/gtest.h>

TEST(BxCoreWhich, FindsTrue) {
  // GIVEN: Nothing

  // WHEN: Running `bx::core::which` on a basename of `true`
  auto const perhapsPath = bx::core::which("true");

  // THEN: The resulting path should be `/usr/bin/true`
  ASSERT_TRUE(perhapsPath.has_value());
  ASSERT_EQ("/usr/bin/true", perhapsPath.value());
}

TEST(BxCoreWhich, LocatesNothing) {
  // GIVEN: Nothing

  // WHEN: Running `bx::core::which` on a basename of `true`
  auto const perhapsPath = bx::core::which("not-a-real-command-probably");

  // THEN: The resulting path should be `/usr/bin/true`
  ASSERT_FALSE(perhapsPath.has_value());
}
