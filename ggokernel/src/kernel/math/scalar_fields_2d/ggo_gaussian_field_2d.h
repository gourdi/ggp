#ifndef __GGO_GAUSSIAN_FIELD_2D__
#define __GGO_GAUSSIAN_FIELD_2D__

#include <kernel/math/scalar_fields_2d/ggo_scalar_field_2d_abc.h>

namespace ggo
{
  template <typename data_t = float>
  class gaussian_field2d : public scalar_field_2d_abc<data_t>
  {
  public:

    data_t sample(data_t x, data_t y) const override
    {
      return _amp * std::exp(-(ggo::square(x - _x) + ggo::square(y - _y)) / _var);
    }

    data_t _x = 0;
    data_t _y = 0;
    data_t _var = 1; // = stddev^2
    data_t _amp = 1;
  };
}

#endif

