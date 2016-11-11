#ifndef __GGO_BUFFER_ACCESS__
#define __GGO_BUFFER_ACCESS__

#include <ggo_kernel.h>

namespace ggo
{
  enum y_direction
  {
    y_up,
    y_down
  };
}

//////////////////////////////////////////////////////////////
// Pointer offset.
namespace ggo
{
  template <typename data_t> data_t *       ptr_offset(data_t * ptr, int offset) { return reinterpret_cast<data_t *>(reinterpret_cast<uint8_t *>(ptr) + offset); }
  template <typename data_t> const data_t * ptr_offset(const data_t * ptr, int offset) { return reinterpret_cast<const data_t *>(reinterpret_cast<const uint8_t *>(ptr) + offset); }

  template <typename data_t> data_t *       ptr_offset(void * ptr, int offset) { return reinterpret_cast<data_t *>(reinterpret_cast<uint8_t *>(ptr) + offset); }
  template <typename data_t> const data_t * ptr_offset(const void * ptr, int offset) { return reinterpret_cast<const data_t *>(reinterpret_cast<const uint8_t *>(ptr) + offset); }
}

//////////////////////////////////////////////////////////////
// 1D - setters
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t, int item_byte_step = sizeof(data_t)>
  void set1d(data_t * buffer, int x, int width, const data_t & v)
  {
    static_assert(item_byte_step >= sizeof(data_t), "invalid item byte step");
    GGO_ASSERT_PTR(buffer);
    GGO_ASSERT_BTW(x, 0, width - 1);
    data_t * ptr = ptr_offset(buffer, x * item_byte_step);
    *ptr = v;
  }
}

//////////////////////////////////////////////////////////////
// 1D - getters
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t, int item_byte_step = sizeof(data_t)>
  const data_t & get1d(const data_t * buffer, int x, int width)
  {
    static_assert(item_byte_step >= sizeof(data_t), "invalid item byte step");
    GGO_ASSERT_PTR(buffer);
    GGO_ASSERT_BTW(x, 0, width - 1);
    const data_t * ptr = ptr_offset(buffer, x * item_byte_step);
    return *ptr;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, int item_byte_step = sizeof(data_t)>
  const data_t & get1d_mirror(const data_t * buffer, int x, int width)
  {
    static_assert(item_byte_step >= sizeof(data_t), "invalid item byte step");
    GGO_ASSERT_PTR(buffer);
    x = mirror_index_edge_duplicated(x, width);
    return get1d<data_t, item_byte_step>(buffer, x, width);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, int item_byte_step = sizeof(data_t)>
  const data_t & get1d_fixed_value(const data_t * buffer, int x, int width, const data_t & fixed_value)
  {
    static_assert(item_byte_step >= sizeof(data_t), "invalid item byte step");
    if (x < 0 || x >= width) { return fixed_value; }

    GGO_ASSERT_PTR(buffer);
    return get1d<data_t, item_byte_step>(buffer, x, width);
  }
}

//////////////////////////////////////////////////////////////
// 2D line and pixel access.
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <y_direction y_dir = y_up>
  void * get_line_ptr(void * buffer, int y, int height, int line_byte_step)
  {
    GGO_ASSERT_PTR(buffer);
    GGO_ASSERT_BTW(y, 0, height - 1);
    return ggo::ptr_offset<void>(buffer, (y_dir == y_down ? height - y - 1 : y) * line_byte_step);
  }

  //////////////////////////////////////////////////////////////
  template <y_direction y_dir = y_up>
  const void * get_line_ptr(const void * buffer, int y, int height, int line_byte_step)
  {
    GGO_ASSERT_PTR(buffer);
    GGO_ASSERT_BTW(y, 0, height - 1);
    return ggo::ptr_offset<void>(buffer, (y_dir == y_down ? height - y - 1 : y) * line_byte_step);
  }

  //////////////////////////////////////////////////////////////
  template <int item_byte_step, y_direction y_dir = y_up>
  void * get_pixel_ptr(void * buffer, int x, int y, int height, int line_byte_step)
  {
    static_assert(item_byte_step >= 1, "invalid item byte step");
    GGO_ASSERT_PTR(buffer);
    GGO_ASSERT_BTW(x * item_byte_step, 0, line_byte_step);
    GGO_ASSERT_BTW(y, 0, height - 1);
    void * line_ptr = get_line_ptr<y_dir>(buffer, y, height, line_byte_step);
    return ptr_offset<void>(line_ptr, x * item_byte_step);
  }

  //////////////////////////////////////////////////////////////
  template <int item_byte_step, y_direction y_dir = y_up>
  const void * get_pixel_ptr(const void * buffer, int x, int y, int height, int line_byte_step)
  {
    static_assert(item_byte_step >= 1, "invalid item byte step");
    GGO_ASSERT_PTR(buffer);
    GGO_ASSERT_BTW(x * item_byte_step, 0, line_byte_step);
    GGO_ASSERT_BTW(y, 0, height - 1);
    const void * line_ptr = get_line_ptr<y_dir>(buffer, y, height, line_byte_step);
    return ptr_offset<void>(line_ptr, x * item_byte_step);
  }
}

