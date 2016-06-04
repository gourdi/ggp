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
  std::string plane3d<data_t>::desc() const 
  {
    std::ostringstream oss;
    oss << "plane3d (" << _normal << ", " << _dist_to_origin << ")";
    return oss.str();
  }
}

