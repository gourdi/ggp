#ifndef __GGO_GAUSSIAN_FIELD_2D__
#define __GGO_GAUSSIAN_FIELD_2D__

#include <ggo_scalar_field_2d_abc.h>

namespace ggo
{
  template <typename T = float>
  class gaussian_field2d : public scalar_field_2d_abc<T>
  {
  public:

    T evaluate(T x, T y) const override
    {
      return _amp * std::exp(-(ggo::square(x - _x) + ggo::square(y - _y)) / _var);
    }

    T _x = 0;
    T _y = 0;
    T _var = 1;
    T _amp = 1;
  };
}

#endif

