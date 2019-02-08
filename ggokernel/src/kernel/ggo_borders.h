#ifndef __GGO_EDGES_MANAGEMENT__
#define __GGO_EDGES_MANAGEMENT__

#include <kernel/ggo_kernel.h>
#include <kernel/memory/ggo_ptr_arithmetics.h>

namespace ggo
{
  constexpr inline int loop_index(int i, int size) { return pos_mod(i, size); }
  constexpr inline int mirror_index(int i, int size) { i = pos_mod(i, 2 * size); return i < size ? i : 2 * size - i - 1; }
}

namespace ggo
{
  enum class border_mode
  {
    mirror,
    duplicate_edge,
    loop
  };

  template <border_mode mode>
  constexpr int inf_index(int inf, int size)
  {
    if (inf < 0)
    {
      if constexpr (mode == border_mode::mirror)
      {
        inf = ggo::mirror_index(inf, size);
      }

      if constexpr (mode == border_mode::duplicate_edge)
      {
        inf = 0;
      }

      if constexpr (mode == border_mode::loop)
      {
        inf = ggo::loop_index(inf, size);
      }
    }

    return inf;
  }

  template <border_mode mode>
  constexpr int sup_index(int sup, int size)
  {
    if (sup >= size)
    {
      if constexpr (mode == border_mode::mirror)
      {
        sup = ggo::mirror_index(sup, size);
      }

      if constexpr (mode == border_mode::duplicate_edge)
      {
        sup = size - 1;
      }

      if constexpr (mode == border_mode::loop)
      {
        sup = ggo::loop_index(sup, size);
      }
    }

    return sup;
  }
}

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
  auto get2d_zero(get2d_t get2d, int x, int y, int width, int height)
  {
    using data_t = std::result_of<get2d_t(int, int)>::type;

    if (x < 0 || x >= width || y < 0 || y >= height)
    {
      return data_t(0);
    }

    return get2d(x, y);
  }

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
    x = mirror_index(x, width);
    y = mirror_index(y, height);
    return get2d(x, y);
  }

  //////////////////////////////////////////////////////////////
  // Helper with a typed buffer.
  template <ggo::memory_lines_order lines_order, typename data_t>
  const data_t & get2d_loop(const data_t * buffer, int x, int y, int width, int height, int line_bytes_step)
  {
    x = loop_index(x, width);
    y = loop_index(y, height);

    auto ptr = get_line_ptr<lines_order>(buffer, y, height, line_bytes_step);
    ptr = ptr_offset(ptr, x * sizeof(data_t));

    return *ptr;
  }

  //////////////////////////////////////////////////////////////
  // Helper with a typed buffer.
  template <ggo::memory_lines_order lines_order, typename data_t>
  const data_t & get2d_mirror(const data_t * buffer, int x, int y, int width, int height, int line_bytes_step)
  {
    x = mirror_index(x, width);
    y = mirror_index(y, height);

    auto ptr = get_line_ptr<lines_order>(buffer, y, height, line_bytes_step);
    ptr = move_ptr(ptr, x * sizeof(data_t));

    return *ptr;
  }

  //////////////////////////////////////////////////////////////
  template <ggo::memory_lines_order lines_order, typename data_t>
  data_t get2d_zero(const data_t * buffer, int x, int y, int width, int height, int line_bytes_step)
  {
    if (x < 0 || x >= width || y < 0 || y >= height)
    {
      return data_t(0);
    }

    auto ptr = get_line_ptr<lines_order>(buffer, y, height, line_bytes_step);
    ptr = move_ptr(ptr, x * sizeof(data_t));

    return *ptr;
  }
}

#endif
