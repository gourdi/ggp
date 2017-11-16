#ifndef __GGO_BUFFER_PAINT__
#define __GGO_BUFFER_PAINT__

#include <ggo_kernel.h>
#include <ggo_pixel_buffer.h>
#include <ggo_sampling_paint.h>
#include <ggo_shapes2d.h>
#include <ggo_paint_shape.h>
#include <ggo_multi_scale_paint.h>

// Hard rectangles.
namespace ggo
{
  // No checks are performed, parameters are expected to be valid ones.
  template <pixel_buffer_format pbf>
  void paint_rect(void * buffer, int width, int height, int line_step, int left, int right, int bottom, int top, const typename pixel_buffer_format_info<pbf>::color_t & c);
}

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
  template <ggo::pixel_buffer_format pbf, sampling smp, typename data_t, typename color_t, typename brush_color_t, typename get_next_paint_shape_t>
  void paint_shapes(void * buffer, int width, int height, int line_step,
    get_next_paint_shape_t get_next_paint_shape,
    const ggo::rect_int & clipping);

  template <ggo::pixel_buffer_format pbf, sampling smp, typename paint_shape_t>
  void paint_shapes(void * buffer, int width, int height, int line_step, const std::vector<paint_shape_t> & shapes);
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

    auto brush = [&c](int, int) { return c; };
    auto blend = [](int, int, const color_t &, const color_t & brush_color) { return brush_color; };

    paint_shape<pbf, smp>(buffer, width, height, line_step, shape, brush, blend);
  }
}

// Paint multiple shapes.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <ggo::pixel_buffer_format pbf, sampling smp, typename data_t, typename color_t, typename brush_color_t, typename get_next_paint_shape_t>
  void paint_shapes(void * buffer, int width, int height, int line_step,
    get_next_paint_shape_t get_next_paint_shape,
    const ggo::rect_int & clipping)
  {
    const int scale_factor = 8;
    const int first_scale = 2;

    static_assert(std::is_same<color_t, typename pixel_buffer_format_info<pbf>::color_t>::value);

    // Lambda to retrieve pixel color.
    auto read_pixel_func = [&](int x, int y)
    {
      return ggo::read_pixel<pbf>(buffer, x, y, height, line_step);
    };

    // Lambda to set pixel color.
    auto write_pixel_func = [&](int x, int y, const color_t & c)
    {
      ggo::write_pixel<pbf>(buffer, x, y, height, line_step, c);
    };

    paint_multi_scale<smp, data_t, color_t, brush_color_t>(width, height, get_next_paint_shape,
      scale_factor, first_scale,
      read_pixel_func, write_pixel_func,
      clipping);
  }

  /////////////////////////////////////////////////////////////////////
  template <ggo::pixel_buffer_format pbf, sampling smp, typename paint_shape_t>
  void paint_shapes(void * buffer, int width, int height, int line_step, const std::vector<paint_shape_t> & shapes)
  {
    using shape_t = typename paint_shape_t::shape_t;
    using data_t = typename shape_t::data_t;
    using color_t = typename paint_shape_t::color_t;
    using brush_color_t = typename paint_shape_t::brush_color_t;

    auto it = shapes.begin();
    auto get_next_paint_shape = [&]() -> const paint_shape_t *
    {
      if (it == shapes.end())
      {
        return nullptr;
      }
      const auto & paint_shape = *it++;
      return &paint_shape;
    };

    paint_shapes<pbf, smp, data_t, color_t, brush_color_t>(buffer, width, height, line_step,
      get_next_paint_shape, ggo::rect_int::from_width_height(width, height));
  }
}

#endif
