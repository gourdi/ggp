#ifndef __GGO_PIXEL_BUFFER__
#define __GGO_PIXEL_BUFFER__

#include <ggo_pixel_buffer_format.h>
#include <ggo_rect_int.h>

namespace ggo
{
  template <pixel_buffer_format pbf, typename func_t>
  void process_pixel_buffer(void * buffer, const int width, const int height, const int line_byte_step, const ggo::rect_int & rect, func_t func)
  {
    using memory_layout = pixel_buffer_format_info<pbf>::memory_layout_t;
    constexpr int pixel_byte_size = pixel_buffer_format_info<pbf>::pixel_byte_size;

    GGO_ASSERT_PTR(buffer);
    GGO_ASSERT_GE(line_byte_step, width * pixel_byte_size);

    auto clipped_buffer = memory_layout::clip(raw_buffer2d(width, height, line_byte_step, buffer), rect);
    if (clipped_buffer)
    {
      for (int y = 0; y < clipped_buffer->_height; ++y)
      {
        void * begin = memory_layout::get_y_ptr(clipped_buffer->_buffer, y, clipped_buffer->_height, clipped_buffer->_line_byte_step);
        void * end = ptr_offset(begin, clipped_buffer->_width * pixel_byte_size);
        for (void * it = begin; it != end; it = ptr_offset<pixel_byte_size>(it))
        {
          func(it);
        }
      }
    }
  }
}

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

