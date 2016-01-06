#ifndef __GGO_DISTANCE3D__
#define __GGO_DISTANCE3D__

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename T>
  T hypot(const ggo::set3<T> & pos1, const ggo::set3<T> & pos2)
  {
    T dx = pos1.x() - pos2.x();
    T dy = pos1.y() - pos2.y();
    T dz = pos1.z() - pos2.z();

    return dx * dx + dy * dy + dz * dz;
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  T distance(const ggo::set3<T> & pos1, const ggo::set3<T> & pos2)
  {
    return std::sqrt(ggo::hypot(pos1, pos2));
  }
}

#endif