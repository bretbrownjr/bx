// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <bx/core/user.hpp>

#include <fmt/core.h>
#include <spdlog/spdlog.h>

namespace bx::core {

void User::show(std::string_view message, Verbosity::Type verbosity) const {
  spdlog::info("User {}:\n{}", to_string(verbosity), message);
  if (this->verbosity <= verbosity) {
    fmt::print("{}\n", message);
  }
}

} // namespace bx::core
