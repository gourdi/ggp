#include <kernel/nonreg/ggo_nonreg.h>
#include <vivelle/ggo_command.h>

//////////////////////////////////////////////////////////////
GGO_TEST(command, single_command)
{
  ggo::command cmd("test");
  GGO_CHECK_EQ(cmd.name(), "test");
  GGO_CHECK(cmd.parameters().empty());
}

//////////////////////////////////////////////////////////////
GGO_TEST(command, string)
{
  ggo::command cmd("test:a=b");
  GGO_CHECK_EQ(cmd.name(), "test");
  GGO_CHECK_EQ(cmd.parameters().size(), 1);
  GGO_CHECK_EQ(*cmd.parameters().get({ "a" }), "b");
}

//////////////////////////////////////////////////////////////
GGO_TEST(command, integer)
{
  ggo::command cmd("test:a=42");
  GGO_CHECK_EQ(cmd.name(), "test");
  GGO_CHECK_EQ(cmd.parameters().size(), 1);
  GGO_CHECK_EQ(*cmd.parameters().get<int>({ "a" }), 42);
}

//////////////////////////////////////////////////////////////
GGO_TEST(command, multiple_parameters)
{
  ggo::command cmd("test:a=42;b=43");
  GGO_CHECK_EQ(cmd.name(), "test");
  GGO_CHECK_EQ(cmd.parameters().size(), 2);
  GGO_CHECK_EQ(*cmd.parameters().get<int>({ "a" }), 42);
  GGO_CHECK_EQ(*cmd.parameters().get<int>({ "b" }), 43);
}

//////////////////////////////////////////////////////////////
GGO_TEST(command, filename)
{
  ggo::command cmd("d:/test", { ggo::command::policy::filename });
  GGO_CHECK_EQ(cmd.name(), "d:/test");
  GGO_CHECK(cmd.parameters().empty());
}

//////////////////////////////////////////////////////////////
GGO_TEST(command, filename_and_parameters)
{
  ggo::command cmd("d:/test:a=42", { ggo::command::policy::filename });
  GGO_CHECK_EQ(cmd.name(), "d:/test");
  GGO_CHECK_EQ(cmd.parameters().size(), 1);
  GGO_CHECK_EQ(*cmd.parameters().get<int>({ "a" }), 42);
}

