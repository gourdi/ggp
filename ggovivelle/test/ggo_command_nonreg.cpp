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
GGO_TEST(command, parse_margins)
{
  // Pixel coordinate, left align.
  GGO_CHECK_EQ(ggo::parse_margins(ggo::parameters("margins=0px,0px"), 10, 10, 4, 4, "margins"), ggo::pos2i(0, 0));
  GGO_CHECK_EQ(ggo::parse_margins(ggo::parameters("margins=1px,1px"), 10, 10, 4, 4, "margins"), ggo::pos2i(1, 1));

  // Pixel coordinate, right align.
  GGO_CHECK_EQ(ggo::parse_margins(ggo::parameters("margins=-0px,-0px"), 10, 10, 4, 4, "margins"), ggo::pos2i(6, 6));
  GGO_CHECK_EQ(ggo::parse_margins(ggo::parameters("margins=-1px,-1px"), 10, 10, 4, 4, "margins"), ggo::pos2i(5, 5));

  // Percentage.
  GGO_CHECK_EQ(ggo::parse_margins(ggo::parameters("margins=0%,0%"), 10, 10, 4, 4, "margins"), ggo::pos2i(0, 0));
  GGO_CHECK_EQ(ggo::parse_margins(ggo::parameters("margins=50%,50%"), 10, 10, 4, 4, "margins"), ggo::pos2i(3, 3));
  GGO_CHECK_EQ(ggo::parse_margins(ggo::parameters("margins=100%,100%"), 10, 10, 4, 4, "margins"), ggo::pos2i(6, 6));
}


