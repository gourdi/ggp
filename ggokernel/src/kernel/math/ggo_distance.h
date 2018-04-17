#ifndef __GGO_DISTANCE__
#define __GGO_DISTANCE__

#include <kernel/ggo_vec.h>
#include <array>

//////////////////////////////////////////////////////////////
// Generic
namespace ggo
{
  template <typename data_t, int n_dim, vec_type vtype>
  data_t hypot(const ggo::vec<data_t, n_dim, vtype> & pos1, const ggo::vec<data_t, n_dim, vtype> & pos2)
  {
    ggo::vec<data_t, n_dim, vtype> diff(pos2 - pos1);

    return ggo::dot(diff, diff);
  }

  template <typename data_t, int n_dim, vec_type vtype>
  data_t distance(const ggo::vec<data_t, n_dim, vtype> & pos1, const ggo::vec<data_t, n_dim, vtype> & pos2)
  {
    return std::sqrt(ggo::hypot(pos1, pos2));
  }
}

//////////////////////////////////////////////////////////////
// 2D
namespace ggo
{
  template <typename data_t>
  data_t hypot(data_t x, data_t y)
  {
    return x * x + y * y;
  }

  template <typename data_t>
  data_t distance(data_t x, data_t y)
  {
    return std::sqrt(ggo::hypot(x, y));
  }

  template <typename data_t>
  data_t hypot(data_t x1, data_t y1, data_t x2, data_t y2)
  {
    data_t dx = x1 - x2;
    data_t dy = y1 - y2;

    return dx * dx + dy * dy;
  }

  template <typename data_t>
  data_t distance(data_t x1, data_t y1, data_t x2, data_t y2)
  {
    return std::sqrt(hypot(x1, y1, x2, y2));
  }

  template <typename data_t>
  data_t hypot(const std::array<data_t, 2> & pos1, const std::array<data_t, 2> & pos2)
  {
    return hypot(pos1[0], pos1[1], pos2[0], pos2[1]);
  }

  template <typename data_t>
  data_t distance(const std::array<data_t, 2> & pos1, const std::array<data_t, 2> & pos2)
  {
    return distance(pos1[0], pos1[1], pos2[0], pos2[1]);
  }

  template <typename data_t>
  data_t hypot(const ggo::pos2<data_t> & pos1, const ggo::pos2<data_t> & pos2)
  {
    return hypot(pos1.x(), pos1.y(), pos2.x(), pos2.y());
  }

  template <typename data_t>
  data_t distance(const ggo::pos2<data_t> & pos1, const ggo::pos2<data_t> & pos2)
  {
    return std::sqrt(hypot(pos1, pos2));
  }
}

//////////////////////////////////////////////////////////////
// 3D
namespace ggo
{
  template <typename data_t>
  data_t hypot(const std::array<data_t, 3> & pos1, const std::array<data_t, 3> & pos2)
  {
    data_t dx = pos1[0] - pos2[0];
    data_t dy = pos1[1] - pos2[1];
    data_t dz = pos1[2] - pos2[2];

    return dx * dx + dy * dy + dz * dz;
  }

  template <typename data_t>
  data_t distance(const std::array<data_t, 3> & pos1, const std::array<data_t, 3> & pos2)
  {
    return std::sqrt(ggo::hypot(pos1, pos2));
  }
}


#endif
