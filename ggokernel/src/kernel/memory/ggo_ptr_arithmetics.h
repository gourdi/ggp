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
