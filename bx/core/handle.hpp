// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef BX_CORE_HANDLE_HPP
#define BX_CORE_HANDLE_HPP

#include <bx/core/subcommand.hpp>
#include <bx/core/user.hpp>

#include <cstdint>

namespace bx::core {

enum class HandleStatus : std::uint8_t {
    success,
    usage_error,
    error
};

using Handler = HandleStatus (*)(SubcommandPayload const &, User &);

} // namespace bx::core

#endif // BX_CORE_HANDLE_HPP
