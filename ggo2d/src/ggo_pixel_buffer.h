#ifndef __GGO_PIXEL_BUFFER__
#define __GGO_PIXEL_BUFFER__

#include <ggo_pixel_buffer_format.h>
#include <ggo_rect_int.h>

namespace ggo
{
  class pixel_buffer final
  {
  public:

    pixel_buffer(int width, int height, pixel_buffer_format pbf, int align = 1);
    pixel_buffer(const void * buffer, int width, int height, int line_byte_step, pixel_buffer_format pbf, int align = 1);
    ~pixel_buffer();

    // Move.
    pixel_buffer(pixel_buffer && pf);
    void operator=(pixel_buffer && pf);

    // No copy.
    pixel_buffer(const pixel_buffer &) = delete;
    void operator=(const pixel_buffer &) = delete;

    int                 get_width() const { return _width; }
    int                 get_height() const { return _height; }
    int                 get_line_byte_step() const { return _line_byte_step; }
    pixel_buffer_format get_pixel_buffer_format() const { return _pbf; }
    void *              data() { return _buffer; }
    const void *        data() const { return _buffer; }

  private:

    void *              _buffer;
    int                 _width;
    int                 _height;
    int                 _line_byte_step;
    pixel_buffer_format _pbf;
  };
}

#endif

