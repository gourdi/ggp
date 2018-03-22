#ifndef __GGO_MULTI_SCALAR_FIELD_2D__
#define __GGO_MULTI_SCALAR_FIELD_2D__

#include <ggo_scalar_field_2d_abc.h>

namespace ggo
{
  template <template <typename> class field2d_type,
            typename data_t = float>
  class multi_scalar_field2d
  {
  public:
    
    data_t sample(data_t x, data_t y) const
    {
      data_t v(0);
      for (const auto & field : _fields)
      {
        v += field.sample(x, y);
      }
      return v;
    }
    
    std::vector<field2d_type<data_t>> _fields;
  };
}

#endif

