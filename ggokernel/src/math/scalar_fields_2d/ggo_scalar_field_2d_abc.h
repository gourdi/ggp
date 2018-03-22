#ifndef __GGO_SCALAR_FIELD_2D__
#define __GGO_SCALAR_FIELD_2D__

namespace ggo
{
  template <typename data_t>
  class scalar_field_2d_abc
  {
  public:

    virtual  ~scalar_field_2d_abc() {}

    virtual data_t sample(data_t x, data_t y) const = 0;
  };
}

#endif
