#ifndef __GGO_MEMORY__
#define __GGO_MEMORY__

#include <ggo_rect_int.h>
#include <optional>

//////////////////////////////////////////////////////////////
// Raw buffer 2d struct.
namespace ggo
{
  struct raw_buffer2d final
  {
    raw_buffer2d(int width, int height, int line_byte_step, void * buffer)
      :
      _width(width), _height(height), _line_byte_step(line_byte_step), _buffer(buffer)
    {}

    int _width;
    int _height;
    int _line_byte_step;
    void * _buffer;
  };

  struct raw_const_buffer2d final
  {
    raw_const_buffer2d(int width, int height, int line_byte_step, const void * buffer)
      :
      _width(width), _height(height), _line_byte_step(line_byte_step), _buffer(buffer)
    {}

    int _width;
    int _height;
    int _line_byte_step;
    const void * _buffer;
  };
}

//////////////////////////////////////////////////////////////
// Pointer offset.
namespace ggo
{
  template <typename data_t> data_t *       ptr_offset(data_t * ptr, int offset) { return reinterpret_cast<data_t *>(reinterpret_cast<uint8_t *>(ptr) + offset); }
  template <typename data_t> const data_t * ptr_offset(const data_t * ptr, int offset) { return reinterpret_cast<const data_t *>(reinterpret_cast<const uint8_t *>(ptr) + offset); }

  template <int offset, typename data_t> data_t *       ptr_offset(data_t * ptr) { return reinterpret_cast<data_t *>(reinterpret_cast<uint8_t *>(ptr) + offset); }
  template <int offset, typename data_t> const data_t * ptr_offset(const data_t * ptr) { return reinterpret_cast<const data_t *>(reinterpret_cast<const uint8_t *>(ptr) + offset); }
}

#endif
