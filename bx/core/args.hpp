// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef BX_CORE_ARGS_HPP
#define BX_CORE_ARGS_HPP

#include <bx/core/user.hpp>

#include <tl/expected.hpp>

#include <any>
#include <memory>
#include <span>
#include <string>
#include <string_view>

namespace bx::core {

enum class Error { bad_command, internal_error, no_subcommand, unknown_subcommand };

using Args = std::span<std::string_view>;

class Action {
public:
  enum class Type { help, version, subcommand };
  Type type;
  std::any payload;
};

struct SubcommandPayload {
  std::string name;
  std::vector<std::string> args;
};

[[nodiscard]]
std::string help_message(std::string_view program_name);

[[nodiscard]]
std::string version_message();

[[nodiscard]]
tl::expected<std::unique_ptr<Action>, Error> parse(User &user, std::string_view program_name,
                                                   Args args);

} // namespace bx::core

#endif // BX_CORE_ARGS_HPP
