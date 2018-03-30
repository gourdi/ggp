#ifndef __GGO_SHAPE_FIELD_2D__
#define __GGO_SHAPE_FIELD_2D__

#include <ggo_scalar_field_2d_abc.h>

namespace ggo
{
  template <typename shape_t>
  class shape_field2d : public scalar_field_2d_abc<typename shape_t::data_t>
  {
  public:

    using data_t = typename shape_t::data_t;

    data_t sample(data_t x, data_t y) const override
    {
      return _shape.is_point_inside({ x, y }) ? _height : 0;
    }

    data_t  _height = 1;
    shape_t _shape;
  };
}

#endif
