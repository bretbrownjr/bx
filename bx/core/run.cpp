// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <bx/core/run.hpp>

#include <spdlog/spdlog.h>

namespace bx::core {

void run(std::span<std::string_view> args) {
    // Log argument[n] = <string> for each n
    for (size_t i = 0; i < args.size(); ++i) {
        spdlog::info("argument[{}] = {}", i, args[i]);
    }
}

} // namespace bx::core
