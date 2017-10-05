namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  axis_aligned_box3d<data_t>::axis_aligned_box3d(data_t x_min, data_t x_max, data_t y_min, data_t y_max, data_t z_min, data_t z_max)
  {
    _x_min = x_min;
    _x_max = x_max;
    _y_min = y_min;
    _y_max = y_max;
    _z_min = z_min;
    _z_max = z_max;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  axis_aligned_box3d<data_t>::axis_aligned_box3d(const ggo::pos3<data_t> & p)
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
  ggo::pos3<data_t> axis_aligned_box3d<data_t>::get_center() const
  {
    data_t x = T(0.5) * (_x_min + _x_max);
    data_t y = T(0.5) * (_y_min + _y_max);
    data_t z = T(0.5) * (_z_min + _z_max);

    return ggo::pos3<data_t>(x, y, z);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  bool axis_aligned_box3d<data_t>::intersect_ray(const ggo::ray3d<data_t> & ray) const
  {
    // X coordinate tests (ie. yOz faces).
    if (std::abs(ray.dir().x()) > data_t(0.00001))
    {
      data_t dist = (_x_min - ray.pos().x()) / ray.dir().x();
      if (dist > 0)
      {
        data_t y = ray.pos().y() + dist * ray.dir().y();
        data_t z = ray.pos().z() + dist * ray.dir().z();
    
        if (y > _y_min && y < _y_max && 
            z > _z_min && z < _z_max)
        {
          return true;
        }
      }

      dist = (_x_max - ray.pos().x()) / ray.dir().x();
      if (dist > 0)
      {
        data_t y = ray.pos().y() + dist * ray.dir().y();
        data_t z = ray.pos().z() + dist * ray.dir().z();
    
        if (y > _y_min && y < _y_max && 
            z > _z_min && z < _z_max)
        {
          return true;
        }
      }
    }

    // Y coordinate tests (ie. xOz faces).
    if (std::abs(ray.dir().y()) > data_t(0.00001))
    {
      data_t dist = (_y_min - ray.pos().y()) / ray.dir().y();
      if (dist > 0)
      {
        data_t x = ray.pos().x() + dist * ray.dir().x();
        data_t z = ray.pos().z() + dist * ray.dir().z();
    
        if (x > _x_min && x < _x_max && 
            z > _z_min && z < _z_max)
        {
          return true;		
        }
      }

      dist = (_y_max - ray.pos().y()) / ray.dir().y();
      if (dist > 0)
      {
        data_t x = ray.pos().x() + dist * ray.dir().x();
        data_t z = ray.pos().z() + dist * ray.dir().z();
    
        if (x > _x_min && x < _x_max && 
            z > _z_min && z < _z_max)
        {
          return true;
        }
      }
    }

    // Z coordinate tests (ie. xOy faces).
    if (std::abs(ray.dir().z()) > data_t(0.00001))
    {
      data_t dist = (_z_min - ray.pos().z()) / ray.dir().z();
      if (dist > 0)
      {
        data_t x = ray.pos().x() + dist * ray.dir().x();
        data_t y = ray.pos().y() + dist * ray.dir().y();
    
        if (x > _x_min && x < _x_max && 
            y > _y_min && y < _y_max)
        {
          return true;
        }
      }

      dist = (_z_max - ray.pos().z()) / ray.dir().z();
      if (dist > 0)
      {
        data_t x = ray.pos().x() + dist * ray.dir().x();
        data_t y = ray.pos().y() + dist * ray.dir().y();
    
        if (x > _x_min && x < _x_max && 
            y > _y_min && y < _y_max)
        {
          return true;
        }
      }
    }
    
    return false;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  bool axis_aligned_box3d<data_t>::intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const
  {
    bool hit = false;
    dist = std::numeric_limits<data_t>::max();

    // X coordinate tests (ie. yOz faces).
    if (std::abs(ray.dir().x()) > data_t(0.00001))
    {
      data_t dist_cur = (_x_min - ray.pos().x()) / ray.dir().x();
      if (dist_cur > 0 && dist_cur < dist)
      {
        data_t y = ray.pos().y() + dist_cur * ray.dir().y();
        data_t z = ray.pos().z() + dist_cur * ray.dir().z();

        if (y > _y_min && y < _y_max && 
            z > _z_min && z < _z_max)
        {
          hit = true;
          dist = dist_cur;
          normal.pos().set(_x_min, y, z);
          normal.set_normalized_dir(-1, 0, 0);	
        }
      }

      dist_cur = (_x_max - ray.pos().x()) / ray.dir().x();
      if (dist_cur > 0 && dist_cur < dist)
      {
        data_t y = ray.pos().y() + dist_cur * ray.dir().y();
        data_t z = ray.pos().z() + dist_cur * ray.dir().z();

        if (y > _y_min && y < _y_max && 
            z > _z_min && z < _z_max)
        {
          hit = true;
          dist = dist_cur;
          normal.pos().set(_x_max, y, z);
          normal.set_normalized_dir(1, 0, 0);	
        }
      }
    }

    // Y coordinate tests (ie. xOz faces).
    if (std::abs(ray.dir().y()) > data_t(0.00001))
    {
      data_t dist_cur = (_y_min - ray.pos().y()) / ray.dir().y();
      if (dist_cur > 0 && dist_cur < dist)
      {
        data_t x = ray.pos().x() + dist_cur * ray.dir().x();
        data_t z = ray.pos().z() + dist_cur * ray.dir().z();

        if (x > _x_min && x < _x_max && 
            z > _z_min && z < _z_max)
        {
          hit = true;
          dist = dist_cur;
          normal.pos().set(x, _y_min, z);
          normal.set_normalized_dir(0, -1, 0);		
        }
      }

      dist_cur = (_y_max - ray.pos().y()) / ray.dir().y();
      if (dist_cur > 0 && dist_cur < dist)
      {
        data_t x = ray.pos().x() + dist_cur * ray.dir().x();
        data_t z = ray.pos().z() + dist_cur * ray.dir().z();

        if (x > _x_min && x < _x_max && 
            z > _z_min && z < _z_max)
        {
          hit = true;
          dist = dist_cur;
          normal.pos().set(x, _y_max, z);
          normal.set_normalized_dir(0, 1, 0);
        }
      }
    }

    // Z coordinate tests (ie. xOy faces).
    if (std::abs(ray.dir().z()) > data_t(0.00001))
    {
      data_t dist_cur = (_z_min - ray.pos().z()) / ray.dir().z();
      if (dist_cur > 0 && dist_cur < dist)
      {
        data_t x = ray.pos().x() + dist_cur * ray.dir().x();
        data_t y = ray.pos().y() + dist_cur * ray.dir().y();

        if (x > _x_min && x < _x_max && 
            y > _y_min && y < _y_max)
        {
          hit = true;
          dist = dist_cur;
          normal.pos().set(x, y, _z_min);
          normal.set_normalized_dir(0, 0, -1);
        }
      }

      dist_cur = (_z_max - ray.pos().z()) / ray.dir().z();
      if (dist_cur > 0 && dist_cur < dist)
      {
        data_t x = ray.pos().x() + dist_cur * ray.dir().x();
        data_t y = ray.pos().y() + dist_cur * ray.dir().y();

        if (x > _x_min && x < _x_max && 
            y > _y_min && y < _y_max)
        {
          hit = true;
          dist = dist_cur;
          normal.pos().set(x, y, _z_max);
          normal.set_normalized_dir(0, 0, 1);
        }
      }
    }

    return hit;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  void axis_aligned_box3d<data_t>::get_bounding_sphere(ggo::sphere3d<data_t> & bounding_sphere) const
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
  void axis_aligned_box3d<data_t>::merge_with(const axis_aligned_box3d & box)
  {
    _x_min = std::min(_x_min, box._x_min);
    _x_max = std::max(_x_max, box._x_max);
    _y_min = std::min(_y_min, box._y_min);
    _y_max = std::max(_y_max, box._y_max);
    _z_min = std::min(_z_min, box._z_min);
    _z_max = std::max(_z_max, box._z_max);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  void axis_aligned_box3d<data_t>::merge_with(const ggo::pos3<data_t> & point)
  {
    _x_min = std::min(_x_min, point.x());
    _x_max = std::max(_x_max, point.x());
    _y_min = std::min(_y_min, point.y());
    _y_max = std::max(_y_max, point.y());
    _z_min = std::min(_z_min, point.z());
    _z_max = std::max(_z_max, point.z());
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  bool axis_aligned_box3d<data_t>::is_point_inside(const ggo::pos3<data_t> & point) const
  {
    return point.x() >= _x_min && point.x() <= _x_max &&
           point.y() >= _y_min && point.y() <= _y_max &&
           point.z() >= _z_min && point.z() <= _z_max;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  bool intersect(const axis_aligned_box3d<data_t> & b1, const axis_aligned_box3d<data_t> & b2)
  {
    auto intersect1d = [](data_t inf1, data_t sup1, data_t inf2, data_t sup2)
    {
      return sup2 >= inf1 && inf2 <= sup1;
    };

    return
      intersect1d(b1.x_min(), b1.x_max(), b2.x_min(), b2.x_max()) &&
      intersect1d(b1.y_min(), b1.y_max(), b2.y_min(), b2.y_max()) &&
      intersect1d(b1.z_min(), b1.z_max(), b2.z_min(), b2.z_max());
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  std::ostream & operator<<(std::ostream & os, const axis_aligned_box3d<data_t> & box)
  {
    os << "(x_min=" << box.x_min();
    os << ", x_max=" << box.x_max();
    os << ", y_min=" << box.y_min();
    os << ", y_max=" << box.y_max();
    os << ", z_min=" << box.z_min();
    os << ", z_max=" << box.z_max() << ")";
    return os;
  }
}