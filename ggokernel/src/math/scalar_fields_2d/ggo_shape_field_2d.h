#ifndef __GGO_SHAPE_FIELD_2D__
#define __GGO_SHAPE_FIELD_2D__

#include <ggo_scalar_field_2d_abc.h>

namespace ggo
{
  template <template <typename> class ggo_shape2d_type, typename data_t = float>
  class shape_field2d : public scalar_field_2d_abc<data_t>
  {
  public:

    data_t sample(data_t x, data_t y) const override
    {
      return _shape.is_point_inside({ x, y }) ? _height : 0;
    }

    data_t                   _height = 1;
    ggo_shape2d_type<data_t> _shape;
  };
}

#endif
