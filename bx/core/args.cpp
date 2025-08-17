// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <bx/core/args.hpp>

#include <CLI/CLI.hpp>
#include <spdlog/spdlog.h>

namespace bx::core {

static constexpr std::string version_string = "0.0.1";

static constexpr char app_description[] = "A better build experience for C++ projects.";

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

} // namespace bx::core
