#ifndef BX_CORE_SUBCOMMAND_HPP
#define BX_CORE_SUBCOMMAND_HPP

#include <string>
#include <vector>

namespace bx::core {

/// An Action payload for a subcommand action.
struct SubcommandPayload {
  /// The name of the subcommand to execute.
  std::string name;

  /// Command-line arguments provided to the subcommand.
  /// Does not include the subcommand name itself or
  /// any application-wide flags parsed before the subcommand.
  std::vector<std::string> args;
};

} // namespace bx::core

#endif // BX_CORE_SUBCOMMAND_HPP
