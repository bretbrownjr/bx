// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <bx/core/user.hpp>

#include <fmt/core.h>
#include <spdlog/spdlog.h>

#include <cstdlib>
#include <filesystem>

namespace bx::core {

void User::show(std::string_view message, Verbosity::Type verbosity) const {
  spdlog::info("User {}:\n{}", to_string(verbosity), message);
  if (this->verbosity <= verbosity) {
    fmt::print("{}\n", message);
  }
}

auto get_default_log_file_path() -> std::string {
  // Check for XDG_DATA_HOME first
  if (char const *xdgDataHome = std::getenv("XDG_DATA_HOME"); xdgDataHome != nullptr) {
    std::filesystem::path xdgPath(xdgDataHome);
    xdgPath /= "bx";

    // Create directory if it doesn't exist
    std::error_code errorCode;
    std::filesystem::create_directories(xdgPath, errorCode);
    if (!errorCode) {
      return (xdgPath / "bx.log").string();
    }
  }

  // Fallback to current working directory
  return "bx.log";
}

} // namespace bx::core
