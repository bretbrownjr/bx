// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <bx/core/subprocess.hpp>
#include <bx/core/run.hpp>

#include <chrono>
#include <fmt/format.h>

#include <cstdlib>

using namespace std::chrono_literals;

namespace bx::core {

namespace {

auto format_timing(std::chrono::steady_clock::duration duration) -> std::string {
  if (duration <= 9999ms) {
    auto const durationMilliseconds =
        std::chrono::duration_cast<std::chrono::milliseconds>(duration);
    return fmt::format("{}ms", durationMilliseconds.count());
  }
  if (duration <= 599s) {
    auto const durationSeconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
    return fmt::format("{}s", durationSeconds.count());
  }
  auto const durationMinutes = std::chrono::duration_cast<std::chrono::minutes>(duration);
  auto const durationSeconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
  static auto constexpr secondsPerMinute = 60;
  auto remainingSeconds = durationSeconds.count() % secondsPerMinute;
  return fmt::format("{}m{}s", durationMinutes.count(), remainingSeconds);
}

} // unnamed namespace

auto subprocess_call_impl(User const &user, Args command) -> Result {
  user.info("Executing command: [{:?}]", fmt::join(command, ", "));
  int const exitCode = std::system(fmt::format("{}", fmt::join(command, " ")).c_str());
  return Result{.exit_code = exitCode};
}

void execute_command_with_timing(User const &user, std::span<std::string_view> const& args) {
  std::string const commandStr = format_command(args);

  auto const startTime = std::chrono::steady_clock::now();
  Result const result = subprocess_call(user, args);
  auto const endTime = std::chrono::steady_clock::now();

  auto const duration = endTime - startTime;
  std::string const timingMessage = format_timing(duration);

  user.info("Ran command in {}: {}", timingMessage, commandStr);

  if (result.exit_code != 0) {
    user.warning("Command failed with exit code {}: {}", result.exit_code, commandStr);
    std::exit(result.exit_code);
  } else {
    user.debug("Command executed successfully: {}", commandStr);
  }
}

} // namespace bx::core
