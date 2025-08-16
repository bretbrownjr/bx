// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <CLI/Error.hpp>
#include <bx/core/run.hpp>

#include <CLI/CLI.hpp>
#include <CLI/Timer.hpp>
#include <spdlog/spdlog.h>
#include <tl/expected.hpp>

#include <span>
#include <string_view>

namespace bx::core {

[[maybe_unused]]
static constexpr std::string version_string = "0.0.1";

[[maybe_unused]]
static constexpr char app_description[] = "A better build experience for C++ projects.";

enum class Subcommand {
  cmake,
  none, // Default value for no subcommand
  run
};

enum class Status { help_requested, no_arguments, parse_error, version_requested };

class Config {
public:
  bool verbose = false;
  Subcommand subcommand = Subcommand::none; // Default to none, will be set based on subcommand
  std::vector<std::string> extra_args;      // To hold arguments for the cmake subcommand
};

tl::expected<Config, Status> parse(std::span<std::string_view> args) {
  if (args.empty()) {
    spdlog::error("No arguments provided. Expected at least the program name.");
    return tl::unexpected(Status::no_arguments);
  }
  CLI::App app{"bx", app_description};

  Config config;
  app.add_flag("-v,--verbose", config.verbose, "Enable verbose output");
  app.set_version_flag("-V,--version", version_string);

  // Convert span to vector of strings for CLI11
  std::vector<std::string> str_args;
  str_args.reserve(args.size());
  auto const args_only = args.subspan(1); // Skip the first argument (program name)
  for (const auto &arg : args_only) {
    str_args.emplace_back(arg);
  }

  // Support the cmake subcommand
  auto cmake_sub = app.add_subcommand("cmake", "Run CMake with better defaults");
  cmake_sub->callback([&config]() { config.subcommand = Subcommand::cmake; });
  cmake_sub->allow_extras();
  cmake_sub->add_option("cmake_args", config.extra_args, "Arguments to pass to CMake")
      ->expected(-1);

  // Support the run subcommand
  auto run_sub = app.add_subcommand("run", "Run any program");
  run_sub->callback([&config]() { config.subcommand = Subcommand::run; });
  run_sub->allow_extras();
  run_sub->add_option("run_args", config.extra_args, "Arguments to pass to the program")
      ->expected(-1);

  try {
    app.parse(str_args);
  } catch (const CLI::CallForHelp &e) {
    spdlog::info("{}", app.help());
    return tl::unexpected(Status::help_requested);
  } catch (const CLI::CallForVersion &e) {
    spdlog::info("bx version {}", app.version());
    return tl::unexpected(Status::version_requested);
  } catch (const CLI::ParseError &e) {
    spdlog::error("{}", e.what());
    return tl::unexpected(Status::parse_error);
  }

  return config;
}

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
