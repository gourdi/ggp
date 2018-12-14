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
  bool plane3d<data_t>::intersect_segment(const ggo::pos3<data_t> & pos, const ggo::vec3<data_t> & dir, data_t length) const
  {
    data_t den = ggo::dot(_normal, dir);
    if (den == 0)
    {
      return false; // Here the segment could totally be included in the plane, but we don't need to handle this case for now.
    }

    data_t num = _dist_to_origin - ggo::dot(_normal, pos);
    data_t dist = num / den;
    
    return (dist >= 0 && dist <= length);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  std::ostream & operator<<(std::ostream & os, const plane3d<data_t> & plane)
  {
    os << "(normal=" << plane.normal() << ", dist_to_origin=" << plane.dist_to_origin() << ")";
    return os;
  }
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t, int normal_x, int normal_y, int normal_z, int dist_to_origin>
  bool const_plane3d<data_t, normal_x, normal_y, normal_z, dist_to_origin>::intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const
  {
    data_t den = normal_x * ray.dir().x() + normal_y * ray.dir().y() + normal_z * ray.dir().z();
    if (den >= 0)
    {
      return false;
    }

    data_t num = dist_to_origin - (normal_x * ray.pos().x() + normal_y * ray.pos().y() + normal_z * ray.pos().z());
    dist = num / den;
    if (dist <= 0)
    {
      return false;
    }

    normal.pos() = ray.pos() + dist * ray.dir();
    normal.set_normalized_dir({ normal_x, normal_y, normal_z });

    return true;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, int normal_x, int normal_y, int normal_z, int dist_to_origin>
  std::ostream & operator<<(std::ostream & os, const const_plane3d<data_t, normal_x, normal_y, normal_z, dist_to_origin> & plane)
  {
    os << "(normal=(" << normal_x << ", " << normal_y << ", " << normal_z << "), dist_to_torigin=" << dist_to_origin << ")";
    return os;
  }
}

