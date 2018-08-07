#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/memory/ggo_sparse_buffer2d.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(sparse_buffer2d, bottom_up)
{
  ggo::sparse_buffer2d<int, ggo::memory_lines_order::bottom_up> buffer({ 20, 10 }, ggo::rect_int::from_left_right_bottom_top(3, 5, 1, 2));

  buffer(3, 1) = 1;
  buffer(4, 1) = 2;
  buffer(5, 1) = 3;
  buffer(3, 2) = 4;
  buffer(4, 2) = 5;
  buffer(5, 2) = 6;

  GGO_CHECK_EQ(buffer.data()[0], 1);
  GGO_CHECK_EQ(buffer.data()[1], 2);
  GGO_CHECK_EQ(buffer.data()[2], 3);
  GGO_CHECK_EQ(buffer.data()[3], 4);
  GGO_CHECK_EQ(buffer.data()[4], 5);
  GGO_CHECK_EQ(buffer.data()[5], 6);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(sparse_buffer2d, top_down)
{
  ggo::sparse_buffer2d<int, ggo::memory_lines_order::top_down> buffer({ 20, 10 }, ggo::rect_int::from_left_right_bottom_top(17, 26, 1, 2));

  buffer(17, 1) = 1;
  buffer(18, 1) = 2;
  buffer(19, 1) = 3;
  buffer(17, 2) = 4;
  buffer(18, 2) = 5;
  buffer(19, 2) = 6;

  GGO_CHECK_EQ(buffer.data()[0], 4);
  GGO_CHECK_EQ(buffer.data()[1], 5);
  GGO_CHECK_EQ(buffer.data()[2], 6);
  GGO_CHECK_EQ(buffer.data()[3], 1);
  GGO_CHECK_EQ(buffer.data()[4], 2);
  GGO_CHECK_EQ(buffer.data()[5], 3);
}
