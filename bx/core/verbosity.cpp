// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <bx/core/verbosity.hpp>

#include <cstdlib>

namespace bx::core {

auto Verbosity::parse(std::string_view verbosityString) -> std::optional<Verbosity::Type> {
  if (verbosityString == "debug") {
    return Verbosity::debug;
  }
  if (verbosityString == "info") {
    return Verbosity::info;
  }
  if (verbosityString == "warning") {
    return Verbosity::warning;
  }
  if (verbosityString == "error") {
    return Verbosity::error;
  }
  return std::nullopt;
}

auto to_string(Verbosity::Type verbosity) -> std::string {
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
