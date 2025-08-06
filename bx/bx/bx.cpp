// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <bx/core/run.hpp>

#include <vector>

namespace core = ::bx::core;

namespace bx::bx {
void run(int argc, char* argv[]) {
    std::vector<std::string_view> args;
    args.reserve(argc);
    for (int ii=0; ii<argc; ++ii) {
        args.emplace_back(argv[ii]);
    }
    core::run(args);
}
} // namespace bx::bx

int main(int argc, char* argv[]) {
    ::bx::bx::run(argc, argv);
    return 0;
}
