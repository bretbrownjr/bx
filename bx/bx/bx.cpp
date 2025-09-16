// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <bx/core/run.hpp>

#include <vector>

namespace core = ::bx::core;

namespace bx::bx {
// Intentionally using a C array to provide a clean API for `main()`.
// NOLINTNEXTLINE(modernize-avoid-c-arrays)
void run(int argc, char *argv[]) {
  std::vector<std::string_view> args;
  args.reserve(argc);
  for (int ii = 0; ii < argc; ++ii) {
    args.emplace_back(argv[ii]);
  }
  core::run(args);
}
} // namespace bx::bx

auto main(int argc, char *argv[]) -> int {
  ::bx::bx::run(argc, argv);
  return 0;
}
