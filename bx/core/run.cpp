// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#include <bx/core/run.hpp>

#include <bx/core/args.hpp>
#include <bx/core/handle.hpp>
#include <bx/core/subprocess.hpp>
#include <bx/core/user.hpp>
#include <bx/core/verbosity.hpp>
#include <bx/tool/tool.hpp>

#include <cstddef>
#include <fmt/format.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/spdlog.h>
#include <tl/expected.hpp>

#include <cstdlib>
#include <optional>
#include <span>
#include <string_view>

namespace bx::core {

namespace {

void set_up_file_logging(std::string const &logFilePath) {
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  static constexpr std::size_t oneMegabyte = 1048576;
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  static std::size_t logFileSize = oneMegabyte * 5;
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  static std::size_t maxLogFiles = 3; // keep 3 log files
  try {
    auto fileLogger = spdlog::rotating_logger_mt("file_logger", logFilePath, logFileSize, maxLogFiles);
    fileLogger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%s:%#] %v");
    spdlog::set_default_logger(fileLogger);
    spdlog::set_level(spdlog::level::debug);
    spdlog::flush_on(spdlog::level::info);
  } catch (const spdlog::spdlog_ex &ex) {
    // Fallback to console if file logging fails
    spdlog::error("Log file creation failed: {}", ex.what());
  }
}

void check(User const &user, bool condition, std::string_view message) {
  if (condition) {
    return;
  }
  user.error("Internal error: {}", message);
  std::exit(EXIT_FAILURE);
}

[[nodiscard]]
auto cmake_command() -> std::string {
  return "cmake";
}

[[nodiscard]]
auto tool_command() -> std::string {
  return "tool";
}

[[nodiscard]]
auto run_action() -> std::string {
  return "run";
}

[[nodiscard]]
auto handle_cmake(SubcommandPayload const &payload, User &user) -> HandleStatus {
  SubcommandPayload cmakePayload;
  cmakePayload.name = tool_command();
  cmakePayload.args.emplace_back(run_action());
  cmakePayload.args.push_back(cmake_command());
  cmakePayload.args.insert(cmakePayload.args.end(), payload.args.begin(), payload.args.end());
  return tool::handle(cmakePayload, user);
}

[[nodiscard]]
auto handle_help(SubcommandPayload const &payload, User &user) -> HandleStatus {
  user.info(help_message(payload.args.empty() ? "bx" : payload.args[0]));
  return HandleStatus::success;
}

// Intentionally not using the payload for an action to print the project
// version. That context is not required.
[[nodiscard]]
// NOLINTNEXTLINE(readability-named-parameter)
auto handle_version(SubcommandPayload const &, User &user) -> HandleStatus {
  user.info(version_message());
  return HandleStatus::success;
}

[[nodiscard]]
auto handle_subcommand(SubcommandPayload const &payload, User &user) -> HandleStatus {
  std::unordered_map<std::string, Handler> const subcommandRegistry = {
      {"cmake", handle_cmake},
      {"help", handle_help},
      {"tool", tool::handle},
      {"version", handle_version},
  };
  auto const handlerIter = subcommandRegistry.find(payload.name);
  check(user, handlerIter != subcommandRegistry.end(),
        fmt::format("Subcommand {:?} is not implemented yet.", payload.name));
  return handlerIter->second(payload, user);
}

/// Extract the log file path from arguments, if specified.
/// Returns the path if --log-file=<path> is found, otherwise returns nullopt.
[[nodiscard]]
auto extract_log_file_path(Args args) -> std::optional<std::string> {
  static constexpr std::size_t logFilePrefixLength = 11; // Length of "--log-file="
  for (auto const &arg : args) {
    if (arg.starts_with("--log-file=")) {
      return std::string(arg.substr(logFilePrefixLength));
    }
  }
  return std::nullopt;
}

} // unnamed namespace

void run(std::span<std::string_view> argv) {
  check({}, !argv.empty(),
        "Expected a program name as the first CLI argument. "
        "Got these args instead: [].");

  auto const programName = argv[0];
  Args const args = argv.subspan(1);

  // Extract log file path early, before setting up logging
  auto logFilePath = extract_log_file_path(args);
  if (!logFilePath.has_value()) {
    logFilePath = get_default_log_file_path();
  }

  set_up_file_logging(*logFilePath);

  User user;

  auto maybeAction = parse(user, programName, args);
  if (!maybeAction.has_value()) {
    switch (maybeAction.error()) {
    default: {
      std::exit(EXIT_FAILURE);
    }
    }
  }
  auto const &actionPtr = maybeAction.value();
  check(user, actionPtr != nullptr, "A nullptr object was returned from parse().");

  switch (actionPtr->type) {
  case Action::Type::help: {
    user.info(help_message(programName));
    break;
  }
  case Action::Type::version: {
    user.info(version_message());
    break;
  }
  case Action::Type::subcommand: {
    auto *const payloadPtr = std::any_cast<SubcommandPayload>(&actionPtr->payload);
    check(user, payloadPtr != nullptr, "Expected a SubcommandPayload in a subcommand action.");
    std::ignore = handle_subcommand(std::any_cast<SubcommandPayload>(actionPtr->payload), user);
    break;
  }
  default: {
    check(user, false, "Unexpected action type encountered.");
    break;
  }
  }
}

} // namespace bx::core
