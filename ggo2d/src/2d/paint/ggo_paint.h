#ifndef __GGO_PAINT__
#define __GGO_PAINT__

#include <vector>
#include <kernel/ggo_kernel.h>
#include <kernel/math/shapes_2d/ggo_shapes2d.h>
#include <2d/ggo_image_format.h>
#include <2d/paint/ggo_pixel_sampling.h>
#include <2d/paint/ggo_brush.h>
#include <2d/paint/ggo_blend.h>
#include <2d/paint/ggo_paint_shape_abc.h>
#include <2d/paint/ggo_multi_scale_paint.h>

// Paint single shape.
namespace ggo
{
  template <image_format format, sampling smp, typename shape_t, typename brush_t, typename blend_t>
  void paint_shape(void * buffer, int width, int height, int line_step,
    const shape_t & shape, brush_t brush, blend_t blend,
    const ggo::rect_int & clipping, const int scale_factor, const int first_scale);

  template <image_format format, sampling smp, typename shape_t, typename brush_t, typename blend_t>
  void paint_shape(void * buffer, int width, int height, int line_step,
    const shape_t & shape, brush_t brush, blend_t blend,
    const ggo::rect_int & clipping);

  template <image_format format, sampling smp, typename shape_t, typename brush_t, typename blend_t>
  void paint_shape(void * buffer, int width, int height, int line_step,
    const shape_t & shape, brush_t brush, blend_t blend);

  template <image_format format, sampling smp, typename shape_t>
  void paint_shape(void * buffer, int width, int height, int line_step,
    const shape_t & shape, const typename image_format_traits<format>::color_t & c);

  template <image_format format, sampling smp, typename shape_t>
  void paint_shape(void * buffer, int width, int height, int line_step,
    const shape_t & shape, const typename image_format_traits<format>::color_t & c, float opacity);

  template <image_format format, sampling smp, typename shape_t>
  void paint_shape(void * buffer, int width, int height, int line_step,
    const shape_t & shape, const typename image_format_traits<format>::color_t & c,
    const ggo::rect_int & clipping);
}

// Paint multiple shapes.
namespace ggo
{
  template <ggo::image_format format, sampling smp, typename paint_shapes_range_t>
  void paint_shapes(void * buffer, int width, int height, int line_step, const paint_shapes_range_t & shapes);

  template <ggo::image_format format, sampling smp, typename paint_shapes_range_t>
  void paint_shapes(void * buffer, int width, int height, int line_step, const paint_shapes_range_t & shapes, const ggo::rect_int & clipping);

  template <ggo::image_format format, sampling smp, typename paint_shape_t>
  void paint_shapes(void * buffer, int width, int height, int line_step, const std::vector<paint_shape_t> & shapes);

  template <ggo::image_format format, sampling smp, typename paint_shape_t>
  void paint_shapes(void * buffer, int width, int height, int line_step, const std::vector<paint_shape_t> & shapes, const ggo::rect_int & clipping);
}

//////////////////////////////////////////////////////////////
// Implementation.

// Paint single shape.
namespace ggo
{
  template <image_format format, sampling smp, typename shape_t, typename brush_t, typename blend_t>
  void paint_shape(void * buffer, int width, int height, int line_step,
    const shape_t & shape, brush_t brush, blend_t blend,
    const ggo::rect_int & clipping, const int scale_factor, const int first_scale)
  {
    using format_traits = image_format_traits<format>;
    using color_t = typename format_traits::color_t;

    // Lambda to retrieve pixel color.
    auto read_pixel_lambda = [&](int x, int y)
    {
      return ggo::read_pixel<format>(buffer, x, y, height, line_step);
    };

    // Lambda to set pixel color.
    auto write_pixel_lambda = [&](int x, int y, const color_t & c)
    {
      ggo::write_pixel<format>(buffer, x, y, height, line_step, c);
    };

    // Call the multi-scale paint algorithm.
    paint_multi_scale<smp>(width, height, shape,
      scale_factor, first_scale,
      brush, blend,
      read_pixel_lambda, write_pixel_lambda,
      clipping);
  }

  /////////////////////////////////////////////////////////////////////
  template <image_format format, sampling smp, typename shape_t, typename brush_t, typename blend_t>
  void paint_shape(void * buffer, int width, int height, int line_step,
    const shape_t & shape, brush_t brush, blend_t blend,
    const ggo::rect_int & clipping)
  {
    paint_shape<format, smp>(buffer, width, height, line_step, shape, brush, blend, clipping, 8, 2);
  }

