#include <kernel/nonreg/ggo_nonreg.h>
#include <vivelle/ggo_command.h>
#include <vivelle/ggo_parsing.h>

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
  ggo::command cmd("my_command:size=100px,200px;pos=-4px,-8px");

  ggo::rect_int r = ggo::parse_rect(cmd.parameters(), { 1000, 1000 });
  GGO_CHECK_EQ(r.left(), 896);
  GGO_CHECK_EQ(r.bottom(), 792);
  GGO_CHECK_EQ(r.width(), 100);
  GGO_CHECK_EQ(r.height(), 200);
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

