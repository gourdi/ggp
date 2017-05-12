#ifndef __GGO_BASIS2D__
#define __GGO_BASIS2D__

#include <ggo_vec.h>

// Note that unlike basis3D class, thes basis2d class properly handle
// basis that are not orthogonal nor normalized.

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

    ggo::vec2<data_t> vector_from_local_to_world(const ggo::vec2<data_t> & v) const;
    ggo::vec2<data_t> vector_from_world_to_local(const ggo::vec2<data_t> & v) const;

    ggo::pos2<data_t> point_from_local_to_world(const ggo::pos2<data_t> & p) const;
    ggo::pos2<data_t> point_from_world_to_local(const ggo::pos2<data_t> & p) const;

    const pos2<data_t> &  pos() const { return _pos; }
    const vec2<data_t> &  x() const { return _x; }
    const vec2<data_t> &  y() const { return _y; }

    pos2<data_t> &  pos() { return _pos; }
    vec2<data_t> &  x() { return _x; }
    vec2<data_t> &  y() { return _y; }

  private:

    ggo::pos2<data_t> _pos;
    ggo::pos2<data_t> _x;
    ggo::pos2<data_t> _y;
  };
}

#endif
