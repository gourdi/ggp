#ifndef __GGO_EDGES_MANAGEMENT__
#define __GGO_EDGES_MANAGEMENT__

#include <kernel/ggo_kernel.h>
#include <kernel/memory/ggo_ptr_arithmetics.h>

namespace ggo
{
  enum class border_mode
  {
    mirror,
    duplicate_edge,
    loop
  };

  template <border_mode mode, typename int_t>
  int_t handle_border_inf(int_t inf, int_t size)
  {
    static_assert(std::is_integral<int_t>::value);
    static_assert(std::is_signed<int_t>::value);

    if constexpr(mode == border_mode::mirror)
    {
      if (inf < 0)
      {
        inf = -inf - 1;
      }
    }

    if constexpr(mode == border_mode::duplicate_edge)
    {
      if (inf < 0)
      {
        inf = 0;
      }
    }

    if constexpr(mode == border_mode::loop)
    {
      if (inf < 0)
      {
        inf += size;
      }
    }

    return inf;
  }

  template <border_mode mode, typename int_t>
  int_t handle_border_sup(int_t sup, int_t size)
  {
    static_assert(std::is_integral<int_t>::value);
    static_assert(std::is_signed<int_t>::value);

    if constexpr(mode == border_mode::mirror)
    {
      if (sup >= size)
      {
        sup = 2 * size - sup - 1;
      }
    }

    if constexpr(mode == border_mode::duplicate_edge)
    {
      if (sup >= size)
      {
        sup = size - 1;
      }
    }

    if constexpr(mode == border_mode::loop)
    {
      if (sup >= size)
      {
        sup -= size;
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
    x = mirror_index_edge_duplicated(x, width);
    y = mirror_index_edge_duplicated(y, height);

    auto ptr = get_line_ptr<lines_order>(buffer, y, height, line_bytes_step);
    ptr = ptr_offset(ptr, x * sizeof(data_t));

    return *ptr;
  }
}

#endif
