#ifndef __GGO_SCALAR_FIELD_2D__
#define __GGO_SCALAR_FIELD_2D__

namespace ggo
{
  template <typename T>
  class scalar_field_2d_abc
  {
  public:

    virtual  ~scalar_field_2d_abc() {}

    virtual T evaluate(T x, T y) const = 0;
  };
}

#endif
