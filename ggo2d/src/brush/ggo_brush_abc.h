#ifndef __GGO_BRUSH_ABC__
#define __GGO_BRUSH_ABC__

#include <ggo_shapes2d.h>
#include <ggo_color.h>

namespace ggo
{
  template <typename color_t, typename real_t>
  struct brush_abc
  {
    static_assert(std::is_floating_point<real_t>::value, "expecting floating point type");

    virtual color_t get(real_t x, real_t y, const ggo::paintable_shape2d_abc<float> & shape, int width, int height) const = 0;
  };
}

namespace ggo
{
  template <typename color_t, typename real_t>
  struct solid_brush : public brush_abc<color_t, real_t>
  {
          solid_brush(const color_t & value) : _value(value) {}
  
   real_t get(real_t x, real_t y, const ggo::paintable_shape2d_abc<real_t> & shape, int width, int height) const override { return _value; }
      
   color_t _value;
  };
}

#endif
