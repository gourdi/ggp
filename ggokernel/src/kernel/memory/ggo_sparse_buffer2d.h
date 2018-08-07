#ifndef __GGO_SPARSE_BUFFER2D__
#define __GGO_SPARSE_BUFFER2D__

#include <kernel/ggo_kernel.h>
#include <kernel/ggo_rect_int.h>
#include <kernel/ggo_size.h>

namespace ggo
{
  template <typename data_t, memory_lines_order lines_order>
  class sparse_buffer2d final
  {
  public:

    sparse_buffer2d(ggo::size size, ggo::rect_int allocated_zone);
    ~sparse_buffer2d() { delete[] _buffer; }

    data_t *        data() { return _buffer; }
    const data_t *  data() const { return _buffer; }

    data_t *        get_line_ptr(int y);
    const data_t *  get_line_ptr(int y) const;

    data_t &        operator()(int x, int y) { GGO_ASSERT(_allocated_zone.is_inside({ x, y })); return get_line_ptr(y)[x]; }
    const data_t &  operator()(int x, int y) const  { GGO_ASSERT(_allocated_zone.is_inside({ x, y })); return get_line_ptr(y)[x]; }

  private:

    ggo::size _size;
    data_t * _buffer;
    rect_int _allocated_zone;
  };
}

//////////////////////////////////////////////////////////////
// Implementation.
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t, memory_lines_order lines_order>
  sparse_buffer2d<data_t, lines_order>::sparse_buffer2d(ggo::size size, ggo::rect_int allocated_zone) : _size(size)
  {
    _allocated_zone = allocated_zone;
    if (_allocated_zone.clip(size) == false)
    {
      throw new std::runtime_error("could not clip allocated rectangle");
    }

    _buffer = new data_t[_allocated_zone.width() * _allocated_zone.height()];
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, memory_lines_order lines_order>
  data_t * sparse_buffer2d<data_t, lines_order>::get_line_ptr(int y)
  {
    if constexpr(lines_order == memory_lines_order::bottom_up)
    {
      return _buffer + (y - _allocated_zone.bottom()) * _allocated_zone.width() - _allocated_zone.left();
    }
    else
    {
      return _buffer + (_allocated_zone.top() - y) * _allocated_zone.width() - _allocated_zone.left();
    }
  }
  
  //////////////////////////////////////////////////////////////
  template <typename data_t, memory_lines_order lines_order>
  const data_t * sparse_buffer2d<data_t, lines_order>::get_line_ptr(int y) const
  {
    if constexpr(lines_order == memory_lines_order::bottom_up)
    {
      return _buffer + (y - _allocated_zone.bottom()) * _allocated_zone.width() - _allocated_zone.left();
    }
    else
    {
      return _buffer + (_allocated_zone.top() - y) * _allocated_zone.width() - _allocated_zone.left();
    }
  }
}

#endif
