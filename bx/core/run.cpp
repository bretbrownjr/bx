// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <CLI/Error.hpp>
#include <bx/core/run.hpp>
#include <bx/core/args.hpp>

#include <CLI/CLI.hpp>
#include <CLI/Timer.hpp>
#include <spdlog/spdlog.h>
#include <tl/expected.hpp>

#include <span>
#include <string_view>

namespace bx::core {

void run(std::span<std::string_view> args) {
  spdlog::debug("{}: [{}]", args.size(), fmt::join(args, "\", \""));
  // Time the program
  CLI::Timer timer;

  auto const maybe_config = parse(args);
  if (!maybe_config) {
    // Transform the error enums into status codes
    // User output was already emitted in parse()
    switch (maybe_config.error()) {
    case Status::help_requested: {
      return;
    }
    case Status::no_arguments: {
      std::exit(EXIT_FAILURE);
    }
    case Status::parse_error: {
      std::exit(EXIT_FAILURE);
    }
    case Status::version_requested: {
      return;
    }
    }
  }

  Config const config = maybe_config.value();

  // Set log level based on verbose flag
  if (config.verbose) {
    spdlog::set_level(spdlog::level::debug);
    spdlog::debug("Verbose mode enabled.");
  }

  if (config.subcommand == Subcommand::none) {
    spdlog::error("No subcommand provided. See `bx --help` for available subcommands.");
    std::exit(EXIT_FAILURE);
  }

  spdlog::info("Completed in {}.", timer.make_time_str());
}

} // namespace bx::core
