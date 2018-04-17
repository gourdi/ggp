#ifndef __GGO_PIXEL_BUFFER__
#define __GGO_PIXEL_BUFFER__

#include <ggo_pixel_buffer_format.h>
#include <ggo_rect_int.h>
#include <ggo_size.h>

namespace ggo
{
  class pixel_buffer final
  {
  public:

    pixel_buffer(int width, int height, pixel_buffer_format pbf); // Line step is the natural one: no padding at the end of lines.
    pixel_buffer(int width, int height, int line_byte_step, pixel_buffer_format pbf);
    pixel_buffer(const void * buffer, int width, int height, int line_byte_step, pixel_buffer_format pbf);
    ~pixel_buffer();

    // Move.
    pixel_buffer(pixel_buffer && pf);
    void operator=(pixel_buffer && pf);

    // No copy.
    pixel_buffer(const pixel_buffer &) = delete;
    void operator=(const pixel_buffer &) = delete;

    int                 width() const { return _width; }
    int                 height() const { return _height; }
    ggo::size           size() const { return { _width, _height }; }
    int                 line_byte_step() const { return _line_byte_step; }
    pixel_buffer_format pbf() const { return _pbf; }
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

