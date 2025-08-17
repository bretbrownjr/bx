// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef BX_CORE_ARGS_HPP
#define BX_CORE_ARGS_HPP

#include <tl/expected.hpp>

#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace bx::core {

enum class Subcommand {
  cmake,
  none, // Default value for no subcommand
  run
};

enum class Status { help_requested, no_arguments, parse_error, version_requested };

class Config {
public:
  bool verbose = false;
  Subcommand subcommand = Subcommand::none; // Default to none, will be set based on subcommand
  std::vector<std::string> extra_args;      // To hold arguments for the cmake subcommand
};

tl::expected<Config, Status> parse(std::span<std::string_view> args);

} // namespace bx::core

#endif // BX_CORE_ARGS_HPP
