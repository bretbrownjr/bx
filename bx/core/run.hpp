// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef BX_CORE_RUN_HPP
#define BX_CORE_RUN_HPP

#include <span>
#include <string_view>

namespace bx::core {
    /// Top-level entry point into `bx`.
    ///
    /// This function is intended to be called from the `main` function of the application.
    ///
    /// Arguments:
    /// - `args`: Command-line arguments passed to the application.
    void run(std::span<std::string_view> args);
}

#endif // BX_CORE_RUN_HPP
