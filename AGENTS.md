# Copilot Instructions for BX Project

## Project Overview

BX is a C++ command-line tool project using CMake for build configuration. The project follows modern C++ practices and includes a modular architecture with core functionality separated into logical components.

## Project Structure

- **Root**: Contains main CMakeLists.txt and project configuration
- **bx/**: Main source code directory
  - **bx/**: Main executable entry point
  - **core/**: Core library components (args, run, subprocess, tool, user, verbosity)
  - **tool/**: Logic for the tool subcommand
- **test/**: Test suite directory with corresponding structure

## Code Style and Standards

### Language and Version
- **Language**: C++ (modern standards)
- **Build System**: CMake 3.28+
- **License**: Apache-2.0 WITH LLVM-exception

### Coding Conventions
- Use SPDX license headers in all source files: `// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception`
- Follow modern C++ best practices
- Each subdirectory of **bx/** should have its own namespace (e.g., `bx::core`, `bx::tool`)
- Naming conventions:
  - Use `snake_case` for variable and function names
  - Use `PascalCase` for class and struct names
  - Use `kebab-case` for command-line arguments
  - Use `camelBackCase` for variables
- Prefer `auto` for type deduction where appropriate
- Use `[[nodiscard]]` on functions with return types
- Use trailing return types for function declarations
- Include guards format: `BX_<PATH>_<FILE>_HPP` (e.g., `BX_CORE_TOOL_HPP`)

### File Organization
- Header files use `.hpp` extension
- Source files use `.cpp` extension
- Each module should have corresponding header and implementation files
- Keep declarations in headers, implementations in source files

## Architecture Patterns

### Core Components
- **args**: Command-line argument parsing
- **run**: Main execution logic
- **subprocess**: Process management
- **tool**: Tool/subcommand handling
- **user**: User interaction and configuration
- **verbosity**: Logging and output control

### Design Principles
- Modular design with clear separation of concerns
- Each core component handles a specific responsibility
- Clean interfaces between modules
- Consistent error handling patterns

## CMake Configuration

### Project Structure
- Root CMakeLists.txt defines project metadata and includes subdirectories
- Each major component has its own CMakeLists.txt

### Build Targets
- Main executable: `bx`
- Core library components
- Test executables in corresponding test directories

## Development Guidelines

### When Adding New Features
1. Determine which core module the feature belongs to
2. Add declarations to appropriate header files
3. Implement functionality in corresponding source files
4. Update CMakeLists.txt if new files are added
5. Add corresponding tests in the test directory
6. Follow existing namespace and naming conventions

### When Modifying Existing Code
- Maintain backward compatibility where possible
- Update related tests
- Ensure SPDX license headers remain intact
- Follow existing code style and patterns

### Testing
- Each core module should have corresponding tests
- Test structure mirrors source structure
- Use CTest framework for test execution

## Common Tasks

### Adding a New Core Module
1. Create `bx/core/module_name.hpp` and `bx/core/module_name.cpp`
2. Add appropriate namespace: `namespace bx::core`
3. Include SPDX license header
4. Update `bx/core/CMakeLists.txt` to include new files
5. Create corresponding test files in `test/core/`

### Implementing Subcommands
- Use the existing `tool.hpp` pattern for subcommand handling
- Implement in `handle_tool` function or similar pattern
- Add usage messages to `tool_usage_message`

### Command-Line Interface
- Arguments are processed through the `args` module
- Main execution flows through `core::run`

## Dependencies and External Libraries
- Standard C++ library
- CMake for build configuration
- CTest for testing framework

## Notes for AI Assistant
- Always include SPDX license headers in new files
- Maintain consistent namespace usage
- Follow the existing modular architecture
- When unsure about implementation details, refer to existing patterns in the codebase
- Prefer modern C++ features and best practices
- Keep the clean separation between the main entry point (`bx/bx/`) and core functionality (`bx/core/`)
- If a `.vscode/settings.json` exists, use it to understand how to build the project.
