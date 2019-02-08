#ifndef __GGO_PTR_ARITHMETICS__
#define __GGO_PTR_ARITHMETICS__

#include <kernel/ggo_kernel.h>

//////////////////////////////////////////////////////////////
// Pointer offset.
namespace ggo
{
  template <typename data_t> data_t *       move_ptr(data_t * ptr, int offset) { return reinterpret_cast<data_t *>(reinterpret_cast<uint8_t *>(ptr) + offset); }
  template <typename data_t> const data_t * move_ptr(const data_t * ptr, int offset) { return reinterpret_cast<const data_t *>(reinterpret_cast<const uint8_t *>(ptr) + offset); }

  template <int offset, typename data_t> data_t *       move_ptr(data_t * ptr) { return reinterpret_cast<data_t *>(reinterpret_cast<uint8_t *>(ptr) + offset); }
  template <int offset, typename data_t> const data_t * move_ptr(const data_t * ptr) { return reinterpret_cast<const data_t *>(reinterpret_cast<const uint8_t *>(ptr) + offset); }
}

//////////////////////////////////////////////////////////////
// Line pointer.
namespace ggo
{
  template <memory_lines_order lines_order, typename data_t> data_t * get_line_ptr(data_t * ptr, int y, int height, int line_byte_step)
  {
    return move_ptr(ptr, (lines_order == ggo::memory_lines_order::bottom_up ? y : height - y - 1) * line_byte_step);
  }

  template <memory_lines_order lines_order, typename data_t> const data_t * get_line_ptr(const data_t * ptr, int y, int height, int line_byte_step)
  {
    return move_ptr(ptr, (lines_order == ggo::memory_lines_order::bottom_up ? y : height - y - 1) * line_byte_step);
  }
}

#endif
