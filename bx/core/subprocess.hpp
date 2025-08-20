// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef BX_CORE_SUBPROCESS_HPP
#define BX_CORE_SUBPROCESS_HPP

#include <bx/core/args.hpp>
#include <bx/core/user.hpp>

#include <functional>

namespace bx::core {

struct Result {
  int exit_code;
};

[[nodiscard]]
Result subprocess_call_impl(User const &user, Args command);

inline std::function<Result(User const &, Args)> subprocess_call = subprocess_call_impl;

} // namespace bx::core

#endif // BX_CORE_SUBPROCESS_HPP
