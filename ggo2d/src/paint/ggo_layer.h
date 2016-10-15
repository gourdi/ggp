#ifndef __GGO_LAYER__
#define __GGO_LAYER__

#include <ggo_shapes2d.h>
#include <ggo_brush_abc.h>
#include <ggo_blender_abc.h>

namespace ggo
{
  template <typename color_t, typename intensity_t, typename data_t>
  struct layer_item
  {
    static_assert(std::is_arithmetic<intensity_t>::value, "unexpected intensity type");

    layer_item(std::shared_ptr<const ggo::paintable_shape2d_abc<data_t>> shape,
      const color_t & color,
      data_t intensity = 1,
      std::shared_ptr<const ggo::blender_abc<color_t>> blender = std::make_shared<ggo::alpha_blender<color_t, data_t>>())
      :
      _shape(shape),
      _color_brush(std::make_shared<ggo::solid_brush<color_t, data_t>>(color)),
      _intensity_brush(std::make_shared<ggo::solid_brush<color_t, data_t>>(intensity)),
      _blender(blender)
    {}

    layer_item(std::shared_ptr<const ggo::paintable_shape2d_abc<data_t>> shape,
      std::shared_ptr<const ggo::brush_abc<color_t>> color_brush,
      std::shared_ptr<const ggo::brush_abc<intensity_t>> intensity_brush,
      std::shared_ptr<const ggo::blender_abc<color_t>> blender = std::make_shared<ggo::alpha_blender<color_t>>())
      :
      _shape(shape),
      _color_brush(color_brush),
      _intensity_brush(intensity_brush),
      _blender(blender)
    {}

    std::shared_ptr<const ggo::paintable_shape2d_abc<data_t>>   _shape;
    std::shared_ptr<const ggo::brush_abc<color_t, data_t>>      _color_brush;
    std::shared_ptr<const ggo::brush_abc<intensity_t, data_t>>  _intensity_brush;
    std::shared_ptr<const ggo::blender_abc<color_t, data_t>>    _blender;

    // For this struct to be used with paint code, it must implement some methods.
    rect_data<data_t> get_bounding_rect() const { return _shape->get_bounding_rect(); }
    rect_intersection	get_rect_intersection(const rect_data<data_t> & rect_data) const { return _shape->get_bounding_rect(rect_data); }
  };
}

#endif
