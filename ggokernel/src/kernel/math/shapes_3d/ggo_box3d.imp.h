namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  box3d<data_t>::box3d(data_t x_min, data_t x_max, data_t y_min, data_t y_max, data_t z_min, data_t z_max)
  :
  _data(x_min, x_max, y_min, y_max, z_min, z_max)
  {
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  box3d<data_t>::box3d(const box3d_data<data_t> & data) : _data(data)
  {
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  box3d<data_t>::box3d(const ggo::pos3<data_t> & p)
  {
    _x_min = p.x();
    _x_max = p.x();
    _y_min = p.y();
    _y_max = p.y();
    _z_min = p.z();
    _z_max = p.z();
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  bool box3d<data_t>::intersect_segment(const ggo::pos3<data_t> & pos, const ggo::vec3<data_t> & dir, data_t length) const
  {
    GGO_ASSERT(dir.is_normalized() == true);

    // First test to check if one of the 2 vertices are inside the box.
    if (is_point_inside(pos) == true)
    {
      return true;
    }
    if (is_point_inside(pos + length * dir) == true)
    {
      return true;
    }

    // X coordinate tests (ie. yOz faces).
    if (std::abs(dir.x()) > data_t(0.00001))
    {
      data_t dist = (_data._x_min - pos.x()) / dir.x();
      if (dist > 0 && dist < length)
      {
        data_t y = pos.y() + dist * dir.y();
        data_t z = pos.z() + dist * dir.z();

        if (y > _data._y_min && y < _data._y_max &&
            z > _data._z_min && z < _data._z_max)
        {
          return true;
        }
      }

      dist = (_data._x_max - pos.x()) / dir.x();
      if (dist > 0 && dist < length)
      {
        data_t y = pos.y() + dist * dir.y();
        data_t z = pos.z() + dist * dir.z();

        if (y > _data._y_min && y < _data._y_max &&
            z > _data._z_min && z < _data._z_max)
        {
          return true;
        }
      }
    }

    // Y coordinate tests (ie. xOz faces).
    if (std::abs(dir.y()) > data_t(0.00001))
    {
      data_t dist = (_data._y_min - pos.y()) / dir.y();
      if (dist > 0 && dist < length)
      {
        data_t x = pos.x() + dist * dir.x();
        data_t z = pos.z() + dist * dir.z();

        if (x > _data._x_min && x < _data._x_max &&
            z > _data._z_min && z < _data._z_max)
        {
          return true;
        }
      }

      dist = (_data._y_max - pos.y()) / dir.y();
      if (dist > 0 && dist < length)
      {
        data_t x = pos.x() + dist * dir.x();
        data_t z = pos.z() + dist * dir.z();

        if (x > _data._x_min && x < _data._x_max &&
            z > _data._z_min && z < _data._z_max)
        {
          return true;
        }
      }
    }

    // Z coordinate tests (ie. xOy faces).
    if (std::abs(dir.z()) > data_t(0.00001))
    {
      data_t dist = (_data._z_min - pos.z()) / dir.z();
      if (dist > 0 && dist < length)
      {
        data_t x = pos.x() + dist * dir.x();
        data_t y = pos.y() + dist * dir.y();

        if (x > _data._x_min && x < _data._x_max &&
            y > _data._y_min && y < _data._y_max)
        {
          return true;
        }
      }

      dist = (_data._z_max - pos.z()) / dir.z();
      if (dist > 0 && dist < length)
      {
        data_t x = pos.x() + dist * dir.x();
        data_t y = pos.y() + dist * dir.y();

        if (x > _data._x_min && x < _data._x_max &&
            y > _data._y_min && y < _data._y_max)
        {
          return true;
        }
      }
    }

    return false;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  bool box3d<data_t>::intersect_ray(const ggo::ray3d<data_t> & ray) const
  {
    // X coordinate tests (ie. yOz faces).
    if (std::abs(ray.dir().x()) > data_t(0.00001))
    {
      data_t dist = (_data._x_min - ray.pos().x()) / ray.dir().x();
      if (dist > 0)
      {
        data_t y = ray.pos().y() + dist * ray.dir().y();
        data_t z = ray.pos().z() + dist * ray.dir().z();

        if (y > _data._y_min && y < _data._y_max &&
            z > _data._z_min && z < _data._z_max)
        {
          return true;
        }
      }

      dist = (_data._x_max - ray.pos().x()) / ray.dir().x();
      if (dist > 0)
      {
        data_t y = ray.pos().y() + dist * ray.dir().y();
        data_t z = ray.pos().z() + dist * ray.dir().z();

        if (y > _data._y_min && y < _data._y_max &&
            z > _data._z_min && z < _data._z_max)
        {
          return true;
        }
      }
    }

    // Y coordinate tests (ie. xOz faces).
    if (std::abs(ray.dir().y()) > data_t(0.00001))
    {
      data_t dist = (_data._y_min - ray.pos().y()) / ray.dir().y();
      if (dist > 0)
      {
        data_t x = ray.pos().x() + dist * ray.dir().x();
        data_t z = ray.pos().z() + dist * ray.dir().z();

        if (x > _data._x_min && x < _data._x_max &&
            z > _data._z_min && z < _data._z_max)
        {
          return true;
        }
      }

      dist = (_data._y_max - ray.pos().y()) / ray.dir().y();
      if (dist > 0)
      {
        data_t x = ray.pos().x() + dist * ray.dir().x();
        data_t z = ray.pos().z() + dist * ray.dir().z();

        if (x > _data._x_min && x < _data._x_max &&
            z > _data._z_min && z < _data._z_max)
        {
          return true;
        }
      }
    }

    // Z coordinate tests (ie. xOy faces).
    if (std::abs(ray.dir().z()) > data_t(0.00001))
    {
      data_t dist = (_data._z_min - ray.pos().z()) / ray.dir().z();
      if (dist > 0)
      {
        data_t x = ray.pos().x() + dist * ray.dir().x();
        data_t y = ray.pos().y() + dist * ray.dir().y();

        if (x > _data._x_min && x < _data._x_max &&
            y > _data._y_min && y < _data._y_max)
        {
          return true;
        }
      }

      dist = (_data._z_max - ray.pos().z()) / ray.dir().z();
      if (dist > 0)
      {
        data_t x = ray.pos().x() + dist * ray.dir().x();
        data_t y = ray.pos().y() + dist * ray.dir().y();

        if (x > _data._x_min && x < _data._x_max &&
            y > _data._y_min && y < _data._y_max)
        {
          return true;
        }
      }
    }

    return false;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  bool box3d<data_t>::intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const
  {
    bool hit = false;
    dist = std::numeric_limits<data_t>::max();

    // X coordinate tests (ie. yOz faces).
    if (std::abs(ray.dir().x()) > data_t(0.00001))
    {
      data_t dist_cur = (_data._x_min - ray.pos().x()) / ray.dir().x();
      if (dist_cur > 0 && dist_cur < dist)
      {
        data_t y = ray.pos().y() + dist_cur * ray.dir().y();
        data_t z = ray.pos().z() + dist_cur * ray.dir().z();

        if (y > _data._y_min && y < _data._y_max &&
            z > _data._z_min && z < _data._z_max)
        {
          hit = true;
          dist = dist_cur;
          normal.pos().set(_data._x_min, y, z);
          normal.set_normalized_dir(-1, 0, 0);
        }
      }

      dist_cur = (_data._x_max - ray.pos().x()) / ray.dir().x();
      if (dist_cur > 0 && dist_cur < dist)
      {
        data_t y = ray.pos().y() + dist_cur * ray.dir().y();
        data_t z = ray.pos().z() + dist_cur * ray.dir().z();

        if (y > _data._y_min && y < _data._y_max &&
            z > _data._z_min && z < _data._z_max)
        {
          hit = true;
          dist = dist_cur;
          normal.pos().set(_data._x_max, y, z);
          normal.set_normalized_dir(1, 0, 0);
        }
      }
    }

    // Y coordinate tests (ie. xOz faces).
    if (std::abs(ray.dir().y()) > data_t(0.00001))
    {
      data_t dist_cur = (_data._y_min - ray.pos().y()) / ray.dir().y();
      if (dist_cur > 0 && dist_cur < dist)
      {
        data_t x = ray.pos().x() + dist_cur * ray.dir().x();
        data_t z = ray.pos().z() + dist_cur * ray.dir().z();

        if (x > _data._x_min && x < _data._x_max &&
            z > _data._z_min && z < _data._z_max)
        {
          hit = true;
          dist = dist_cur;
          normal.pos().set(x, _data._y_min, z);
          normal.set_normalized_dir(0, -1, 0);
        }
      }

      dist_cur = (_data._y_max - ray.pos().y()) / ray.dir().y();
      if (dist_cur > 0 && dist_cur < dist)
      {
        data_t x = ray.pos().x() + dist_cur * ray.dir().x();
        data_t z = ray.pos().z() + dist_cur * ray.dir().z();

        if (x > _data._x_min && x < _data._x_max &&
            z > _data._z_min && z < _data._z_max)
        {
          hit = true;
          dist = dist_cur;
          normal.pos().set(x, _data._y_max, z);
          normal.set_normalized_dir(0, 1, 0);
        }
      }
    }

    // Z coordinate tests (ie. xOy faces).
    if (std::abs(ray.dir().z()) > data_t(0.00001))
    {
      data_t dist_cur = (_data._z_min - ray.pos().z()) / ray.dir().z();
      if (dist_cur > 0 && dist_cur < dist)
      {
        data_t x = ray.pos().x() + dist_cur * ray.dir().x();
        data_t y = ray.pos().y() + dist_cur * ray.dir().y();

        if (x > _data._x_min && x < _data._x_max &&
            y > _data._y_min && y < _data._y_max)
        {
          hit = true;
          dist = dist_cur;
          normal.pos().set(x, y, _data._z_min);
          normal.set_normalized_dir(0, 0, -1);
        }
      }

      dist_cur = (_data._z_max - ray.pos().z()) / ray.dir().z();
      if (dist_cur > 0 && dist_cur < dist)
      {
        data_t x = ray.pos().x() + dist_cur * ray.dir().x();
        data_t y = ray.pos().y() + dist_cur * ray.dir().y();

        if (x > _data._x_min && x < _data._x_max &&
            y > _data._y_min && y < _data._y_max)
        {
          hit = true;
          dist = dist_cur;
          normal.pos().set(x, y, _data._z_max);
          normal.set_normalized_dir(0, 0, 1);
        }
      }
    }

    return hit;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  std::optional<box3d_data<data_t>> box3d<data_t>::get_bounding_box(const ggo::basis3d<data_t> & basis) const
  {
    return box3d_data<data_t>::from({
      basis.point_from_local_to_world({ _data._x_min, _data._y_min, _data._z_min }),
      basis.point_from_local_to_world({ _data._x_min, _data._y_min, _data._z_max }),
      basis.point_from_local_to_world({ _data._x_min, _data._y_max, _data._z_min }),
      basis.point_from_local_to_world({ _data._x_min, _data._y_max, _data._z_max }),
      basis.point_from_local_to_world({ _data._x_max, _data._y_min, _data._z_min }),
      basis.point_from_local_to_world({ _data._x_max, _data._y_min, _data._z_max }),
      basis.point_from_local_to_world({ _data._x_max, _data._y_max, _data._z_min }),
      basis.point_from_local_to_world({ _data._x_max, _data._y_max, _data._z_max }) });
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  void box3d<data_t>::get_bounding_sphere(ggo::sphere3d<data_t> & bounding_sphere) const
  {
    bounding_sphere.center().x() = T(0.5) * (_x_min + _x_max);
    bounding_sphere.center().y() = T(0.5) * (_y_min + _y_max);
    bounding_sphere.center().z() = T(0.5) * (_z_min + _z_max);

    data_t dx = _x_max - _x_min;
    data_t dy = _y_max - _y_min;
    data_t dz = _z_max - _z_min;
    bounding_sphere.radius() = T(0.5) * std::sqrt(dx * dx + dy * dy + dz * dz);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  bool box3d<data_t>::is_point_inside(const ggo::pos3<data_t> & point) const
  {
    return point.x() >= _data._x_min && point.x() <= _data._x_max &&
           point.y() >= _data._y_min && point.y() <= _data._y_max &&
           point.z() >= _data._z_min && point.z() <= _data._z_max;
  }
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  bool test_intersection(const box3d<data_t> & b1, const box3d<data_t> & b2)
  {
    return test_intersection(b1.data(), b2.data());
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  box3d<data_t> get_union(const box3d<data_t> & b1, const box3d<data_t> & b2)
  {
    return get_union(b1.data(), b2.data());
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  box3d<data_t> get_union(const box3d<data_t> & box, const pos3<data_t> & p)
  {
    return get_union(box.data(), p);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  std::ostream & operator<<(std::ostream & os, const box3d<data_t> & box)
  {
    os << box.data();
    return os;
  }
}