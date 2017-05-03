#ifndef __GGO_BUFFER_PAINT__
#define __GGO_BUFFER_PAINT__

#include <ggo_kernel.h>
#include <ggo_pixel_buffer.h>
#include <ggo_sampling_paint.h>
#include <ggo_shapes2d.h>
#include <ggo_multi_scale_paint.h>

// Hard rectangles.
namespace ggo
{
  // No checks are performed, parameters are expected to be valid ones.
  template <pixel_buffer_format pbf>
  void paint_rect(void * buffer, int width, int height, int line_step, int left, int right, int bottom, int top, const typename pixel_buffer_format_info<pbf>::color_t & c);
}

// Shapes.
namespace ggo
{
  template <pixel_buffer_format pbf, sampling smp, typename shape_t, typename brush_t, typename blend_t>
  void paint_shape(void * buffer, int width, int height, int line_step,
    const shape_t & shape, brush_t brush, blend_t blend,
    const ggo::rect_int & clipping, const int scale_factor, const int first_scale);

  template <pixel_buffer_format pbf, sampling smp, typename shape_t, typename brush_t, typename blend_t>
  void paint_shape(void * buffer, int width, int height, int line_step,
    const shape_t & shape, brush_t brush, blend_t blend,
    const ggo::rect_int & clipping);

  template <pixel_buffer_format pbf, sampling smp, typename shape_t, typename brush_t, typename blend_t>
  void paint_shape(void * buffer, int width, int height, int line_step,
    const shape_t & shape, brush_t brush, blend_t blend);

  template <pixel_buffer_format pbf, sampling smp, typename shape_t>
  void paint_shape(void * buffer, int width, int height, int line_step,
    const shape_t & shape, const typename pixel_buffer_format_info<pbf>::color_t & color);
}

//////////////////////////////////////////////////////////////
// Implementation.

// Hard rectangles.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <pixel_buffer_format pbf>
  void paint_rect(void * buffer, int width, int height, int line_step, int left, int right, int bottom, int top, const typename pixel_buffer_format_info<pbf>::color_t & c)
  {
    process_pixel_buffer<pbf>(buffer, width, height, line_step,
      ggo::rect_int::from_left_right_bottom_top(left, right, bottom, top),
      [&](void * ptr) { write_pixel<pbf>(ptr, c); });
  }
}

// Shape.
namespace ggo
{
  template <pixel_buffer_format pbf, sampling smp, typename shape_t, typename brush_t, typename blend_t>
  void paint_shape(void * buffer, int width, int height, int line_step,
    const shape_t & shape, brush_t brush, blend_t blend,
    const ggo::rect_int & clipping, const int scale_factor, const int first_scale)
  {
    using color_t = typename pixel_buffer_format_info<pbf>::color_t;
    using format = pixel_buffer_format_info<pbf>;

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

    // Call the multi-scale paint algorithm.
    paint_multi_scale<smp>(width, height, shape,
      scale_factor, first_scale,
      brush, blend,
      read_pixel_lambda, write_pixel_lambda,
      clipping);
  }

  /////////////////////////////////////////////////////////////////////
  template <pixel_buffer_format pbf, sampling smp, typename shape_t, typename brush_t, typename blend_t>
  void paint_shape(void * buffer, int width, int height, int line_step,
    const shape_t & shape, brush_t brush, blend_t blend,
    const ggo::rect_int & clipping)
  {
    paint_shape<pbf, smp>(buffer, width, height, line_step, shape, brush, blend, clipping, 8, 2);
  }

  /////////////////////////////////////////////////////////////////////
  template <pixel_buffer_format pbf, sampling smp, typename shape_t, typename brush_t, typename blend_t>
  void paint_shape(void * buffer, int width, int height, int line_step,
    const shape_t & shape, brush_t brush, blend_t blend)
  {
    paint_shape<pbf, smp>(buffer, width, height, line_step, shape, brush, blend, ggo::rect_int::from_width_height(width, height));
  }

  /////////////////////////////////////////////////////////////////////
  template <pixel_buffer_format pbf, sampling smp, typename shape_t>
  void paint_shape(void * buffer, int width, int height, int line_step, const shape_t & shape, const typename pixel_buffer_format_info<pbf>::color_t & c)
  {
    using color_t = typename pixel_buffer_format_info<pbf>::color_t;

    auto brush = [&c](int, int) { return c; };
    auto blend = [](int, int, const color_t &, const color_t & brush_color) { return brush_color; };

    paint_shape<pbf, smp>(buffer, width, height, line_step, shape, brush, blend);
  }
}

#endif
