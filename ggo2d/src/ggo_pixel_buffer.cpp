#include <ggo_pixel_buffer.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////
ggo::pixel_buffer::pixel_buffer(int width, int height, pixel_buffer_format pbf, int align)
{
  _line_byte_step = pad(width * get_pixel_byte_size(pbf), align); // For each line to be aligned too.

#ifdef _MSC_VER
  _buffer = _aligned_malloc(_line_byte_step, align);
#else
  _buffer = aligned_alloc(align, _line_byte_step);
#endif
  _width = width;
  _height = height;
  _pbf = pbf;
}

//////////////////////////////////////////////////////////////
ggo::pixel_buffer::pixel_buffer(const void * buffer, int width, int height, int line_byte_step, pixel_buffer_format pbf, int align)
:
pixel_buffer(width, height, pbf, align)
{
  int line_byte_size = ggo::get_line_byte_size(pbf, width);
  const void * src = buffer;
  void * dst = _buffer;

  for (int y = 0; y < height; ++y)
  {
    memcpy(dst, src, line_byte_size);

    src = ggo::ptr_offset(src, line_byte_step);
    dst = ggo::ptr_offset(dst, _line_byte_step);
  }
}

//////////////////////////////////////////////////////////////
ggo::pixel_buffer::~pixel_buffer()
{
#ifdef _MSC_VER
  _aligned_free(_buffer);
#else
  free(_buffer);
#endif
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
#ifdef _MSC_VER
    _aligned_free(_buffer);
#else
    free(_buffer);
#endif

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
