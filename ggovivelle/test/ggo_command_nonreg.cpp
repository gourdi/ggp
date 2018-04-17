#include <ggo_nonreg.h>
#include <ggo_command.h>

//////////////////////////////////////////////////////////////
GGO_TEST(command, regular)
{
  {
    ggo::command cmd("test");
    GGO_CHECK_EQ(cmd.name(), "test");
    GGO_CHECK(cmd.empty());
  }

  {
    ggo::command cmd("test:a=b");
    GGO_CHECK_EQ(cmd.name(), "test");
    GGO_CHECK_EQ(cmd.size(), 1);
    GGO_CHECK_EQ(*cmd["a"], "b");
  }
}

//////////////////////////////////////////////////////////////
GGO_TEST(command, filename)
{
  {
    ggo::command cmd("test", true);
    GGO_CHECK_EQ(cmd.name(), "test");
    GGO_CHECK(cmd.empty());
  }

  {
    ggo::command cmd("test:a=b", true);
    GGO_CHECK_EQ(cmd.name(), "test");
    GGO_CHECK_EQ(cmd.size(), 1);
    GGO_CHECK_EQ(*cmd["a"], "b");
  }

  {
    ggo::command cmd("d:/test:a=b", true);
    GGO_CHECK_EQ(cmd.name(), "d:/test");
    GGO_CHECK_EQ(cmd.size(), 1);
    GGO_CHECK_EQ(*cmd["a"], "b");
  }
}




