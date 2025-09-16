// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef BX_CORE_SUBPROCESS_HPP
#define BX_CORE_SUBPROCESS_HPP

#include <bx/core/args.hpp>
#include <bx/core/user.hpp>

#include <functional>
#include <span>
#include <string_view>

namespace bx::core {

/// Result of a subprocess call
struct Result {
  int exit_code;
};

[[nodiscard]]
auto subprocess_call_impl(User const &user, Args command) -> Result;

// We'll deviate from the usual pattern of using camelBack for variables here
// because this is intended to be used as if it were a function.
// NOLINTNEXTLINE(readability-identifier-naming)
inline std::function<Result(User const &, Args)> subprocess_call = subprocess_call_impl;

/// Execute a command with timing and logging
///
/// Arguments:
///   user: The user interface for logging
///   args: The command and its arguments to execute
void execute_command_with_timing(User const &user, std::span<std::string_view> const& args);

} // namespace bx::core

#endif // BX_CORE_SUBPROCESS_HPP
