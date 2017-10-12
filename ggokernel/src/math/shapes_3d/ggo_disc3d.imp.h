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
  std::optional<box3d_data<data_t>> disc3d<data_t>::get_bounding_box(const ggo::basis3d<data_t> & basis) const
  {
    ggo::pos3<data_t> world_center = basis.point_from_local_to_world(_ray.pos());

    const ggo::vec3<data_t> x(data_t(1), data_t(0), data_t(0));
    const ggo::vec3<data_t> y(data_t(0), data_t(1), data_t(0));
    const ggo::vec3<data_t> z(data_t(0), data_t(0), data_t(1));

    // Suboptimal bounding box: does not match the tighest box, but it is OK anyway.
    return box3d_data<data_t>::from({
      world_center + _radius * x, world_center - _radius * x,
      world_center + _radius * y, world_center - _radius * y,
      world_center + _radius * z, world_center - _radius * z });
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  std::ostream & operator<<(std::ostream & os, const disc3d<data_t> & disc)
  {
    os << "(ray=" << disc.ray() << ", radius=" << disc.radius() << ")";
    return os;
  }
}