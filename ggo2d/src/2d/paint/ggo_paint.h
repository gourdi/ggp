#ifndef __GGO_PAINT__
#define __GGO_PAINT__

#include <vector>
#include <kernel/ggo_kernel.h>
#include <kernel/ggo_rect_int.h>
#include <kernel/math/shapes_2d/ggo_shapes2d.h>
#include <2d/ggo_image.h>
#include <2d/paint/ggo_pixel_sampling.h>
#include <2d/paint/ggo_brush.h>
#include <2d/paint/ggo_blend.h>
#include <2d/paint/ggo_paint_shape_abc.h>
#include <2d/paint/ggo_multi_scale_paint.h>

// Paint single shape.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <pixel_sampling sampling, typename image_t, typename shape_t, typename brush_t, typename blend_t>
  void paint(image_t & image, const shape_t & shape, brush_t brush, blend_t blend,
    const ggo::rect_int & clipping)
  {
    paint_multi_scale<sampling>(image, shape, 8, 2, brush, blend, clipping);
  }

  /////////////////////////////////////////////////////////////////////
  template <pixel_sampling sampling, typename image_t, typename shape_t, typename brush_t, typename blend_t>
  void paint(image_t & image, const shape_t & shape, brush_t brush, blend_t blend)
  {
    paint<sampling>(image, shape, brush, blend, ggo::rect_int::from_size(image.size()));
  }

  /////////////////////////////////////////////////////////////////////
  template <pixel_sampling sampling, typename image_t, typename shape_t>
  void paint(image_t & image, const shape_t & shape, const typename image_t::color_t & c, float opacity,
    const ggo::rect_int & clipping)
  {
    using color_t = typename image_t::color_t;

    solid_color_brush<color_t> brush(c);
    alpha_blender<color_t> blender(opacity);

    paint<sampling, image_t, shape_t, solid_color_brush<color_t>, alpha_blender<color_t>>(image, shape, brush, blender, clipping);
  }

  /////////////////////////////////////////////////////////////////////
  template <pixel_sampling sampling, typename image_t, typename shape_t>
  void paint(image_t & image, const shape_t & shape, const typename image_t::color_t & c, float opacity)
  {
    paint<sampling>(image, shape, c, opacity, ggo::rect_int::from_size(image.size()));
  }

  /////////////////////////////////////////////////////////////////////
  template <pixel_sampling sampling, typename image_t, typename shape_t>
  void paint(image_t & image, const shape_t & shape, const typename image_t::color_t & c,
    const ggo::rect_int & clipping)
  {
    using color_t = typename image_t::color_t;

    solid_color_brush<color_t> brush(c);
    overwrite_blender<color_t> blender;

    paint<sampling, image_t, shape_t, solid_color_brush<color_t>, overwrite_blender<color_t>>(image, shape, brush, blender, clipping);
  }

  /////////////////////////////////////////////////////////////////////
  template <pixel_sampling sampling, typename image_t, typename shape_t>
  void paint(image_t & image, const shape_t & shape, const typename image_t::color_t & c)
  {
    paint<sampling>(image, shape, c, ggo::rect_int::from_size(image.size()));
  }
}

// Paint multiple shapes.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <pixel_sampling sampling, typename image_t, typename color_t, typename data_t>
  void paint(image_t & image, const scene2d<color_t, data_t> & scene, const ggo::rect_int & clipping)
  {
    paint<sampling>(image, scene, 8, 2, clipping);
  }

  /////////////////////////////////////////////////////////////////////
  template <pixel_sampling sampling, typename image_t, typename color_t, typename data_t>
  void paint(image_t & image, const scene2d<color_t, data_t> & scene)
  {
    paint_multi_scale<sampling>(image, scene, 8, 2, ggo::rect_int::from_size(image.size()));
  }
}

#endif
