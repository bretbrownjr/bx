// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef BX_CORE_VERBOSITY_HPP
#define BX_CORE_VERBOSITY_HPP

#include <optional>
#include <string>

namespace bx::core {

struct Verbosity {
  Verbosity() = delete;
  Verbosity(const Verbosity &) = delete;
  ~Verbosity() = delete;

  using Type = int;
  static constexpr Type debug = 0;
  static constexpr Type info = 1;
  static constexpr Type warning = 2;
  static constexpr Type error = 3;

  // Parse a verbosity string into a verbosity type
  // Returns nullopt if the string is not a valid verbosity level
  [[nodiscard]]
  static auto parse(std::string_view verbosityString) -> std::optional<Verbosity::Type>;
};

[[nodiscard]]
auto to_string(Verbosity::Type verbosity) -> std::string;

} // namespace bx::core

#endif // BX_CORE_VERBOSITY_HPP
