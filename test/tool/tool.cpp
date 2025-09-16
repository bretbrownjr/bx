#include <bx/tool/tool.hpp>

#include <bx/core/subprocess.hpp>

#include <gtest/gtest.h>

namespace {
using bx::core::Args;
using bx::core::Result;
using bx::core::SubcommandPayload;
using bx::core::User;
} // unnamed namespace

TEST(BxTool, NoArgs) {
  // GIVEN: A subcommand payload with no arguments
  SubcommandPayload payload{.name = "tool", .args = {}};
  User user;

  // WHEN: Handling the payload
  auto const status = bx::tool::handle(payload, user);

  // THEN: It should return a usage error
  ASSERT_EQ(status, bx::core::HandleStatus::usage_error);
}

TEST(BxTool, UnknownAction) {
  // GIVEN: A subcommand payload with an unknown action
  SubcommandPayload payload{.name = "tool", .args = {"unknown_action"}};
  User user;

  // WHEN: Handling the payload
  auto const status = bx::tool::handle(payload, user);

  // THEN: It should return a usage error
  ASSERT_EQ(status, bx::core::HandleStatus::usage_error);
}

TEST(BxTool, RunNoToolSpecified) {
  // GIVEN: A subcommand payload with the "run" action but no tool specified
  SubcommandPayload payload{.name = "tool", .args = {"run"}};
  User user;

  // WHEN: Handling the payload
  auto const status = bx::tool::handle(payload, user);

  // THEN: It should return a usage error
  ASSERT_EQ(status, bx::core::HandleStatus::usage_error);
}

namespace {
using SubprocessCallFn = std::function<Result(User const &, Args)>;

struct SubprocessCallGuard {

  SubprocessCallGuard(SubprocessCallFn fn) {
    originalFunc = bx::core::subprocess_call;
    bx::core::subprocess_call = fn;
  }
  ~SubprocessCallGuard() { bx::core::subprocess_call = originalFunc; }

  SubprocessCallFn originalFunc;
};
} // namespace

TEST(BxTool, RunTrue) {
  // GIVEN: A subcommand payload with the "run" action and "true" tool
  SubcommandPayload payload{.name = "tool", .args = {"run", "true"}};
  User user;
  SubprocessCallFn mockCall = [](User const &, Args command) -> Result {
    // Simulate the behavior of the "true" command
    if (command.size() == 1 && command[0] == "true") {
      return Result{.exit_code = 0};
    }
    return Result{.exit_code = 1}; // Simulate failure for other commands
  };
  SubprocessCallGuard guard(mockCall);

  // WHEN: Handling the payload
  auto const status = bx::tool::handle(payload, user);

  // THEN: It should return success
  ASSERT_EQ(status, bx::core::HandleStatus::success);
}
