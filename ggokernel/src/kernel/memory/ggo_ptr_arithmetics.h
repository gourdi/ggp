#ifndef __GGO_PTR_ARITHMETICS__
#define __GGO_PTR_ARITHMETICS__

#include <kernel/ggo_kernel.h>

//////////////////////////////////////////////////////////////
// Pointer offset.
namespace ggo
{
  template <typename ptr_t>
  ptr_t move_ptr(ptr_t ptr, int offset)
  {
    static_assert(std::is_pointer_v<ptr_t>);
    if constexpr (std::is_const_v<std::remove_pointer_t<ptr_t>>)
    {
      return reinterpret_cast<ptr_t>(reinterpret_cast<const uint8_t *>(ptr) + offset);
    }
    else
    {
      return reinterpret_cast<ptr_t>(reinterpret_cast<uint8_t *>(ptr) + offset);
    }
  }

  template <int offset, typename ptr_t>
  ptr_t move_ptr(ptr_t ptr)
  {
    static_assert(std::is_pointer_v<ptr_t>);
    if constexpr (std::is_const_v<std::remove_pointer_t<ptr_t>>)
    {
      return reinterpret_cast<ptr_t>(reinterpret_cast<const uint8_t *>(ptr) + offset);
    }
    else
    {
      return reinterpret_cast<ptr_t>(reinterpret_cast<uint8_t *>(ptr) + offset);
    }
  }
}

//////////////////////////////////////////////////////////////
// Line pointer.
namespace ggo
{
  template <memory_lines_order lines_order, typename ptr_t> ptr_t get_line_ptr(ptr_t ptr, int y, int height, int line_byte_step)
  {
    if constexpr (lines_order == ggo::memory_lines_order::bottom_up)
    {
      return move_ptr(ptr, y * line_byte_step);
    }
    else
    {
      return move_ptr(ptr, (height - y - 1) * line_byte_step);
    }
  }
}

#endif
