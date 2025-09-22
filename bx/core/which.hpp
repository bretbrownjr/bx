// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef BX_CORE_HANDLE_HPP
#define BX_CORE_HANDLE_HPP

#include <optional>
#include <string>

namespace bx::core {

/// Locate the path to a given command.
///
/// Uses the following lookup rules, in order:
///
/// - The directories in the environment variable PATH, in order
///
/// Arguments:
///
/// - command: The command to look for, for instance: "bx"
///
/// Returns Either:
///
/// - nullopt: The path could not be located
/// - std::string: A resolved path to the command
std::optional<std::string> which(std::string_view command);

} // namespace bx::core

#endif // BX_CORE_HANDLE_HPP
