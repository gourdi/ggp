#ifndef __GGO_EDGES_MANAGEMENT__
#define __GGO_EDGES_MANAGEMENT__

#include <kernel/ggo_kernel.h>
#include <kernel/memory/ggo_ptr_arithmetics.h>

namespace ggo
{
  template <typename data_t>
  constexpr data_t loop_index(data_t i, data_t size) { return pos_mod(i, size); }
  
  template <typename data_t>
  constexpr data_t mirror_index(data_t i, data_t size) { i = pos_mod(i, 2 * size); return i < size ? i : 2 * size - i - 1; }
}

namespace ggo
{
  enum class border_mode
  {
    mirror,
    duplicate_edge,
    loop,
    zero,
    in_memory
  };

  template <border_mode mode, typename data_t>
  constexpr data_t index(data_t i, data_t size)
  {
    if constexpr (mode == border_mode::mirror)
    {
      return ggo::mirror_index(i, size);
    }
    else if constexpr (mode == border_mode::duplicate_edge)
    {
      return ggo::clamp(i, 0, size - 1);
    }
    else if constexpr (mode == border_mode::loop)
    {
      return ggo::loop_index(i, size);
    }
    else
    {
      static_assert(mode == border_mode::in_memory);
      return i;
    }
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
    x = mirror_index(x, width);
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
