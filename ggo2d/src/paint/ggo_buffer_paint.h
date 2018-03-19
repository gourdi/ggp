#ifndef __GGO_BUFFER_PAINT__
#define __GGO_BUFFER_PAINT__

#include <vector>
#include <ggo_kernel.h>
#include <ggo_pixel_buffer.h>
#include <ggo_sampling_paint.h>
#include <ggo_shapes2d.h>
#include <ggo_brush.h>
#include <ggo_blend.h>
#include <ggo_paint_shape.h>
#include <ggo_multi_scale_paint.h>

// Paint single shape.
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

// Paint multiple shapes.
namespace ggo
{
  template <ggo::pixel_buffer_format pbf, sampling smp, typename paint_shapes_range_t>
  void paint_shapes(void * buffer, int width, int height, int line_step, const paint_shapes_range_t & shapes);

  template <ggo::pixel_buffer_format pbf, sampling smp, typename paint_shapes_range_t>
  void paint_shapes(void * buffer, int width, int height, int line_step, const paint_shapes_range_t & shapes, const ggo::rect_int & clipping);
}

//////////////////////////////////////////////////////////////
// Implementation.

// Paint single shape.
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

    solid_color_brush<color_t> brush(c);
    overwrite_blender<color_t, color_t> blender;

    paint_shape<pbf, smp>(buffer, width, height, line_step, shape, brush, blender);
  }
}

// Paint multiple shapes.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <ggo::pixel_buffer_format pbf, sampling smp, typename paint_shapes_range_t>
  void paint_shapes(void * buffer, int width, int height, int line_step, const paint_shapes_range_t & shapes)
  {
    paint_shapes<pbf, smp>(buffer, width, height, line_step, shapes, ggo::rect_int::from_width_height(width, height));
  }

  /////////////////////////////////////////////////////////////////////
  template <ggo::pixel_buffer_format pbf, sampling smp, typename paint_shapes_range_t>
  void paint_shapes(void * buffer, int width, int height, int line_step, const paint_shapes_range_t & paint_shapes, const ggo::rect_int & clipping)
  {
    const int scale_factor = 8;
    const int first_scale = 2;

    using color_t = typename pixel_buffer_format_info<pbf>::color_t;

    auto read_pixel_func = [&](int x, int y)
    {
      return ggo::read_pixel<pbf>(buffer, x, y, height, line_step);
    };

    auto write_pixel_func = [&](int x, int y, const color_t & c)
    {
      ggo::write_pixel<pbf>(buffer, x, y, height, line_step, c);
    };

    paint_multi_scale<smp>(width, height, paint_shapes, scale_factor, first_scale, read_pixel_func, write_pixel_func, clipping);
  }
}

#endif
