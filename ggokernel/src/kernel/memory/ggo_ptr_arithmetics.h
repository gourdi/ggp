#pragma once

#include <kernel/ggo_kernel.h>

//////////////////////////////////////////////////////////////
// Pointer offset.
namespace ggo
{
  template <typename ptr_t>
  ptr_t move_ptr(ptr_t ptr, std::ptrdiff_t offset)
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

  template <std::ptrdiff_t offset, typename ptr_t>
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
//
////////////////////////////////////////////////////////////////
//// Line pointer.
//namespace ggo
//{
//  template <lines_order memory_lines_order, typename ptr_t>
//  ptr_t get_line_ptr(ptr_t ptr, int y, int height, int line_byte_step)
//  {
//    if constexpr (memory_lines_order == ggo::lines_order::up)
//    {
//      return move_ptr(ptr, y * line_byte_step);
//    }
//    else
//    {
//      return move_ptr(ptr, (height - y - 1) * line_byte_step);
//    }
//  }
//
//  template <lines_order memory_lines_order, int pixel_byte_size, typename ptr_t>
//  ptr_t get_pixel_ptr(ptr_t ptr, int x, int y, int height, int line_byte_step)
//  {
//    return move_ptr(get_line_ptr<memory_lines_order>(ptr, y, height, line_byte_step), x * pixel_byte_size);
//  }
//
//  template <typename ptr_t>
//  ptr_t get_line_ptr(ptr_t ptr, int y, int height, int line_byte_step, lines_order memory_lines_order)
//  {
//    if (memory_lines_order == ggo::lines_order::up)
//    {
//      return move_ptr(ptr, y * line_byte_step);
//    }
//    else
//    {
//      return move_ptr(ptr, (height - y - 1) * line_byte_step);
//    }
//  }
//
//  template <typename ptr_t>
//  ptr_t get_pixel_ptr(ptr_t ptr, int x, int y, int height, int line_byte_step, lines_order memory_lines_order, int pixel_byte_size)
//  {
//    return move_ptr(get_line_ptr(ptr, y, height, line_byte_step, memory_lines_order), x * pixel_byte_size);
//  }
//}
//
