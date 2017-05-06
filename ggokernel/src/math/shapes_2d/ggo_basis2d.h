#ifndef __GGO_BASIS2D__
#define __GGO_BASIS2D__

#include <ggo_vec.h>

namespace ggo
{
  template <typename data_t>
  class basis2d
  {
  public:

    basis2d();
    basis2d(const ggo::pos2<data_t> & pos, const ggo::pos2<data_t> & x, const ggo::pos2<data_t> & y);

    void move(const vec2<data_t> & d);

    void rotate(data_t angle);
    void rotate(const vec2<data_t> & center, data_t angle);

    const pos2<data_t> &  pos() const { return _pos; }
    const vec2<data_t> &  x() const { return _x; }
    const vec2<data_t> &  y() const { return _y; }

  private:

    ggo::pos2<data_t> _pos;
    ggo::pos2<data_t> _x;
    ggo::pos2<data_t> _y;
  };
}

#endif
