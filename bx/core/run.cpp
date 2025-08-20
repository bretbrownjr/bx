// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <bx/core/args.hpp>
#include <bx/core/run.hpp>
#include <bx/core/subprocess.hpp>
#include <bx/core/user.hpp>
#include <bx/core/verbosity.hpp>

#include <cstddef>
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <tl/expected.hpp>

#include <cstdlib>
#include <span>
#include <string_view>

namespace bx::core {

namespace {

void set_up_file_logging() {
  try {
    static std::size_t log_file_size = 1048576 * 5; // 5 MB
    static std::size_t max_log_files = 3;    // keep 3 log files
    auto file_logger = spdlog::rotating_logger_mt("file_logger", "bx.log", log_file_size, max_log_files);
    spdlog::set_default_logger(file_logger);
    spdlog::set_level(spdlog::level::debug);
    spdlog::flush_on(spdlog::level::info);
  } catch (const spdlog::spdlog_ex& ex) {
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

void handle_run(SubcommandPayload const &payload, User &user) {
  std::vector<std::string_view> command;
  command.reserve(payload.args.size());
  command.insert(command.end(), payload.args.begin(), payload.args.end());
  if (command.empty()) {
    command.push_back("true");
    user.debug("No command was provided for the \"run\" subcommand. Running [\"true\"].");
  }
  Result const result = subprocess_call(user, command);
  if (result.exit_code != 0) {
    user.warning("Subcommand \"{}\" failed with exit code: {}", payload.name, result.exit_code);
    std::exit(result.exit_code);
  } else {
    user.debug("Subcommand \"{}\" executed successfully.", payload.name);
  }
}

std::string cmake_command() { return "cmake"; }

void handle_cmake(SubcommandPayload const &payload, User &user) {
  SubcommandPayload cmake_payload;
  cmake_payload.name = "run";
  cmake_payload.args.push_back(cmake_command());
  cmake_payload.args.insert(cmake_payload.args.end(), payload.args.begin(), payload.args.end());
  handle_run(cmake_payload, user);
}

void handle_help(SubcommandPayload const &payload, User &user) {
  user.info(help_message(payload.args.empty() ? "bx" : payload.args[0]));
}

void handle_version(SubcommandPayload const &, User &user) { user.info(version_message()); }

void handle_subcommand(SubcommandPayload const &payload, User &user) {
  using Handler = void (*)(SubcommandPayload const &, User &);
  std::unordered_map<std::string, Handler> subcommand_registry = {
      {"run", handle_run},
      {"cmake", handle_cmake},
      {"help", handle_help},
      {"version", handle_version},
  };
  auto handler_it = subcommand_registry.find(payload.name);
  check(user, handler_it != subcommand_registry.end(),
        fmt::format("Subcommand {:?} is not implemented yet.", payload.name));
  handler_it->second(payload, user);
}

} // unnamed namespace

void run(std::span<std::string_view> argv) {
  set_up_file_logging();

  User user;

  check(user, !argv.empty(),
        "Expected a program name as the first CLI argument. "
        "Got these args instead: [].");
  auto const program_name = argv[0];
  Args const args = argv.subspan(1);

  auto maybe_action = parse(user, program_name, args);
  if (!maybe_action.has_value()) {
    switch (maybe_action.error()) {
    default: {
      std::exit(EXIT_FAILURE);
    }
    }
  }
  auto const &action_ptr = maybe_action.value();
  check(user, action_ptr != nullptr, "A nullptr object was returned from parse().");

  switch (action_ptr->type) {
  case Action::Type::help: {
    user.info(help_message(program_name));
    break;
  }
  case Action::Type::version: {
    user.info(version_message());
    break;
  }
  case Action::Type::subcommand: {
    auto *const payload_ptr = std::any_cast<SubcommandPayload>(&action_ptr->payload);
    check(user, payload_ptr != nullptr, "Expected a SubcommandPayload in a subcommand action.");
    handle_subcommand(std::any_cast<SubcommandPayload>(action_ptr->payload), user);
    break;
  }
  default: {
    check(user, false, "Unexpected action type encountered.");
    break;
  }
  }
}

} // namespace bx::core
