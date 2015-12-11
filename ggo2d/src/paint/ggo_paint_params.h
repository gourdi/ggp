#ifndef __GGO_PAINT_PARAMS__
#define __GGO_PAINT_PARAMS__

#include <memory>
#include <ggo_brush_abc.h>
#include <ggo_blender_abc.h>
#include <ggo_color.h>
/*
namespace ggo
{
  template <typename T>
  struct paint_params
  {
    paint_params(std::shared_ptr<const ggo::brush_abc<T>> color_brush, float opacity)
    :
    _color_brush(color_brush),
    _opacity_brush(std::make_shared<ggo::opacity_solid_brush>(opacity))
    {
    }
    
    paint_params(std::shared_ptr<const ggo::brush_abc<T>> color_brush,
                 std::shared_ptr<const ggo::opacity_brush_abc> opacity_brush = std::make_shared<ggo::opacity_solid_brush>(1),
                 std::shared_ptr<const ggo::blender_abc<T>> blender = std::make_shared<ggo::alpha_blender<T>>())
    :
    _color_brush(color_brush),
    _opacity_brush(opacity_brush),
    _blender(blender)
    {
    }
      
    paint_params(const T & color, float opacity = 1)
    :
    _color_brush(std::make_shared<const ggo::solid_brush<T>>(color)),
    _opacity_brush(std::make_shared<ggo::opacity_solid_brush>(opacity))
    {
    }

    std::shared_ptr<const ggo::brush_abc<T>>      _color_brush;
    std::shared_ptr<const ggo::opacity_brush_abc> _opacity_brush = std::make_shared<ggo::opacity_solid_brush>(1);
    std::shared_ptr<const ggo::blender_abc<T>>    _blender = std::make_shared<ggo::alpha_blender<T>>();
  };

  using gray_paint_params = paint_params<float>;
  using rgb_paint_params = paint_params<ggo::color>;
}*/

#endif

