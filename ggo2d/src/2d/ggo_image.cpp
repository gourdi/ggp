#include <2d/ggo_image.h>
#include <kernel/memory/ggo_ptr_arithmetics.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  image::image(ggo::size s, image_format format)
    :
    image(s, s.width() * get_pixel_byte_size(format), format)
  {

  }

  //////////////////////////////////////////////////////////////
  image::image(ggo::size s, int line_byte_step, image_format format)
    :
    image_wrapper(malloc(s.height() * s.width() * get_pixel_byte_size(format)), s, line_byte_step, format)
  {

  }

  //////////////////////////////////////////////////////////////
  image::~image()
  {
    free(_buffer);
  }

  //////////////////////////////////////////////////////////////
  image::image(image && img)
    :
    image_wrapper(img._buffer, img._size, img._line_byte_step, img._format)
  {
    img._buffer = nullptr;
  }

  //////////////////////////////////////////////////////////////
  void image::operator=(image && img)
  {
    free(_buffer);

    _size = img._size;
    _line_byte_step = img._line_byte_step;
    _format = img._format;
    _buffer = img._buffer;
    
    img._buffer = nullptr;
  }
}


