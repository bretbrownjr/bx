// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <bx/core/args.hpp>
#include <bx/core/subcommand.hpp>

#include <array>
#include <fmt/core.h>
#include <gtest/gtest.h>

TEST(BxArgs, FormatNoArgs) {
  // GIVEN: An empty argument list
  bx::core::Args args{};

  // WHEN: Formatting the command
  auto const formatted = bx::core::format_command(args);

  // THEN: It should return an empty list
  ASSERT_EQ(formatted, "[]");
}

TEST(BxArgs, FormatSomeArgs) {
  // GIVEN: A list of arguments
  std::array<std::string_view, 3> arr{"arg1", "arg2", "arg3"};
  std::span<std::string_view> const fullSpan{arr};
  bx::core::Args args = fullSpan;

  // WHEN: Formatting the command
  auto const formatted = bx::core::format_command(args);

  // THEN: It should return a formatted list
  ASSERT_EQ(formatted, R"(["arg1", "arg2", "arg3"])");
}

TEST(BxArgs, FormatSomeArgsWithSpaces) {
  // GIVEN: A list of arguments with spaces
  std::array<std::string_view, 3> arr{"arg 1", "arg2", "arg 3"};
  std::span<std::string_view> const fullSpan{arr};
  bx::core::Args args = fullSpan;

  // WHEN: Formatting the command
  auto const formatted = bx::core::format_command(args);

  // THEN: It should return a formatted list with quotes around args with spaces
  ASSERT_EQ(formatted, R"(["arg 1", "arg2", "arg 3"])");
}

TEST(BxArgs, FormatSomeArgsWithQuotes) {
  // GIVEN: A list of arguments with quotes
  std::array<std::string_view, 3> arr{R"(arg"1)", "arg2", R"(arg'3)"};
  std::span<std::string_view> const fullSpan{arr};
  bx::core::Args args = fullSpan;

  // WHEN: Formatting the command
  auto const formatted = bx::core::format_command(args);

  // THEN: It should return a formatted list with quotes around args with quotes
  ASSERT_EQ(formatted, R"(["arg\"1", "arg2", "arg'3"])");
}

TEST(BxArgs, VersionMessage) {
  // GIVEN: Nothing
  auto const expectedVersion = fmt::format("bx version {}", BX_VERSION_STRING);

  // WHEN: Getting the version message
  auto const version = bx::core::version_message();

  // THEN: It should contain the version string
  ASSERT_EQ(version, expectedVersion);
}

TEST(BxArgs, ParseNoArgs) {
  // GIVEN: No arguments
  bx::core::User user;
  std::string_view programName = "bx";
  bx::core::Args args{};

  // WHEN: Parsing the arguments
  auto const result = bx::core::parse(user, programName, args);

  // THEN: It should return an error indicating no subcommand was provided
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->get()->type, bx::core::Action::Type::help);
}

TEST(BxArgs, ParseUnknownSubcommand) {
  // GIVEN: An unknown subcommand
  bx::core::User user;
  std::string_view programName = "bx";
  std::array<std::string_view, 1> arr{"unknown"};
  std::span<std::string_view> const fullSpan{arr};
  bx::core::Args args = fullSpan;

  // WHEN: Parsing the arguments
  auto const result = bx::core::parse(user, programName, args);

  // THEN: It should return an error indicating the subcommand is unknown
  ASSERT_FALSE(result.has_value());
  ASSERT_EQ(result.error(), bx::core::Error::bad_command);
}

TEST(BxArgs, ParseHelp) {
  // GIVEN: The help argument
  bx::core::User user;
  std::string_view programName = "bx";
  std::array<std::string_view, 1> arr{"--help"};
  std::span<std::string_view> const fullSpan{arr};
  bx::core::Args args = fullSpan;

  // WHEN: Parsing the arguments
  auto const result = bx::core::parse(user, programName, args);

  // THEN: It should return an action to display help
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->get()->type, bx::core::Action::Type::help);
}

TEST(BxArgs, ParseVersion) {
  // GIVEN: The version argument
  bx::core::User user;
  std::string_view programName = "bx";
  std::array<std::string_view, 1> arr{"--version"};
  std::span<std::string_view> const fullSpan{arr};
  bx::core::Args args = fullSpan;

  // WHEN: Parsing the arguments
  auto const result = bx::core::parse(user, programName, args);

  // THEN: It should return an action to display the version
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->get()->type, bx::core::Action::Type::version);
}

TEST(BxArgs, ParseToolRunSubcommand) {
  // GIVEN: The "tool run" subcommand with a tool name
  bx::core::User user;
  std::string_view programName = "bx";
  std::array<std::string_view, 3> arr{"tool", "run", "my_tool"};
  std::span<std::string_view> const fullSpan{arr};
  bx::core::Args args = fullSpan;

  // WHEN: Parsing the arguments
  auto const result = bx::core::parse(user, programName, args);

  // THEN: It should return an action to run the tool subcommand
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->get()->type, bx::core::Action::Type::subcommand);
  auto const &payload = std::any_cast<bx::core::SubcommandPayload &>(result->get()->payload);
  ASSERT_EQ(payload.name, "tool");
  ASSERT_EQ(payload.args.size(), 2);
  ASSERT_EQ(payload.args[0], "run");
  ASSERT_EQ(payload.args[1], "my_tool");
}
