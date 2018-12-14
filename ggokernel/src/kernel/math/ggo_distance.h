#ifndef __GGO_DISTANCE__
#define __GGO_DISTANCE__

#include <kernel/ggo_vec.h>
#include <array>

//////////////////////////////////////////////////////////////
// 2D
namespace ggo
{
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
