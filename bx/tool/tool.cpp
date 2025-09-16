// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <bx/tool/tool.hpp>

#include <bx/core/args.hpp>
#include <bx/core/handle.hpp>
#include <bx/core/run.hpp>
#include <bx/core/subprocess.hpp>
#include <bx/core/user.hpp>

#include <cstdlib>
#include <fmt/format.h>
#include <string_view>
#include <vector>

namespace bx::tool {

namespace {

[[nodiscard]]
auto tool_usage_message() -> std::string {
  return "Usage: bx tool <action> [args...]\n"
         "Supported actions:\n"
         "  run <tool> [args...]    Run a tool with optional arguments\n"
         "  install [args...]       Install tools (not yet implemented)";
}

using core::HandleStatus;

} // unnamed namespace

auto handle(core::SubcommandPayload const &payload, core::User &user) -> HandleStatus {
  // Implements the "tool" subcommand to support a `run` action for developer tools.
  if (payload.args.empty()) {
    user.error("No action provided for 'tool' subcommand.\n{}", tool_usage_message());
    return HandleStatus::usage_error;
  }

  std::string_view action = payload.args[0];
  if (action == "run") {
    if (payload.args.size() < 2) {
      user.error("No tool specified.\n{}", tool_usage_message());
      return HandleStatus::usage_error;
    }

    // Compose the command: tool + args
    std::vector<std::string_view> command;
    command.reserve(payload.args.size() - 1);
    command.insert(command.end(), payload.args.begin() + 1, payload.args.end());

    user.info("Running tool: {}", core::format_command(command));

    execute_command_with_timing(user, command);
    return HandleStatus::success;
  }
  if (action == "install") {
    user.info("Tool installation feature is not enabled yet.");
    return HandleStatus::success;
  }
  user.error("Unknown action '{}' for 'tool' subcommand.\n{}", action, tool_usage_message());
  return HandleStatus::usage_error;
}

} // namespace bx::tool
