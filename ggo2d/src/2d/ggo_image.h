#ifndef __GGO_IMAGE__
#define __GGO_IMAGE__

#include <kernel/ggo_rect_int.h>
#include <kernel/ggo_size.h>
#include <2d/ggo_image_format.h>

namespace ggo
{
  class image final
  {
  public:

    image(ggo::size s, image_format format); // Line step is the natural one: no padding at the end of lines.
    image(int width, int height, image_format format); // Line step is the natural one: no padding at the end of lines.
    image(int width, int height, int line_byte_step, image_format format);
    ~image();

    // Move.
    image(image && i);
    void operator=(image && i);

    // No copy.
    image(const image &) = delete;
    void operator=(const image &) = delete;

    int                 width() const { return _width; }
    int                 height() const { return _height; }
    ggo::size           size() const { return { _width, _height }; }
    int                 line_byte_step() const { return _line_byte_step; }
    image_format        format() const { return _format; }
    void *              data() { return _buffer; }
    const void *        data() const { return _buffer; }

    void *              line_ptr(int y);
    const void *        line_ptr(int y) const;
    void *              pixel_ptr(int x, int y);
    const void *        pixel_ptr(int x, int y) const;

  private:

    void *        _buffer;
    int           _width;
    int           _height;
    int           _line_byte_step;
    image_format  _format;
  };
}

#endif