  /////////////////////////////////////////////////////////////////////
  template <image_format format, sampling smp, typename shape_t, typename brush_t, typename blend_t>
  void paint_shape(void * buffer, int width, int height, int line_step,
    const shape_t & shape, brush_t brush, blend_t blend)
  {
    paint_shape<format, smp>(buffer, width, height, line_step, shape, brush, blend, ggo::rect_int::from_width_height(width, height));
  }

  /////////////////////////////////////////////////////////////////////
  template <image_format format, sampling smp, typename shape_t>
  void paint_shape(void * buffer, int width, int height, int line_step, const shape_t & shape, const typename image_format_traits<format>::color_t & c)
  {
    using color_t = typename image_format_traits<format>::color_t;

    solid_color_brush<color_t> brush(c);
    overwrite_blender<color_t, color_t> blender;

    paint_shape<format, smp>(buffer, width, height, line_step, shape, brush, blender);
  }

  /////////////////////////////////////////////////////////////////////
  template <image_format format, sampling smp, typename shape_t>
  void paint_shape(void * buffer, int width, int height, int line_step,
    const shape_t & shape, const typename image_format_traits<format>::color_t & c, float opacity)
  {
    using color_t = typename image_format_traits<format>::color_t;

    solid_color_brush<color_t> brush(c);
    alpha_blender<color_t> blender(opacity);

    paint_shape<format, smp>(buffer, width, height, line_step, shape, brush, blender);
  }

  /////////////////////////////////////////////////////////////////////
  template <image_format format, sampling smp, typename shape_t>
  void paint_shape(void * buffer, int width, int height, int line_step,
    const shape_t & shape, const typename image_format_traits<format>::color_t & c,
    const ggo::rect_int & clipping)
  {
    using color_t = typename image_format_traits<format>::color_t;

    solid_color_brush<color_t> brush(c);
    overwrite_blender<color_t, color_t> blender;

    paint_shape<format, smp>(buffer, width, height, line_step, shape, brush, blender, clipping);
  }
}

// Paint multiple shapes.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <ggo::image_format format, sampling smp, typename paint_shapes_range_t>
  void paint_shapes(void * buffer, int width, int height, int line_step, const paint_shapes_range_t & shapes)
  {
    paint_shapes<format, smp>(buffer, width, height, line_step, shapes, ggo::rect_int::from_width_height(width, height));
  }

  /////////////////////////////////////////////////////////////////////
  template <ggo::image_format format, sampling smp, typename paint_shapes_range_t>
  void paint_shapes(void * buffer, int width, int height, int line_step, const paint_shapes_range_t & paint_shapes, const ggo::rect_int & clipping)
  {
    const int scale_factor = 8;
    const int first_scale = 2;

    using color_t = typename image_format_traits<format>::color_t;

    auto read_pixel_func = [&](int x, int y)
    {
      return ggo::read_pixel<format>(buffer, x, y, height, line_step);
    };

    auto write_pixel_func = [&](int x, int y, const color_t & c)
    {
      ggo::write_pixel<format>(buffer, x, y, height, line_step, c);
    };

    paint_multi_scale<smp>(width, height, paint_shapes, scale_factor, first_scale, read_pixel_func, write_pixel_func, clipping);
  }

  /////////////////////////////////////////////////////////////////////
  template <ggo::image_format format, sampling smp, typename paint_shape_t>
  void paint_shapes(void * buffer, int width, int height, int line_step, const std::vector<paint_shape_t> & shapes)
  {
    paint_shapes<format, smp>(buffer, width, height, line_step, shapes, ggo::rect_int::from_width_height(width, height));
  }

  /////////////////////////////////////////////////////////////////////
  template <ggo::image_format format, sampling smp, typename paint_shape_t>
  void paint_shapes(void * buffer, int width, int height, int line_step, const std::vector<paint_shape_t> & shapes, const ggo::rect_int & clipping)
  {
    if constexpr(std::is_pointer<paint_shape_t>::value == true)
    {
      // I could not use an adaptator here, but by doing so, I can call the generic version of the paint_shapes function.
      auto adaptor = ggo::make_adaptor(shapes, [](const auto * paint_shape) { return paint_shape; });

      paint_shapes<format, smp>(buffer, width, height, line_step, adaptor, clipping);
    }
    else
    {
      auto adaptor = ggo::make_adaptor(shapes, [](const auto & paint_shape) { return &paint_shape; });

      paint_shapes<format, smp>(buffer, width, height, line_step, adaptor, clipping);
    }
  }
}

#endif
