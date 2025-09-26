# bx

A better build experience for C++ projects.

bx is a small, fast CLI that wraps common C++ build tooling with consistent UX, helpful logging, and simple subcommands. Today it provides a thin, zero-magic pass-through to CMake and a generic tool runner with timing/logging.

## What you can do today

- Help and version: `bx --help`, `bx --version`
- Run CMake via bx: `bx cmake [args...]` (e.g., `bx cmake --version`)
- Run any tool with timing and logging: `bx tool run <tool> [args...]`
- Control verbosity of user messages and logs

## Install and build (early adopters)

Prerequisites:
- C++20 compiler (GCC/Clang)
- CMake 3.28+
- Ninja (recommended)
- Dependencies available to CMake via find_package:
  - spdlog
  - tl-expected
  - GoogleTest (tests are part of the build)

How to build:
1) Configure and build

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

2) Run tests (optional but recommended)

```sh
ctest --test-dir build --output-on-failure
```

3) Try it

```sh
./build/bx/bx/bx --help
```

Notes:
- If CMake cannot find dependencies (spdlog, tl-expected, GTest), point it to your package locations (for example via CMAKE_PREFIX_PATH) or use a package manager/toolchain (such as a vcpkg toolchain file) so find_package can resolve them.
- An install target is provided for the executable. To install into your user tree without root:

```sh
DESTDIR="$HOME/.local" cmake --install build --component bx --prefix /
```

## Usage

Global options:
- --help, -h
- --version
- --verbose[=LEVEL] where LEVEL is one of: debug, info, warning, error
- --log-level=LEVEL for internal logging (debug, info, warning, error)

Common commands:

```sh
# Show help / version
bx --help
bx --version

# Run CMake (bx forwards to the system cmake)
bx cmake --version
bx cmake -S . -B build -G Ninja

# Run any tool with timing and logging
bx tool run true
bx tool run clang-format --version

# Tune user-visible verbosity
bx --verbose=warning tool run true

# Tune internal log level (file logs)
bx --log-level=debug tool run true

# Specify custom log file location
bx --log-file=/path/to/custom.log tool run true
```

## Logging behavior

- bx writes rotating file logs to a configurable location (see `--log-file`).
- Default log location follows XDG specification: `$XDG_DATA_HOME/bx/bx.log` or falls back to `bx.log` in the current directory.
- Console output is controlled by `--verbose[=LEVEL]` and shows messages at that level or higher.
- `--log-level=LEVEL` adjusts detailed internal logging (via spdlog) without changing user-facing verbosity.

## Roadmap (short list)

- Add clang-format as a first-class tool (via `bx tool run clang-format` and lint integration)
- Provide a constrained, reproducible environment for `tool run` actions
- Add `bx tool install` with initial support for cmake and clang-format
- Introduce bx configuration files for version pinning and tool configuration
- Add a `bx lint` action that dispatches to supported tools (clang-format qualifies)

## License

Apache-2.0 WITH LLVM-exception
