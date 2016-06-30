namespace ggo
{
  //////////////////////////////////////////////////////////////////
  template <typename T>
  bool half_plane<T>::is_point_inside(T x, T y) const
  {
    return ggo::dot(_normal, ggo::pos2<T>(x, y)) <= _dist_to_origin;
  }

  //////////////////////////////////////////////////////////////////
  template <typename T>
  T half_plane<T>::dist_to_point(T x, T y) const
  {
    return std::max(T(0), ggo::dot(_normal, ggo::pos2<T>(x, y)) - _dist_to_origin);
  }
}