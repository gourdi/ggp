#ifndef __GGO_BUFFER_PAINT__
#define __GGO_BUFFER_PAINT__

#include <ggo_kernel.h>
#include <ggo_pixel_buffer.h>
#include <ggo_sampling_paint.h>
#include <ggo_multi_scale_paint.h>

// Hard rectangles.
namespace ggo
{
  // No checks are performed, parameters are expected to be valid ones.
  template <pixel_buffer_format pbf, typename color_t = typename pixel_buffer_format_info<pbf>::color_t>
  void paint_rect_fast(void * buffer, int height, int line_step, int left, int right, int bottom, int top, const color_t & c);

  template <pixel_buffer_format pbf, typename color_t = typename pixel_buffer_format_info<pbf>::color_t>
  void paint_rect_safe(void * buffer, int width, int height, int line_step, int left, int right, int bottom, int top, const color_t & c);
}

// Shapes.
namespace ggo
{
  template <pixel_buffer_format pbf, sampling smp, typename shape_t, typename brush_t, typename blend_t,
    typename color_t = typename pixel_buffer_format_info<pbf>::color_t>
  void paint_shape(void * buffer, int width, int height, int line_step, const shape_t & shape, brush_t brush, blend_t blend);

  template <pixel_buffer_format pbf, sampling smp, typename shape_t,
    typename color_t = typename pixel_buffer_format_info<pbf>::color_t>
  void paint_shape(void * buffer, int width, int height, int line_step, const shape_t & shape, const color_t & color);
}

//////////////////////////////////////////////////////////////
// Implementation.

// Hard rectangles.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <pixel_buffer_format pbf, typename color_t>
  void paint_rect_fast(void * buffer, int height, int line_step, int left, int right, int bottom, int top, const color_t & c)
  {
    process_rect_fast<pbf>(buffer, height, line_step, left, right, bottom, top, [&](void * ptr) { set_pixel<pbf, color_t>(ptr, c); });
  }

  /////////////////////////////////////////////////////////////////////
  template <pixel_buffer_format pbf, typename color_t>
  void paint_rect_safe(void * buffer, int width, int height, int line_step, int left, int right, int bottom, int top, const color_t & c)
  {
    process_rect_safe<pbf>(buffer, width, height, line_step, left, right, bottom, top, [&](void * ptr) { write_pixel<pbf>(ptr, c); });
  }
}

// Shape.
namespace ggo
{
  template <pixel_buffer_format pbf, sampling smp, typename shape_t, typename brush_t, typename blend_t, typename color_t>
  void paint_shape(void * buffer, int width, int height, int line_step, const shape_t & shape, brush_t brush, blend_t blend)
  {
    const int scale_factor = 8;
    const int first_scale = 2;

    // Lambda to retrieve pixel color.
    auto read_pixel_lambda = [&](int x, int y)
    {
      return ggo::read_pixel<pbf>(buffer, x, y, height, line_step);
    };

    // Lambda to set pixel color.
    auto write_pixel_lambda = [&](int x, int y, const color_t & c)
    {
      ggo::write_pixel<pbf>(buffer, x, y, height, line_step, c);
    };

    // Lambda that paints a block of pixels without shape sampling.
    auto paint_block_lambda = [&](const ggo::pixel_rect & block_rect)
    {
      for (int y = block_rect.bottom(); y <= block_rect.top(); ++y)
      {
        uint8_t * ptr = static_cast<uint8_t *>(get_pixel_ptr<pbf>(buffer, block_rect.left(), y, height, line_step));
        for (int x = block_rect.left(); x <= block_rect.right(); ++x)
        {
          const color_t bkgd_color = read_pixel<pbf>(ptr);
          const color_t brush_color = brush(x, y);
          const color_t pixel_color = blend(bkgd_color, brush_color);
          ggo::write_pixel<pbf>(ptr, pixel_color);
          ptr += pixel_buffer_format_info<pbf>::pixel_byte_size;
        }
      }
    };

    // Call the multi-scale paint algorithm.
    paint_multi_scale<smp>(width, height, shape,
      scale_factor, first_scale,
      brush, blend,
      read_pixel_lambda, write_pixel_lambda, paint_block_lambda);
  }

  /////////////////////////////////////////////////////////////////////
  template <pixel_buffer_format pbf, sampling smp, typename shape_t, typename color_t>
  void paint_shape(void * buffer, int width, int height, int line_step, const shape_t & shape, const color_t & c)
  {
    auto brush = [&c](int x, int y) { return c; };
    auto blend = [](const color_t & bkgd_color, const color_t & brush_color) { return brush_color; };

    paint_shape<pbf, smp>(buffer, width, height, line_step, shape, brush, blend);
  }
}

#endif
