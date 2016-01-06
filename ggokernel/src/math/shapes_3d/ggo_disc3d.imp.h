namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename T>
  bool disc3d<T>::intersect_ray(const ggo::ray3d<T> & ray, float & dist, ggo::ray3d<T> & normal) const
  {
    // The ray is not looking in the right direction.
    T den = ggo::dot(ray.dir(), _ray.dir());
    if (den >= T(-0.001))
    {
      return false;
    }
    
    // The ray's origin is behind the disc.
    ggo::set3<T> diff(_ray.pos() - ray.pos());
    T num = ggo::dot(diff, _ray.dir());
    if (num >= 0)
    {
      return false;
    }
    
    // Find intersection with the place of the disc.
    dist = num / den;
    GGO_ASSERT_GE(dist, 0);
    
    normal.pos() = ray.pos() + dist * ray.dir();
    
    // The intersection point is too far away from the center of the disc.
    if (ggo::distance(normal.pos(), _ray.pos()) > _radius)
    {
      return false;
    }
    
    // Compute the normal's direction.
    normal.set_normalized_dir(_ray.dir());
    
    return true;
  }
  
  //////////////////////////////////////////////////////////////
  template <typename T>
  std::string disc3d<T>::desc() const
  {
    std::ostringstream oss;
    oss << "disc3d " << _ray;
    return oss.str();
  }
}