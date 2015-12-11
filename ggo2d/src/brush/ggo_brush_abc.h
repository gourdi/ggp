#ifndef __GGO_BRUSH_ABC__
#define __GGO_BRUSH_ABC__

#include <ggo_shapes2d.h>
#include <ggo_color.h>

namespace ggo
{
  template <typename T>
  struct brush_abc
  {
    virtual T get(float x, float y, const ggo::paintable_shape2d_abc<float> & shape, int width, int height) const = 0;
  };

  using rgb_brush_abc = brush_abc<ggo::color>;
  using gray_brush_abc = brush_abc<float>;
  using opacity_brush_abc = brush_abc<float>;
}

namespace ggo
{
  template <typename T>
  struct solid_brush : public brush_abc<T>
  {
      solid_brush(const T & value) : _value(value) {}
  
    T get(float x, float y, const ggo::paintable_shape2d_abc<float> & shape, int width, int height) const override { return _value; }
      
    T _value;
  };
  
  using rgb_solid_brush = solid_brush<ggo::color>;
  using gray_solid_brush = solid_brush<float>;
  using opacity_solid_brush = solid_brush<float>;
}

#endif
