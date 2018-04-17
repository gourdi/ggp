#include <ggo_image.h>

//////////////////////////////////////////////////////////////
ggo::image::image(int width, int height, image_format format)
:
image(width, height, get_line_byte_size(format, width), format)
{

}

//////////////////////////////////////////////////////////////
ggo::image::image(int width, int height, int line_byte_step, image_format format)
{
  _buffer = malloc(height * line_byte_step);
  _width = width;
  _height = height;
  _line_byte_step = line_byte_step;
  _format = format;
}

//////////////////////////////////////////////////////////////
ggo::image::image(const void * buffer, int width, int height, int line_byte_step, image_format format)
:
image(width, height, line_byte_step, format)
{
  memcpy(_buffer, buffer, width * line_byte_step);
}

//////////////////////////////////////////////////////////////
ggo::image::~image()
{
  free(_buffer);
}

//////////////////////////////////////////////////////////////
ggo::image::image(image && i)
{
  _buffer = i._buffer;
  _width = i._width;
  _height = i._height;
  _line_byte_step = i._line_byte_step;
  _format = i._format;

  i._buffer = nullptr;
  i._width = 0;
  i._height = 0;
  i._line_byte_step = 0;
}

//////////////////////////////////////////////////////////////
void ggo::image::operator=(image && i)
{
  if (&i != this)
  {
    free(_buffer);

    _buffer = i._buffer;
    _width = i._width;
    _height = i._height;
    _line_byte_step = i._line_byte_step;
    _format = i._format;

    i._buffer = nullptr;
    i._width = 0;
    i._height = 0;
    i._line_byte_step = 0;
  }
}
