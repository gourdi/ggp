#include <2d/ggo_image.h>
#include <kernel/memory/ggo_ptr_arithmetics.h>

//////////////////////////////////////////////////////////////
ggo::image::image(ggo::size s, image_format format)
:
image(s.width(), s.height(), format)
{

}

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

namespace
{
  struct line_ptr_functor
  {
    template <ggo::image_format format>
    static void * call(void * ptr, int y, int height, int line_byte_step)
    {
      using format_traits = ggo::image_format_traits<format>;

      return ggo::get_line_ptr<format_traits::lines_order>(ptr, y, height, line_byte_step);
    }

    template <ggo::image_format format>
    static const void * call(const void * ptr, int y, int height, int line_byte_step)
    {
      using format_traits = ggo::image_format_traits<format>;

      return ggo::get_line_ptr<format_traits::lines_order>(ptr, y, height, line_byte_step);
    }
  };

  struct pixel_ptr_functor
  {
    template <ggo::image_format format>
    static void * call(void * ptr, int x, int y, int height, int line_byte_step)
    {
      using format_traits = ggo::image_format_traits<format>;

      ptr = ggo::get_line_ptr<format_traits::lines_order>(ptr, y, height, line_byte_step);
      ptr = ggo::move_ptr(ptr, x * format_traits::pixel_byte_size);

      return ptr;
    }

    template <ggo::image_format format>
    static const void * call(const void * ptr, int x, int y, int height, int line_byte_step)
    {
      using format_traits = ggo::image_format_traits<format>;

      ptr = ggo::get_line_ptr<format_traits::lines_order>(ptr, y, height, line_byte_step);
      ptr = ggo::move_ptr(ptr, x * format_traits::pixel_byte_size);

      return ptr;
    }
  };
}

//////////////////////////////////////////////////////////////
void * ggo::image::line_ptr(int y)
{
  return dispatch_image_format<line_ptr_functor>(_format, _buffer, y, _height, _line_byte_step);
}

//////////////////////////////////////////////////////////////
const void * ggo::image::line_ptr(int y) const
{
  return dispatch_image_format<line_ptr_functor>(_format, _buffer, y, _height, _line_byte_step);
}

//////////////////////////////////////////////////////////////
void * ggo::image::pixel_ptr(int x, int y)
{
  return dispatch_image_format<pixel_ptr_functor>(_format, _buffer, x, y, _height, _line_byte_step);
}

//////////////////////////////////////////////////////////////
const void * ggo::image::pixel_ptr(int x, int y) const
{
  return dispatch_image_format<pixel_ptr_functor>(_format, _buffer, x, y, _height, _line_byte_step);
}
