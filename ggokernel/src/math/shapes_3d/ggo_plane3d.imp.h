namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  bool plane3d<data_t>::intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const
  {
    data_t den = ggo::dot(_normal, ray.dir());
    if (den >= 0)
    {
      return false;
    }
    
    data_t num = _dist_to_origin - ggo::dot(_normal, ray.pos());
    dist = num / den;
    if (dist <= 0)
    {
      return false;
    }

    normal.pos() = ray.pos() + dist * ray.dir();
    normal.set_normalized_dir(_normal);
    
    return true;
  }
  
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  std::ostream & plane3d<data_t>::operator<<(std::ostream & os) const
  {
    os << "plane3d (" << _normal << ", " << _dist_to_origin << ")";
    return os;
  }
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t, int normal_x, int normal_y, int normal_z, int dist_to_origin>
  bool const_plane3d<data_t, normal_x, normal_y, normal_z, dist_to_origin>::intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const
  {
    data_t den = normal_x * ray.dir().template get<0>() + normal_y * ray.dir().template get<1>() + normal_z * ray.dir().template get<2>();
    if (den >= 0)
    {
      return false;
    }

    data_t num = dist_to_origin - (normal_x * ray.pos().template get<0>() + normal_y * ray.pos().template get<1>() + normal_z * ray.pos().template get<2>());
    dist = num / den;
    if (dist <= 0)
    {
      return false;
    }

    normal.pos() = ray.pos() + dist * ray.dir();
    normal.set_normalized_dir(normal_x, normal_y, normal_z);

    return true;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, int normal_x, int normal_y, int normal_z, int dist_to_origin>
  std::ostream & const_plane3d<data_t, normal_x, normal_y, normal_z, dist_to_origin>::operator<<(std::ostream & os) const
  {
    os << "const_plane3d (" << normal_x << ", " << normal_y << ", " << normal_z << ", " << dist_to_origin << ")";
    return os;
  }
}

