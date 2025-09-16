// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef BX_CORE_ARGS_HPP
#define BX_CORE_ARGS_HPP

#include <bx/core/user.hpp>

#include <tl/expected.hpp>

#include <any>
#include <memory>
#include <span>
#include <string>
#include <string_view>

namespace bx::core {

enum class Error : std::uint8_t { bad_command, internal_error, no_subcommand, unknown_subcommand };

using Args = std::span<std::string_view>;

/// Format a command for display purposes
///
/// Arguments:
/// - `args`: The command and its arguments as a span of string views.
///
/// Returns:
/// A single string representing the formatted command.
[[nodiscard]]
auto format_command(Args const &args) -> std::string;

/// An action to be performed by the application.
class Action {
public:
  enum class Type : std::uint8_t { help, version, subcommand };
  Type type;

  /// An opaque payload for performing the action. The precise value
  /// stored here depends on the type of the action being performed.
  std::any payload;
};

/// Returns a help message for the application.
[[nodiscard]]
auto help_message(std::string_view program_name) -> std::string;

/// Returns a version message for the application.
[[nodiscard]]
auto version_message() -> std::string;

/// Parse command-line arguments and return an Action to be performed.
///
/// Arguments:
/// - `user`: A User object for logging and interaction.
/// - `program_name`: The name of the program (typically argv[0]).
/// - `args`: The command-line arguments (typically argv[1..n]).
///
/// Returns:
/// Either a unique_ptr to an Action if parsing was successful,
/// or an Error indicating what went wrong.
[[nodiscard]]
auto parse(User &user, std::string_view program_name, Args args)
    -> tl::expected<std::unique_ptr<Action>, Error>;

} // namespace bx::core

#endif // BX_CORE_ARGS_HPP
