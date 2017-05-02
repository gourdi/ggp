#ifndef __GGO_BUFFER_ACCESS__
#define __GGO_BUFFER_ACCESS__

#include <ggo_kernel.h>
#include <ggo_memory_layouts.h>
#include <ggo_data_access.h>

//////////////////////////////////////////////////////////////
// 1D - setters
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t, typename data_accessor = base_data_accessor<data_t>>
  void set1d(data_t * buffer, int x, int width, const typename data_accessor::type & v)
  {
    GGO_ASSERT_PTR(buffer);
    GGO_ASSERT_BTW(x, 0, width - 1);
    data_accessor::write(buffer + x, v)
  }
}

//////////////////////////////////////////////////////////////
// 1D - getters
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t, typename data_accessor = base_data_accessor<data_t>>
  auto get1d(const data_t * buffer, int x, int width)
  {
    GGO_ASSERT_PTR(buffer);
    GGO_ASSERT_BTW(x, 0, width - 1);
    return data_accessor::read(buffer + x);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, typename data_accessor = base_data_accessor<data_t>>
  auto get1d_mirror(const data_t * buffer, int x, int width)
  {
    x = mirror_index_edge_duplicated(x, width);
    return get1d<data_t, data_accessor>(buffer, x, width);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, typename data_accessor = base_data_accessor<data_t>>
  auto get1d_fixed_value(const data_t * buffer, int x, int width, const typename data_accessor::type & fixed_value)
  {
    if (x < 0 || x >= width) { return fixed_value; }
    return get1d<data_t, data_accessor>(buffer, x, width);
  }
}

//////////////////////////////////////////////////////////////
// 2D - setters
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename memory_layout, typename data_t, typename data_access = base_data_accessor<data_t>>
  void set2d(data_t * buffer, int x, int y, int height, int line_byte_step, const typename data_access::type & data)
  {
    data_t * ptr = memory_layout::get_xy_ptr(buffer, x, y, height, line_byte_step);
    data_access::write(ptr, data);
  }
}

//////////////////////////////////////////////////////////////
// 2D - getters
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename memory_layout, typename data_t, typename data_access = base_data_accessor<data_t>>
  auto get2d(const data_t * buffer, int x, int y, int height, int line_byte_step)
  {
    return data_access::read(memory_layout::get_xy_ptr(buffer, x, y, height, line_byte_step));
  }

  //////////////////////////////////////////////////////////////
  template <typename memory_layout, typename data_t, typename data_access = base_data_accessor<data_t>>
  auto get2d_loop(const data_t * buffer, int x, int y, int width, int height, int line_byte_step)
  {
    x = loop_index(x, width);
    y = loop_index(y, height);
    return get2d<memory_layout, data_t, data_access>(buffer, x, y, height, line_byte_step);
  }

  //////////////////////////////////////////////////////////////
  template <typename memory_layout, typename data_t, typename data_access = base_data_accessor<data_t>>
  auto get2d_mirror(const data_t * buffer, int x, int y, int width, int height, int line_byte_step)
  {
    x = mirror_index_edge_duplicated(x, width);
    y = mirror_index_edge_duplicated(y, height);
    return get2d<memory_layout, data_t, data_access>(buffer, x, y, height, line_byte_step);
  }

  //////////////////////////////////////////////////////////////
  template <typename memory_layout, typename data_t, typename data_access = base_data_accessor<data_t>>
  const data_t & get2d_fixed_value(const data_t * buffer, int x, int y, int width, int height, int line_byte_step, const typename data_access::type & fixed_value)
  {
    if (x < 0 || x >= width) { return fixed_value; }
    if (y < 0 || y >= height) { return fixed_value; }
    return get2d<memory_layout, data_t, data_access>(buffer, x, y, height, line_byte_step);
  }
}

#endif
