#include <kernel/memory/ggo_memory_layouts.h>
#include <kernel/memory/ggo_array.h>
#include <kernel/nonreg/ggo_nonreg.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(memory_layouts, bottom_up_buffer_byte_size)
{
  ggo::rows_memory_layout<2, ggo::vertical_direction::up> memory_layout({ 4, 3 }, 10);
  
  GGO_CHECK_EQ(memory_layout.buffer_byte_size(), 30);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(memory_layouts, for_each)
{
  ggo::rows_memory_layout<2, ggo::vertical_direction::up> memory_layout({ 4, 3 }, 10);
  
  ggo::array_8u a(30);
  a.fill(0);
  void * buffer = a.data();
  
  std::vector<ggo::vec2_i> pixels;
  
  memory_layout.for_each(buffer, ggo::rect_int::from_left_right_bottom_top(0, 2, 1, 2), [&](void * ptr, int x, int y)
  {
    pixels.push_back(ggo::vec2_i(x, y));

    uint8_t * ptr_u8 = static_cast<uint8_t *>(ptr);
    *ptr_u8 = 1;
  });
  
  const std::vector<ggo::vec2_i> expected_pixels{
    {0, 1}, {1, 1}, {2,1},
    {0, 2}, {1, 2}, {2,2} };
  GGO_CHECK_EQ(pixels, expected_pixels);

  const uint8_t expected_buffer[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 1, 0, 1, 0, 0, 0, 0, 0,
    1, 0, 1, 0, 1, 0, 0, 0, 0, 0 };

  GGO_CHECK(memcmp(a.data(), expected_buffer, a.size()) == 0);
}

