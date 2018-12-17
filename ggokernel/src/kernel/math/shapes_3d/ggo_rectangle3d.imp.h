//////////////////////////////////////////////////////////////
template <int normal_x, int normal_y, int normal_z, typename data_t>
bool ggo::rectangle3d<normal_x, normal_y, normal_z, data_t>::intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const
{
  if constexpr(normal_x == 1 || normal_x == -1)
  {
    return intersect_ray_x(ray, dist, normal);
  }
  if constexpr(normal_y == 1 || normal_y == -1)
  {
    return intersect_ray_y(ray, dist, normal);
  }
  if constexpr(normal_z == 1 || normal_z == -1)
  {
    return intersect_ray_z(ray, dist, normal);
  }

  return false;
}

//////////////////////////////////////////////////////////////
template <int normal_x, int normal_y, int normal_z, typename data_t>
bool ggo::rectangle3d<normal_x, normal_y, normal_z, data_t>::intersect_ray_x(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const
{
  if (normal_x * ray.dir().x() >= 0)
  {
    return false;
  }

  dist = (_center.x() - ray.pos().x()) / ray.dir().x();
  if (dist <= 0.f)
  {
    return false;
  }

  normal.pos() = ray.pos() + dist * ray.dir();

  if (normal.pos().y() > _center.y() + _size1 || normal.pos().y() < _center.y() - _size1)
  {
    return false;
  }
  if (normal.pos().z() > _center.z() + _size2 || normal.pos().z() < _center.z() - _size2)
  {
    return false;
  }

  normal.set_normalized_dir({ normal_x, normal_y, normal_z });

  return true;
}

//////////////////////////////////////////////////////////////
template <int normal_x, int normal_y, int normal_z, typename data_t>
bool ggo::rectangle3d<normal_x, normal_y, normal_z, data_t>::intersect_ray_y(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const
{
  if (normal_y * ray.dir().y() >= 0)
  {
    return false;
  }

  dist = (_center.y() - ray.pos().y()) / ray.dir().y();
  if (dist <= 0.f)
  {
    return false;
  }

  normal.pos() = ray.pos() + dist * ray.dir();

  if (normal.pos().x() > _center.x() + _size1 || normal.pos().x() < _center.x() - _size1)
  {
    return false;
  }
  if (normal.pos().z() > _center.z() + _size2 || normal.pos().z() < _center.z() - _size2)
  {
    return false;
  }

  normal.set_normalized_dir({ normal_x, normal_y, normal_z });

  return true;
}

//////////////////////////////////////////////////////////////
template <int normal_x, int normal_y, int normal_z, typename data_t>
bool ggo::rectangle3d<normal_x, normal_y, normal_z, data_t>::intersect_ray_z(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const
{
  if (normal_z * ray.dir().z() >= 0)
  {
    return false;
  }

  dist = (_center.z() - ray.pos().z()) / ray.dir().z();
  if (dist <= 0.f)
  {
    return false;
  }

  normal.pos() = ray.pos() + dist * ray.dir();

  if (normal.pos().x() > _center.x() + _size1 || normal.pos().x() < _center.x() - _size1)
  {
    return false;
  }
  if (normal.pos().y() > _center.y() + _size2 || normal.pos().y() < _center.y() - _size2)
  {
    return false;
  }

  normal.set_normalized_dir({ normal_x, normal_y, normal_z });

  return true;
}

//////////////////////////////////////////////////////////////
template <int normal_x, int normal_y, int normal_z, typename data_t>
std::optional<ggo::box3d_data<data_t>> ggo::rectangle3d<normal_x, normal_y, normal_z, data_t>::get_bounding_box(const ggo::basis3d<data_t> & basis) const
{
  if constexpr(normal_x == 1 || normal_x == -1)
  {
    const vec3<data_t> normal1{ data_t(0), data_t(1), data_t(0) };
    const vec3<data_t> normal2{ data_t(0), data_t(0), data_t(1) };

    return box3d_data<data_t>::from({
      basis.point_from_local_to_world(_center - _size1 * normal1),
      basis.point_from_local_to_world(_center + _size1 * normal1),
      basis.point_from_local_to_world(_center - _size2 * normal2),
      basis.point_from_local_to_world(_center + _size2 * normal2),
    });
  }
  else if constexpr(normal_y == 1 || normal_y == -1)
  {
    const vec3<data_t> normal1{ data_t(1), data_t(0), data_t(0) };
    const vec3<data_t> normal2{ data_t(0), data_t(0), data_t(1) };

    return box3d_data<data_t>::from({
      basis.point_from_local_to_world(_center - _size1 * normal1),
      basis.point_from_local_to_world(_center + _size1 * normal1),
      basis.point_from_local_to_world(_center - _size2 * normal2),
      basis.point_from_local_to_world(_center + _size2 * normal2),
    });
  }
  else
  {
    static_assert(normal_z == 1 || normal_z == -1);

    const vec3<data_t> normal1{ data_t(1), data_t(0), data_t(0) };
    const vec3<data_t> normal2{ data_t(0), data_t(1), data_t(0) };

    return box3d_data<data_t>::from({
      basis.point_from_local_to_world(_center - _size1 * normal1),
      basis.point_from_local_to_world(_center + _size1 * normal1),
      basis.point_from_local_to_world(_center - _size2 * normal2),
      basis.point_from_local_to_world(_center + _size2 * normal2),
    });
  }
}
