namespace ggo
{
  //////////////////////////////////////////////////////////////////
  template <typename data_t>
  bool half_plane<data_t>::is_point_inside(const ggo::pos2<data_t> & p) const
  {
    return ggo::dot(_normal, p) <= _dist_to_origin;
  }

  //////////////////////////////////////////////////////////////////
  template <typename data_t>
  data_t half_plane<data_t>::dist_to_point(const ggo::pos2<data_t> & p) const
  {
    return std::max(data_t(0), ggo::dot(_normal, p) - _dist_to_origin);
  }
}