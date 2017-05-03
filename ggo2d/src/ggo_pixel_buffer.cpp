#include <ggo_pixel_buffer.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////
ggo::const_pixel_buffer::const_pixel_buffer(const void * buffer, int width, int height, int line_byte_step, pixel_buffer_format pbf)
:
_buffer(buffer), _width(width), _height(height), _line_byte_step(line_byte_step), _pbf(pbf)
{
}

//////////////////////////////////////////////////////////////
ggo::pixel_buffer::pixel_buffer(int width, int height, pixel_buffer_format pbf, int align)
{
  _line_byte_step = pad(width * get_pixel_byte_size(pbf), align); // For each line to be aligned too.

#ifdef _MSC_VER
  _buffer = _aligned_malloc(_line_byte_step, align);
#else
  _buffer = aligned_alloc(align, _line_byte_step);
#endif
  _delete_buffer = true;
  _width = width;
  _height = height;
  _pbf = pbf;
}

//////////////////////////////////////////////////////////////
ggo::pixel_buffer::pixel_buffer(void * buffer, int width, int height, int line_byte_step, pixel_buffer_format pbf, contruction_flag flag)
:
_delete_buffer(flag == contruction_flag::copy_buffer || flag == contruction_flag::take_ownership), _width(width), _height(height), _line_byte_step(line_byte_step), _pbf(pbf)
{
  switch (flag)
  {
  case contruction_flag::copy_buffer:
    _buffer = new char[height * line_byte_step];
    memcpy(_buffer, buffer, height * line_byte_step);
    break;
  case contruction_flag::take_ownership:
  case contruction_flag::wrap_buffer:
    _buffer = buffer;
    break;
  default:
    GGO_FAIL();
    break;
  }
}

//////////////////////////////////////////////////////////////
ggo::pixel_buffer::~pixel_buffer()
{
  if (_delete_buffer == true)
  {
#ifdef _MSC_VER
    _aligned_free(_buffer);
#else
    free(_buffer);
#endif
  }
}

//////////////////////////////////////////////////////////////
ggo::pixel_buffer::pixel_buffer(pixel_buffer && pf)
{
  if (&pf != this)
  {
    _buffer = pf._buffer;
    _delete_buffer = pf._delete_buffer;
    _width = pf._width;
    _height = pf._height;
    _line_byte_step = pf._line_byte_step;
    _pbf = pf._pbf;

    pf._buffer = nullptr;
    pf._delete_buffer = false;
    pf._width = 0;
    pf._height = 0;
    pf._line_byte_step = 0;
  }
}

//////////////////////////////////////////////////////////////
void ggo::pixel_buffer::operator=(pixel_buffer && pf)
{
  if (&pf != this)
  {
    if (_delete_buffer == true)
    {
#ifdef _MSC_VER
      _aligned_free(_buffer);
#else
      free(_buffer);
#endif
    }

    _buffer = pf._buffer;
    _delete_buffer = pf._delete_buffer;
    _width = pf._width;
    _height = pf._height;
    _line_byte_step = pf._line_byte_step;
    _pbf = pf._pbf;

    pf._buffer = nullptr;
    pf._delete_buffer = false;
    pf._width = 0;
    pf._height = 0;
    pf._line_byte_step = 0;
  }
}
