// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <bx/core/which.hpp>

#include <fmt/core.h>

#include <cassert>
#include <cstdlib>
#include <filesystem>
#include <ranges>

namespace bx::core {

std::optional<std::string> which(std::string_view command) {
  char const* pathZ = std::getenv("PATH");
  if (pathZ == nullptr) {
    return std::nullopt;
  }
  std::string const path = pathZ;

  // TODO: Handle Windows-like paths using ';' as the delimiter
  static char constexpr delimiter = ':';
  auto parts = path |
	       std::views::split(delimiter) |
	       std::views::transform(
                   [](auto && subrange){
		       return std::string(subrange.begin(), subrange.end());
                   }
               );

  for (auto && part : parts) {
    auto const candidatePath = fmt::format("{}/{}", part, command); 
    std::filesystem::path candidateFsPath(candidatePath);
    if (std::filesystem::is_regular_file(candidateFsPath)) {
      return candidatePath;
    }
  }
  return std::nullopt;
}

} // namespace bx::core
