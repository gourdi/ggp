#ifndef __GGO_MULTI_SCALAR_FIELD_2D__
#define __GGO_MULTI_SCALAR_FIELD_2D__

#include <ggo_scalar_field_2d_abc.h>

namespace ggo
{
  template <template <typename> class field2d_type,
            typename T = float,          
            template <typename, typename...> class container = std::vector>
  class multi_scalar_field2d
  {
  public:
    
    T evaluate(T x, T y) const
    {
      T v(0);
      for (const auto & field : _fields)
      {
        v += field.evaluate(x, y);
      }
      return v;
    }
    
    container<field2d_type<T>> _fields;
  };
}

#endif

