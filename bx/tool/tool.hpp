// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef BX_TOOL_TOOL_HPP
#define BX_TOOL_TOOL_HPP

#include <bx/core/handle.hpp>
#include <bx/core/subcommand.hpp>
#include <bx/core/user.hpp>

namespace bx::tool {

/// Handles the "tool" subcommand for managing developer tools.
auto handle(core::SubcommandPayload const &payload, core::User &user) -> core::HandleStatus;

} // namespace bx::tool

#endif // BX_TOOL_TOOL_HPP
