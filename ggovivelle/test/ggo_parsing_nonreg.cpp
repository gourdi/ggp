#include <kernel/nonreg/ggo_nonreg.h>
#include <vivelle/ggo_command.h>
#include <vivelle/ggo_parsing.h>

//////////////////////////////////////////////////////////////
GGO_TEST(parsing, margins)
{
  ggo::parameters params("margins=10px,15px");

  ggo::pos2_i pos = parse_margins(params, "margins", { 1000, 1000 }, { 100, 200 });
  GGO_CHECK_EQ(pos.x(), 10);
  GGO_CHECK_EQ(pos.y(), 15);
}

//////////////////////////////////////////////////////////////
GGO_TEST(parsing, margins_percent)
{
  ggo::parameters params("margins=10%,15%");

  ggo::pos2_i pos = parse_margins(params, "margins", { 150, 150 }, { 50, 50 });
  GGO_CHECK_EQ(pos.x(), 10);
  GGO_CHECK_EQ(pos.y(), 15);
}

//////////////////////////////////////////////////////////////
GGO_TEST(parsing, margins_right_align)
{
  ggo::parameters params("margins=-2px,-3px");

  ggo::pos2_i pos = parse_margins(params, "margins", { 10, 10 }, { 3, 5 });
  GGO_CHECK_EQ(pos.x(), 5);
  GGO_CHECK_EQ(pos.y(), 2);
}

//////////////////////////////////////////////////////////////
GGO_TEST(parsing, rect_pixels)
{
  ggo::command cmd("my_command:size=100px,200px;pos=4px,8px");

  ggo::rect_int r = ggo::parse_rect(cmd.parameters(), { 1000, 1000 });
  GGO_CHECK_EQ(r.left(), 4);
  GGO_CHECK_EQ(r.bottom(), 8);
  GGO_CHECK_EQ(r.width(), 100);
  GGO_CHECK_EQ(r.height(), 200);
}

//////////////////////////////////////////////////////////////
GGO_TEST(parsing, rect_pixels_right_align)
{
  ggo::command cmd("my_command:size=3px,5px;pos=-2px,-3px");

  ggo::rect_int r = ggo::parse_rect(cmd.parameters(), { 10, 10 });
  GGO_CHECK_EQ(r.left(), 5);
  GGO_CHECK_EQ(r.bottom(), 2);
  GGO_CHECK_EQ(r.width(), 3);
  GGO_CHECK_EQ(r.height(), 5);
}

//////////////////////////////////////////////////////////////
GGO_TEST(parsing, rect_percents)
{
  ggo::command cmd("my_command:size=50%,10%;pos=0%,50%");

  ggo::rect_int r = ggo::parse_rect(cmd.parameters(), { 1000, 1000 });
  GGO_CHECK_EQ(r.left(), 0);
  GGO_CHECK_EQ(r.bottom(), 450);
  GGO_CHECK_EQ(r.width(), 500);
  GGO_CHECK_EQ(r.height(), 100);
}




