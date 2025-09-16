// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <bx/core/args.hpp>

#include <bx/core/subcommand.hpp>
#include <bx/core/verbosity.hpp>

#include <fmt/core.h>
#include <spdlog/spdlog.h>
#include <tl/expected.hpp>

#include <cstdlib>
#include <memory>
#include <ranges>
#include <string>

namespace bx::core {

namespace {

constexpr std::string versionString = "0.0.1";

constexpr std::string_view shortDescription = "A better build experience for C++ projects.";

std::vector<std::string_view> const subcommandNames = {"cmake", "help", "tool", "version"};

struct State {
  Args remaining;
};

enum class Status : std::uint8_t { match, no_match, bad_argument };

template <typename Tcallback>
concept OnSubcommand = std::invocable<Tcallback, std::string_view, Args> &&
                       std::same_as<std::invoke_result_t<Tcallback, std::string_view, Args>, void>;

template <OnSubcommand Tcallback>
auto parse_subcommand(User &user, State &state, Tcallback &&callback) -> Status {
  for (const auto &subcommandName : subcommandNames) {
    if (state.remaining.front() == subcommandName) {
      state.remaining = state.remaining.subspan(1); // Remove the matched subcommand
      callback(subcommandName, state.remaining);
      return Status::match;
    }
  }
  return Status::no_match;
}

template <typename Tcallback>
concept OnHelp = std::invocable<Tcallback> && std::same_as<std::invoke_result_t<Tcallback>, void>;

template <OnHelp Tcallback> auto parse_help(User &user, State &state, Tcallback &&callback) -> Status {
  if (state.remaining.front() == "--help" || state.remaining.front() == "-h") {
    state.remaining = state.remaining.subspan(1); // Remove the flag
    callback();
    return Status::match;
  }
  return Status::no_match;
}

// Parse flags like --verbose=debug or --verbose or -v
template <typename Tcallback>
concept OnVerbosity = std::invocable<Tcallback, Verbosity::Type> &&
                      std::same_as<std::invoke_result_t<Tcallback, Verbosity::Type>, void>;

template <OnVerbosity Tcallback>
auto parse_verbosity(User &user, State &state, Tcallback &&callback) -> Status {
  auto const &flag = state.remaining.front();
  if (flag == "--verbose") {
    callback(Verbosity::debug);
    state.remaining = state.remaining.subspan(1); // Remove only the --verbose flag
    return Status::match;
  }
  if (flag.starts_with("--verbose=")) {
    auto const levelString = flag.substr(10); // Length of "--verbose="
    if (auto verbosity = Verbosity::parse(levelString)) {
      callback(*verbosity);
    } else {
      user.error("Invalid verbosity level: {:?}. "
                 "Valid levels are: debug, info, warning, error.",
                 flag);
      return Status::bad_argument;
    }
    state.remaining = state.remaining.subspan(1); // Remove the --verbose=<verbosity> flag
    return Status::match;
  }
  return Status::no_match;
}

// Parse flags like --log-verbosity=<verbosity>
template <typename Tcallback>
concept OnLogVerbosity = std::invocable<Tcallback, Verbosity::Type> &&
                         std::same_as<std::invoke_result_t<Tcallback, Verbosity::Type>, void>;

template <OnLogVerbosity Tcallback>
auto parse_log_verbosity(User &user, State &state, Tcallback &&callback) -> Status {
  auto const &flag = state.remaining.front();
  if (flag.starts_with("--log-level=")) {
    auto const levelString = flag.substr(12); // Length of "--log-level="
    if (auto verbosity = Verbosity::parse(levelString)) {
      callback(*verbosity);
    } else {
      user.error("Invalid log verbosity level: {:?}. "
                 "Valid levels are: debug, info, warning, error.",
                 flag);
      return Status::bad_argument;
    }
    state.remaining = state.remaining.subspan(1); // Remove the --log-level=<verbosity> flag
    return Status::match;
  }
  return Status::no_match;
}

template <typename Tcallback>
concept OnVersion =
    std::invocable<Tcallback> && std::same_as<std::invoke_result_t<Tcallback>, void>;

template <OnVersion Tcallback>
auto parse_version(User &user, State &state, Tcallback &&callback) -> Status {
  if (state.remaining.front() == "--version") {
    state.remaining = state.remaining.subspan(1); // Remove the flag
    callback();
    return Status::match;
  }
  return Status::no_match;
}

[[nodiscard]]
auto to_spdlog_level(Verbosity::Type verbosity) -> spdlog::level::level_enum {
  switch (verbosity) {
  case Verbosity::debug:
    return spdlog::level::debug;
  case Verbosity::info:
    return spdlog::level::info;
  case Verbosity::warning:
    return spdlog::level::warn;
  case Verbosity::error:
    return spdlog::level::err;
  }
  std::abort();
}

} // unnamed namespace

auto format_command(Args const &args) -> std::string {
  return fmt::format("[{}]", fmt::join(args | std::ranges::views::transform([](const auto &arg) {
                                         return fmt::format("{:?}", arg);
                                       }),
                                       ", "));
}

auto help_message(std::string_view program_name) -> std::string {
  return fmt::format("{}\n"
                     "\n"
                     "Usage: {} [subcommand] [options]\n"
                     "\n"
                     "Commands:\n"
                     "  {}\n"
                     "\n"
                     "Global options:\n"
                     "  --help\n"
                     "        Display the concise help for this command.\n"
                     "  --verbose[=LEVEL]\n"
                     "        Set user output verbosity level (debug, info, warning, error).\n"
                     "  --log-level=LEVEL\n"
                     "        Set spdlog verbosity level (debug, info, warning, error).\n"
                     "  --version\n"
                     "        Display the {} version.\n",
                     shortDescription, program_name, fmt::join(subcommandNames, "\n  "),
                     program_name);
}

auto version_message() -> std::string { return fmt::format("bx version {}", versionString); }

auto parse(User &user, std::string_view program_name,
           Args args) -> tl::expected<std::unique_ptr<Action>, Error> {
  if (args.empty()) {
    return std::make_unique<Action>(Action::Type::help);
  }

  State state{.remaining = args};

  std::unique_ptr<Action> action;
  while (!state.remaining.empty()) {
    if (args.empty()) {
      break;
    }

    // Parse help
    {
      auto const status = parse_help(
          user, state, [&action] { action = std::make_unique<Action>(Action::Type::help); });
      if (status == Status::match) {
        break;
      }
    }
    // Parse version
    {
      auto const status = parse_version(
          user, state, [&action] { action = std::make_unique<Action>(Action::Type::version); });
      if (status == Status::match) {
        break;
      }
    }
    // Parse verbosity
    {
      auto const status = parse_verbosity(
          user, state, [&user](Verbosity::Type verbosity) { user.verbosity = verbosity; });
      if (status == Status::match) {
        continue;
      }
      if (status == Status::bad_argument) {
        return tl::unexpected(Error::bad_command);
      }
    }
    // Parse log verbosity
    {
      auto const status = parse_log_verbosity(user, state, [](Verbosity::Type verbosity) {
        spdlog::set_level(to_spdlog_level(verbosity));
      });
      if (status == Status::match) {
        continue;
      }
      if (status == Status::bad_argument) {
        return tl::unexpected(Error::bad_command);
      }
    }
    // Parse subcommands
    {
      auto const status = parse_subcommand(
          user, state, [&action](std::string_view subcommand_name, Args subcommand_args) {
            SubcommandPayload payload;
            payload.name = subcommand_name;
            payload.args.assign(subcommand_args.begin(), subcommand_args.end());
            action = std::make_unique<Action>(Action::Type::subcommand, payload);
          });
      if (status == Status::match) {
        break;
      }
    }
    // No matches!
    {
      user.error("Unexpected argument found: "
                 "{:?}.\n"
                 "\n"
                 "Usage: {} [OPTIONS] <COMMAND>\n"
                 "\n"
                 "For more information, try  `--help`.",
                 state.remaining.front(), program_name);
      return tl::unexpected(Error::bad_command);
    }
  }

  if (!action) {
    user.error("{:?} requires a subcommand but one was not provided.\n"
               "  [subcommands: {}]\n"
               "\n"
               "Usage: {} [OPTIONS] <COMMAND>\n"
               "\n"
               "For more information, try `--help`.",
               program_name, fmt::join(subcommandNames, ", "), program_name);
    return tl::unexpected(Error::no_subcommand);
  }

  return std::move(action);
}

} // namespace bx::core
