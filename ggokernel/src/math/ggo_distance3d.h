#ifndef __GGO_DISTANCE3D__
#define __GGO_DISTANCE3D__

#include <ggo_vec.h>
#include <array>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  data_t hypot(const ggo::vec3<data_t> & pos1, const ggo::vec3<data_t> & pos2)
  {
    data_t dx = pos1.template get<0>() - pos2.template get<0>();
    data_t dy = pos1.template get<1>() - pos2.template get<1>();
    data_t dz = pos1.template get<2>() - pos2.template get<2>();

    return dx * dx + dy * dy + dz * dz;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  data_t distance(const ggo::vec3<data_t> & pos1, const ggo::vec3<data_t> & pos2)
  {
    return std::sqrt(ggo::hypot(pos1, pos2));
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  data_t hypot(const std::array<data_t, 3> & pos1, const std::array<data_t, 3> & pos2)
  {
    data_t dx = pos1[0] - pos2[0];
    data_t dy = pos1[1] - pos2[1];
    data_t dz = pos1[2] - pos2[2];

    return dx * dx + dy * dy + dz * dz;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  data_t distance(const std::array<data_t, 3> & pos1, const std::array<data_t, 3> & pos2)
  {
    return std::sqrt(ggo::hypot(pos1, pos2));
  }
}

#endif