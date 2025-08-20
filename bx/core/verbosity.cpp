// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <bx/core/verbosity.hpp>

#include <cstdlib>

namespace bx::core {

std::optional<Verbosity::Type> Verbosity::parse(std::string_view verbosity_str) {
  if (verbosity_str == "debug") {
    return Verbosity::debug;
  } else if (verbosity_str == "info") {
    return Verbosity::info;
  } else if (verbosity_str == "warning") {
    return Verbosity::warning;
  } else if (verbosity_str == "error") {
    return Verbosity::error;
  }
  return std::nullopt;
}

std::string to_string(Verbosity::Type verbosity) {
  switch (verbosity) {
  case Verbosity::debug: {
    return "debug";
  }
  case Verbosity::info: {
    return "info";
  }
  case Verbosity::warning: {
    return "warning";
  }
  case Verbosity::error: {
    return "error";
  }
  }
  std::abort();
}

} // namespace bx::core
