namespace ggo
{
  /////////////////////////////////////////////////////////////////////  
  template <typename data_t>
  influence_plane3d<data_t>::influence_plane3d(const ggo::vec3<data_t> & normal, data_t dist_to_origin, data_t width)
  :
  _normal(normalize(normal)),
  _dist_to_origin(dist_to_origin),
  _width(width)
  {
  }

  /////////////////////////////////////////////////////////////////////  
  template <typename data_t>
  data_t influence_plane3d<data_t>::hypot_to_center(const ggo::pos3<data_t> & p) const
  {
    return ggo::square(ggo::dot(p, _normal) - _dist_to_origin);
  }

  /////////////////////////////////////////////////////////////////////  
  template <typename data_t>
  std::vector<data_t> influence_plane3d<data_t>::intersect_ray(const ggo::ray3d<data_t> & ray) const
  {
    data_t den = ggo::dot(_normal, ray.dir());
    if (std::abs(den) < data_t(0.01))
    {
      return std::vector<data_t>();
    }

    data_t dot_normal_ray = ggo::dot(_normal, ray.pos());
    data_t dist1 = (_dist_to_origin - _width - dot_normal_ray) / den;
    data_t dist2 = (_dist_to_origin + _width - dot_normal_ray) / den;

    if (dist1 <= 0 && dist2 <= 0)
    {
      return std::vector<data_t>();
    }

    if (dist1 < dist2)
    {
      return std::vector<data_t>{dist1, dist2};
    }
    else
    {
      return std::vector<data_t>{dist2, dist1};
    }
  }
}
