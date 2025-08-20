// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef BX_CORE_USER_HPP
#define BX_CORE_USER_HPP

#include <bx/core/verbosity.hpp>

#include <fmt/core.h>

#include <string_view>

namespace bx::core {

/// A simple interface for showing messages to the user.
struct User {
  Verbosity::Type verbosity = Verbosity::info;

  // Show a message to the user.
  void show(std::string_view message, Verbosity::Type verbosity = Verbosity::info) const;

  inline void debug(std::string_view message) const {
    show(message, Verbosity::debug);
  }

  inline void info(std::string_view message) const {
    show(message, Verbosity::info);
  }

  inline void warning(std::string_view message) const {
    show(message, Verbosity::warning);
  }

  inline void error(std::string_view message) const {
    show(message, Verbosity::error);
  }

  template <typename... Targs>
  void show(Verbosity::Type verbosity, fmt::format_string<Targs...> fmt, Targs &&...args) const {
    show(fmt::vformat(fmt, fmt::make_format_args(std::forward<Targs>(args)...)), verbosity);
  }

  template<typename... Targs>
  void debug(fmt::format_string<Targs...> fmt, Targs &&...args) const {
    show(Verbosity::debug, fmt, std::forward<Targs>(args)...);
  }

  template<typename... Targs>
  void info(fmt::format_string<Targs...> fmt, Targs &&...args) const {
    show(Verbosity::info, fmt, std::forward<Targs>(args)...);
  }

  template<typename... Targs>
  void warning(fmt::format_string<Targs...> fmt, Targs &&...args) const {
    show(Verbosity::warning, fmt, std::forward<Targs>(args)...);
  }

  template<typename... Targs>
  void error(fmt::format_string<Targs...> fmt, Targs &&...args) const {
    show(Verbosity::error, fmt, std::forward<Targs>(args)...);
  }
};

} // namespace bx::core

#endif // BX_CORE_USER_HPP
