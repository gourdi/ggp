#ifndef __GGO_SHAPE_FIELD_2D__
#define __GGO_SHAPE_FIELD_2D__

#include <ggo_scalar_field_2d_abc.h>

namespace ggo
{
  template <template <typename> class ggo_shape2d_type, typename T = float>
  class shape_field2d : public scalar_field_2d_abc<T>
  {
  public:

    T evaluate(T x, T y) const override
    {
      return _shape.is_point_inside({ x, y }) ? _height : 0;
    }

    T                   _height = 1;
    ggo_shape2d_type<T> _shape;
  };
}

#endif
