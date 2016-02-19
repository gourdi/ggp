namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename T>
  bool plane3d<T>::intersect_ray(const ggo::ray3d<T> & ray, float & dist, ggo::ray3d<T> & normal) const
  {
    T den = ggo::dot(_normal, ray.dir());
    if (den >= T(0))
    {
      return false;
    }
    
    T num = _dist_to_origin - ggo::dot(_normal, ray.pos());
    dist = num / den;
    if (dist <= T(0))
    {
      return false;
    }

    normal.pos() = ray.pos() + dist * ray.dir();
    normal.set_normalized_dir(_normal);
    
    return true;
  }
  
  //////////////////////////////////////////////////////////////
  template <typename T>
  std::string plane3d<T>::desc() const 
  {
    std::ostringstream oss;
    oss << "plane3d (" << _normal << ", " << _dist_to_origin << ")";
    return oss.str();
  }
}