//////////////////////////////////////////////////////////////
// 2D - setters
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t, y_direction y_dir = y_up, int item_byte_step = sizeof(data_t)>
  void set2d(data_t * buffer, int x, int y, int height, int line_byte_step, const data_t & data)
  {
    data_t * ptr = static_cast<data_t *>(get_pixel_ptr<item_byte_step, y_dir>(buffer, x, y, height, line_byte_step));
    *ptr = data;
  }
}

//////////////////////////////////////////////////////////////
// 2D - getters
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t, y_direction y_dir = y_up, int item_byte_step = sizeof(data_t)>
  const data_t & get2d(const data_t * buffer, int x, int y, int height, int line_byte_step)
  {
    const data_t * ptr = static_cast<const data_t *>(get_pixel_ptr<item_byte_step, y_dir>(buffer, x, y, height, line_byte_step));
    return *ptr;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, y_direction y_dir = y_up, int item_byte_step = sizeof(data_t)>
  const data_t & get2d_loop(const data_t * buffer, int x, int y, int width, int height, int line_byte_step)
  {
    x = loop_index(x, width);
    y = loop_index(y, height);
    return get2d<data_t, y_dir, item_byte_step>(buffer, x, y, height, line_byte_step);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, y_direction y_dir = y_up, int item_byte_step = sizeof(data_t)>
  const data_t & get2d_mirror(const data_t * buffer, int x, int y, int width, int height, int line_byte_step)
  {
    x = mirror_index_edge_duplicated(x, width);
    y = mirror_index_edge_duplicated(y, height);
    return get2d<data_t, y_dir, item_byte_step>(buffer, x, y, height, line_byte_step);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, y_direction y_dir = y_up, int item_byte_step = sizeof(data_t)>
  const data_t & get2d_fixed_value(const data_t * buffer, int x, int y, int width, int height, int line_byte_step, const data_t & fixed_value)
  {
    if (x < 0 || x >= width) { return fixed_value; }
    if (y < 0 || y >= height) { return fixed_value; }
    return get2d<data_t, y_dir, item_byte_step>(buffer, x, y, height, line_byte_step);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, data_t fixed_value, y_direction y_dir = y_up, int item_byte_step = sizeof(data_t)>
  data_t get2d_fixed_value_t(const data_t * buffer, int x, int y, int width, int height, int line_byte_step)
  {
    if (x < 0 || x >= width) { return fixed_value; }
    if (y < 0 || y >= height) { return fixed_value; }
    return get2d<data_t, y_dir, item_byte_step>(buffer, x, y, height, line_byte_step);
  }
}

#endif
