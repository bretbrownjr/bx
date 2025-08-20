// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <bx/core/subprocess.hpp>

#include <fmt/format.h>

#include <cstdlib>

namespace bx::core {

Result subprocess_call_impl(User const &user, Args command) {
  user.info("Executing command: [{:?}]", fmt::join(command, ", "));
  int const exit_code = std::system(fmt::format("{}", fmt::join(command, " ")).c_str());
  return Result{.exit_code = exit_code};
}

} // namespace bx::core
