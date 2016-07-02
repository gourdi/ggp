namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  bool disc3d<data_t>::intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const
  {
    // The ray is not looking in the right direction.
    data_t den = ggo::dot(ray.dir(), _ray.dir());
    if (den >= data_t(-0.001))
    {
      return false;
    }
    
    // The ray's origin is behind the disc.
    ggo::vec3<data_t> diff(_ray.pos() - ray.pos());
    data_t num = ggo::dot(diff, _ray.dir());
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
  template <typename data_t>
  std::ostream & disc3d<data_t>::operator<<(std::ostream & os) const
  {
    os << "disc3d " << _ray;
    return os;
  }
}