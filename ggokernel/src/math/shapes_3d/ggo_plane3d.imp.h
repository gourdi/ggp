namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename T>
  bool plane3d<T>::intersect_ray(const ggo::ray3d<T> & ray, float & dist, ggo::ray3d<T> & normal) const
  {
    ggo::set3<T> plane_normal(_a, _b, _c);

    T den = ggo::dot(plane_normal, ray.dir());
    if (std::abs(den) < T(0.000001))
    {
      return false;
    }
    
    T num = ggo::dot(plane_normal, ray.pos()) + _d;
    dist = -num / den;
    if (dist <= T(0))
    {
      return false;
    }
    
    normal.pos() = ray.pos() + dist * ray.dir();
    
    if (den > T(0))
    {
      plane_normal *= T(-1);
    }
    normal.set_dir(plane_normal);
    
    return true;
  }
  
  //////////////////////////////////////////////////////////////
  template <typename T>
  std::string plane3d<T>::desc() const 
  {
    std::ostringstream oss;
    oss << "plane3d (" << _a << ", " << _b << ", " << _c << ", " << _d << ")";
    return oss.str();
  }
}

