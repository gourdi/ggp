#pragma once

#include <kernel/ggo_kernel.h>

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
