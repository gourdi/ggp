#include <ggo_pixel_buffer.h>

//////////////////////////////////////////////////////////////
ggo::pixel_buffer::pixel_buffer(int width, int height, int line_byte_step, pixel_buffer_format pbf)
{
  _buffer = malloc(height * line_byte_step);
  _width = width;
  _height = height;
  _line_byte_step = line_byte_step;
  _pbf = pbf;
}

//////////////////////////////////////////////////////////////
ggo::pixel_buffer::pixel_buffer(const void * buffer, int width, int height, int line_byte_step, pixel_buffer_format pbf)
:
pixel_buffer(width, height, line_byte_step, pbf)
{
  memcpy(_buffer, buffer, width * line_byte_step);
}

//////////////////////////////////////////////////////////////
ggo::pixel_buffer::~pixel_buffer()
{
  free(_buffer);
}

//////////////////////////////////////////////////////////////
ggo::pixel_buffer::pixel_buffer(pixel_buffer && pf)
{
  _buffer = pf._buffer;
  _width = pf._width;
  _height = pf._height;
  _line_byte_step = pf._line_byte_step;
  _pbf = pf._pbf;

  pf._buffer = nullptr;
  pf._width = 0;
  pf._height = 0;
  pf._line_byte_step = 0;
}

//////////////////////////////////////////////////////////////
void ggo::pixel_buffer::operator=(pixel_buffer && pf)
{
  if (&pf != this)
  {
    free(_buffer);

    _buffer = pf._buffer;
    _width = pf._width;
    _height = pf._height;
    _line_byte_step = pf._line_byte_step;
    _pbf = pf._pbf;

    pf._buffer = nullptr;
    pf._width = 0;
    pf._height = 0;
    pf._line_byte_step = 0;
  }
}
