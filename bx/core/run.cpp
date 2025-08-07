// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <CLI/Error.hpp>
#include <bx/core/run.hpp>

#include <CLI/CLI.hpp>
#include <CLI/Timer.hpp>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include <span>
#include <string_view>
#include <vector>

namespace bx::core {

static constexpr std::string version_string = "0.0.1";
static constexpr char app_description[] = "A better build experience for C++ projects.";

void run(std::span<std::string_view> args) {
  CLI::Timer total_timer;
  spdlog::debug("{}: [{}]", args.size(), fmt::join(args, "\", \""));

  // Parse arguments with cli11
  CLI::App app{app_description};
  app.set_help_flag("-h,--help", "Display this help message");
  app.set_help_all_flag("-H,--help-all", "Display all help messages");
  app.set_version_flag("-v,--version", version_string);
  static const int min_subcommands = 0;
  static const int max_subcommands = 1;
  app.require_subcommand(min_subcommands, max_subcommands);

  // Add `cmake` subcommand
  auto cmake_cmd = app.add_subcommand("cmake", "Execute `cmake` through `bx`.");
  cmake_cmd->allow_extras();

  // parse
  try {
    std::vector<std::string> args_vec(args.begin(), args.end());
    app.parse(args_vec);

    if (cmake_cmd->parsed()) {
      spdlog::debug("Executing cmake with arguments: {}", fmt::join(cmake_cmd->remaining(), " "));
      spdlog::warn("TODO: Pretend to call CMake.");
      CLI::Error notImplemented("NotImplemented", "TODO: Not implemented yet.",
                                CLI::ExitCodes::InvalidError);
      app.exit(notImplemented);
    } else {
      spdlog::warn("No valid subcommand provided. Use `bx --help` for usage "
                   "information.");
    }
  } catch (CLI::CallForHelp const &e) {
    app.exit(e);
  } catch (CLI::CallForAllHelp const &e) {
    app.exit(e);
  } catch (CLI::CallForVersion const &e) {
    app.exit(e);
  } catch (CLI::ParseError const &e) {
    spdlog::error("Error parsing arguments: {}", e.what());
    app.exit(e);
  }
  spdlog::info("Total execution time: {}", total_timer.make_time_str());
}

} // namespace bx::core
