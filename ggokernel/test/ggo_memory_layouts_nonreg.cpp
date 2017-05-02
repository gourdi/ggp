#include <ggo_nonreg.h>
#include <ggo_memory_layouts.h>
#include <ggo_data_access.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(memory_layouts, get_xy_ptr)
{
  int buffer[] = {
    10, 11, 12, 13,
    20, 21, 22, 23,
    30, 31, 32, 33,
    40, 41, 42, 43,
    50, 51, 52, 53
  };

  auto * ptr1 = ggo::lines_typed_memory_access<ggo::direction::down>::get_xy_ptr(buffer, 2, 3, 5, 4 * sizeof(int));
  GGO_CHECK_EQ(*ptr1, 22);

  auto * ptr2 = ggo::lines_typed_memory_access<ggo::direction::up>::get_xy_ptr(buffer, 2, 3, 5, 4 * sizeof(int));
  GGO_CHECK_EQ(*ptr2, 42);

  void * raw_buffer = buffer;

  auto * ptr3 = ggo::lines_raw_memory_access<ggo::direction::down, sizeof(int)>::get_xy_ptr(raw_buffer, 2, 3, 5, 4 * sizeof(int));
  GGO_CHECK_EQ(ptr3, ptr1);

  auto * ptr4 = ggo::lines_raw_memory_access<ggo::direction::up, sizeof(int)>::get_xy_ptr(raw_buffer, 2, 3, 5, 4 * sizeof(int));
  GGO_CHECK_EQ(ptr4, ptr2);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(memory_layouts, clipping)
{
  int buffer[] = {
    10, 11, 12, 13,
    20, 21, 22, 23,
    30, 31, 32, 33
  };

  ggo::raw_buffer2d buffer_data(4, 3, 4 * sizeof(int), buffer);

  {
    auto clipped_buffer = ggo::lines_raw_memory_access<ggo::direction::down, sizeof(int)>::clip(buffer_data, ggo::rect_int::from_left_right_bottom_top(1, 5, 2, 5));
    GGO_CHECK_EQ(clipped_buffer->_width, 3);
    GGO_CHECK_EQ(clipped_buffer->_height, 1);
    GGO_CHECK_EQ(clipped_buffer->_line_byte_step, 4 * sizeof(int));
    int * ptr = static_cast<int *>(clipped_buffer->_buffer);
    GGO_CHECK_EQ(ptr[0], 11);
    GGO_CHECK_EQ(ptr[1], 12);
    GGO_CHECK_EQ(ptr[2], 13);
  }

  {
    auto clipped_buffer = ggo::lines_raw_memory_access<ggo::direction::up, sizeof(int)>::clip(buffer_data, ggo::rect_int::from_left_right_bottom_top(1, 5, 2, 5));
    GGO_CHECK_EQ(clipped_buffer->_width, 3);
    GGO_CHECK_EQ(clipped_buffer->_height, 1);
    GGO_CHECK_EQ(clipped_buffer->_line_byte_step, 4 * sizeof(int));
    int * ptr = static_cast<int *>(clipped_buffer->_buffer);
    GGO_CHECK_EQ(ptr[0], 31);
    GGO_CHECK_EQ(ptr[1], 32);
    GGO_CHECK_EQ(ptr[2], 33);
  }

  ggo::raw_const_buffer2d const_buffer_data(4, 3, 4 * sizeof(int), buffer);

  {
    auto clipped_buffer = ggo::lines_raw_memory_access<ggo::direction::down, sizeof(int)>::clip(const_buffer_data, ggo::rect_int::from_left_right_bottom_top(1, 5, 2, 5));
    GGO_CHECK_EQ(clipped_buffer->_width, 3);
    GGO_CHECK_EQ(clipped_buffer->_height, 1);
    GGO_CHECK_EQ(clipped_buffer->_line_byte_step, 4 * sizeof(int));
    const int * ptr = static_cast<const int *>(clipped_buffer->_buffer);
    GGO_CHECK_EQ(ptr[0], 11);
    GGO_CHECK_EQ(ptr[1], 12);
    GGO_CHECK_EQ(ptr[2], 13);
  }

  {
    auto clipped_buffer = ggo::lines_raw_memory_access<ggo::direction::up, sizeof(int)>::clip(const_buffer_data, ggo::rect_int::from_left_right_bottom_top(1, 5, 2, 5));
    GGO_CHECK_EQ(clipped_buffer->_width, 3);
    GGO_CHECK_EQ(clipped_buffer->_height, 1);
    GGO_CHECK_EQ(clipped_buffer->_line_byte_step, 4 * sizeof(int));
    const int * ptr = static_cast<const int *>(clipped_buffer->_buffer);
    GGO_CHECK_EQ(ptr[0], 31);
    GGO_CHECK_EQ(ptr[1], 32);
    GGO_CHECK_EQ(ptr[2], 33);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(memory_layouts, vertical_iterator)
{
  const std::vector<float> buffer{
    10.f, 20.f, 30.f, 40.f, 50.f, 60.f,
    11.f, 21.f, 31.f, 41.f, 51.f, 61.f,
    12.f, 22.f, 32.f, 42.f, 52.f, 62.f,
    13.f, 23.f, 33.f, 43.f, 53.f, 63.f,
    14.f, 24.f, 34.f, 44.f, 54.f, 64.f };

  {
    auto it_down = ggo::lines_typed_memory_access<ggo::direction::down>::make_vertical_iterator<ggo::base_data_accessor<float>>(buffer.data(), 0, 5, 6 * sizeof(float));
    GGO_CHECK_FABS(it_down.read(), 14.f);
    GGO_CHECK_FABS(it_down.read(1), 13.f);
    GGO_CHECK_FABS(it_down.read(2), 12.f);

    it_down.move_nxt();
    GGO_CHECK_FABS(it_down.read(), 13.f);
    GGO_CHECK_FABS(it_down.read(1), 12.f);
    GGO_CHECK_FABS(it_down.read(2), 11.f);

    it_down.move(2);
    GGO_CHECK_FABS(it_down.read(), 11.f);
    GGO_CHECK_FABS(it_down.read(1), 10.f);
  }

  {
    auto it_down = ggo::lines_typed_memory_access<ggo::direction::down>::make_vertical_iterator<ggo::base_data_accessor<float>>(buffer.data(), 2, 5, 6 * sizeof(float));
    GGO_CHECK_FABS(it_down.read(), 34.f);
    GGO_CHECK_FABS(it_down.read(1), 33.f);
    GGO_CHECK_FABS(it_down.read(2), 32.f);

    it_down.move_nxt();
    GGO_CHECK_FABS(it_down.read(), 33.f);
    GGO_CHECK_FABS(it_down.read(1), 32.f);
    GGO_CHECK_FABS(it_down.read(2), 31.f);

    it_down.move(2);
    GGO_CHECK_FABS(it_down.read(), 31.f);
    GGO_CHECK_FABS(it_down.read(1), 30.f);
  }

  {
    auto it_up = ggo::lines_typed_memory_access<ggo::direction::up>::make_vertical_iterator<ggo::base_data_accessor<float>>(buffer.data(), 5, 5, 6 * sizeof(float));
    GGO_CHECK_FABS(it_up.read(), 60.f);
    GGO_CHECK_FABS(it_up.read(1), 61.f);
    GGO_CHECK_FABS(it_up.read(2), 62.f);

    it_up.move_nxt();
    GGO_CHECK_FABS(it_up.read(), 61.f);
    GGO_CHECK_FABS(it_up.read(1), 62.f);
    GGO_CHECK_FABS(it_up.read(2), 63.f);

    it_up.move(2);
    GGO_CHECK_FABS(it_up.read(), 63.f);
    GGO_CHECK_FABS(it_up.read(1), 64.f);
  }
}
