#ifndef __GGO_EDGES_MANAGEMENT__
#define __GGO_EDGES_MANAGEMENT__

#include <ggo_kernel.h>

//////////////////////////////////////////////////////////////
// 1D
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename get1d_t>
  auto get1d_mirror(get1d_t get1d, int x, int width)
  {
    x = mirror_index_edge_duplicated(x, width);
    return get1d(x);
  }

  //////////////////////////////////////////////////////////////
  // Helper with a typed buffer.
  template <typename data_t>
  data_t get1d_mirror(const data_t * buffer, int x, int width)
  {
    return get1d_mirror([buffer](int x) { return buffer[x]; }, x, width);
  }
}

//////////////////////////////////////////////////////////////
// 2D
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename get2d_t>
  auto get2d_loop(get2d_t get2d, int x, int y, int width, int height)
  {
    x = loop_index(x, width);
    y = loop_index(y, height);
    return get2d(x, y);
  }

  //////////////////////////////////////////////////////////////
  template <typename get2d_t>
  auto get2d_mirror(get2d_t get2d, int x, int y, int width, int height)
  {
    x = mirror_index_edge_duplicated(x, width);
    y = mirror_index_edge_duplicated(y, height);
    return get2d(x, y);
  }

  //////////////////////////////////////////////////////////////
  // Helper with a typed buffer.
  template <ggo::direction lines_order, typename data_t>
  const data_t & get2d_loop(const data_t * buffer, int x, int y, int width, int height, int line_bytes_step)
  {
    using memory_layout = lines_memory_layout<lines_order, sizeof(data_t)>;

    x = loop_index(x, width);
    y = loop_index(y, height);

    auto ptr = static_cast<const data_t *>(memory_layout::get_xy_ptr(buffer, x, y, height, line_bytes_step));
    return *ptr;
  }

  //////////////////////////////////////////////////////////////
  // Helper with a typed buffer.
  template <ggo::direction lines_order, typename data_t>
  const data_t & get2d_mirror(const data_t * buffer, int x, int y, int width, int height, int line_bytes_step)
  {
    using memory_layout = lines_memory_layout<lines_order, sizeof(data_t)>;

    x = mirror_index_edge_duplicated(x, width);
    y = mirror_index_edge_duplicated(y, height);

    auto ptr = static_cast<const data_t *>(memory_layout::get_xy_ptr(buffer, x, y, height, line_bytes_step));
    return *ptr;
  }
}

#endif
