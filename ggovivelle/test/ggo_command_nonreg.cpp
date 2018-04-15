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

//////////////////////////////////////////////////////////////
GGO_TEST(command, parse_position)
{
  // Pixel coordinate, left align.
  GGO_CHECK_EQ(ggo::parse_margin(ggo::parameters("p=0px"), 10, 4, "p"), 0);
  GGO_CHECK_EQ(ggo::parse_margin(ggo::parameters("p=1px"), 10, 4, "p"), 1);

  // Pixel coordinate, right align.
  GGO_CHECK_EQ(ggo::parse_margin(ggo::parameters("p=-0px"), 10, 4, "p"), 6);
  GGO_CHECK_EQ(ggo::parse_margin(ggo::parameters("p=-1px"), 10, 4, "p"), 5);

  // Percentage.
  GGO_CHECK_EQ(ggo::parse_margin(ggo::parameters("p=0%"), 10, 4, "p"), 0);
  GGO_CHECK_EQ(ggo::parse_margin(ggo::parameters("p=50%"), 10, 4, "p"), 3);
  GGO_CHECK_EQ(ggo::parse_margin(ggo::parameters("p=100%"), 10, 4, "p"), 6);
}


