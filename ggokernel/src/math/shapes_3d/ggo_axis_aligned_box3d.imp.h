namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename T>
  axis_aligned_box3d<T>::axis_aligned_box3d(T x_min, T x_max, T y_min, T y_max, T z_min, T z_max)
  {
    _x_min = x_min;
    _x_max = x_max;
    _y_min = y_min;
    _y_max = y_max;
    _z_min = z_min;
    _z_max = z_max;
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  axis_aligned_box3d<T>::axis_aligned_box3d(const ggo::pos3<T> & p)
  {
    _x_min = p.template get<0>();
    _x_max = p.template get<0>();
    _y_min = p.template get<1>();
    _y_max = p.template get<1>();
    _z_min = p.template get<2>();
    _z_max = p.template get<2>();
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  ggo::pos3<T> axis_aligned_box3d<T>::get_center() const
  {
    T x = T(0.5) * (_x_min + _x_max);
    T y = T(0.5) * (_y_min + _y_max);
    T z = T(0.5) * (_z_min + _z_max);

    return ggo::pos3<T>(x, y, z);
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  bool axis_aligned_box3d<T>::intersect_ray(const ggo::ray3d<T> & ray) const
  {
    // X coordinate tests (ie. yOz faces).
    if (std::abs(ray.dir().template get<0>()) > T(0.00001))
    {
      T dist = (_x_min - ray.pos().template get<0>()) / ray.dir().template get<0>();
      if (dist > 0)
      {
        T y = ray.pos().template get<1>() + dist * ray.dir().template get<1>();
        T z = ray.pos().template get<2>() + dist * ray.dir().template get<2>();
    
        if (y > _y_min && y < _y_max && 
            z > _z_min && z < _z_max)
        {
          return true;
        }
      }

      dist = (_x_max - ray.pos().template get<0>()) / ray.dir().template get<0>();
      if (dist > 0)
      {
        T y = ray.pos().template get<1>() + dist * ray.dir().template get<1>();
        T z = ray.pos().template get<2>() + dist * ray.dir().template get<2>();
    
        if (y > _y_min && y < _y_max && 
            z > _z_min && z < _z_max)
        {
          return true;
        }
      }
    }

    // Y coordinate tests (ie. xOz faces).
    if (std::abs(ray.dir().template get<1>()) > T(0.00001))
    {
      T dist = (_y_min - ray.pos().template get<1>()) / ray.dir().template get<1>();
      if (dist > 0)
      {
        T x = ray.pos().template get<0>() + dist * ray.dir().template get<0>();
        T z = ray.pos().template get<2>() + dist * ray.dir().template get<2>();
    
        if (x > _x_min && x < _x_max && 
            z > _z_min && z < _z_max)
        {
          return true;		
        }
      }

      dist = (_y_max - ray.pos().template get<1>()) / ray.dir().template get<1>();
      if (dist > 0)
      {
        T x = ray.pos().template get<0>() + dist * ray.dir().template get<0>();
        T z = ray.pos().template get<2>() + dist * ray.dir().template get<2>();
    
        if (x > _x_min && x < _x_max && 
            z > _z_min && z < _z_max)
        {
          return true;
        }
      }
    }

    // Z coordinate tests (ie. xOy faces).
    if (std::abs(ray.dir().template get<2>()) > T(0.00001))
    {
      T dist = (_z_min - ray.pos().template get<2>()) / ray.dir().template get<2>();
      if (dist > 0)
      {
        T x = ray.pos().template get<0>() + dist * ray.dir().template get<0>();
        T y = ray.pos().template get<1>() + dist * ray.dir().template get<1>();
    
        if (x > _x_min && x < _x_max && 
            y > _y_min && y < _y_max)
        {
          return true;
        }
      }

      dist = (_z_max - ray.pos().template get<2>()) / ray.dir().template get<2>();
      if (dist > 0)
      {
        T x = ray.pos().template get<0>() + dist * ray.dir().template get<0>();
        T y = ray.pos().template get<1>() + dist * ray.dir().template get<1>();
    
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
  template <typename T>
  bool axis_aligned_box3d<T>::intersect_ray(const ggo::ray3d<T> & ray, T & dist, ggo::ray3d<T> & normal) const
  {
    bool hit = false;
    dist = std::numeric_limits<T>::max();

    // X coordinate tests (ie. yOz faces).
    if (std::abs(ray.dir().template get<0>()) > T(0.00001))
    {
      T dist_cur = (_x_min - ray.pos().template get<0>()) / ray.dir().template get<0>();
      if (dist_cur > 0 && dist_cur < dist)
      {
        T y = ray.pos().template get<1>() + dist_cur * ray.dir().template get<1>();
        T z = ray.pos().template get<2>() + dist_cur * ray.dir().template get<2>();

        if (y > _y_min && y < _y_max && 
            z > _z_min && z < _z_max)
        {
          hit = true;
          dist = dist_cur;
          normal.pos().set(_x_min, y, z);
          normal.set_normalized_dir(-1, 0, 0);	
        }
      }

      dist_cur = (_x_max - ray.pos().template get<0>()) / ray.dir().template get<0>();
      if (dist_cur > 0 && dist_cur < dist)
      {
        T y = ray.pos().template get<1>() + dist_cur * ray.dir().template get<1>();
        T z = ray.pos().template get<2>() + dist_cur * ray.dir().template get<2>();

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
    if (std::abs(ray.dir().template get<1>()) > T(0.00001))
    {
      T dist_cur = (_y_min - ray.pos().template get<1>()) / ray.dir().template get<1>();
      if (dist_cur > 0 && dist_cur < dist)
      {
        T x = ray.pos().template get<0>() + dist_cur * ray.dir().template get<0>();
        T z = ray.pos().template get<2>() + dist_cur * ray.dir().template get<2>();

        if (x > _x_min && x < _x_max && 
            z > _z_min && z < _z_max)
        {
          hit = true;
          dist = dist_cur;
          normal.pos().set(x, _y_min, z);
          normal.set_normalized_dir(0, -1, 0);		
        }
      }

      dist_cur = (_y_max - ray.pos().template get<1>()) / ray.dir().template get<1>();
      if (dist_cur > 0 && dist_cur < dist)
      {
        T x = ray.pos().template get<0>() + dist_cur * ray.dir().template get<0>();
        T z = ray.pos().template get<2>() + dist_cur * ray.dir().template get<2>();

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
    if (std::abs(ray.dir().template get<2>()) > T(0.00001))
    {
      T dist_cur = (_z_min - ray.pos().template get<2>()) / ray.dir().template get<2>();
      if (dist_cur > 0 && dist_cur < dist)
      {
        T x = ray.pos().template get<0>() + dist_cur * ray.dir().template get<0>();
        T y = ray.pos().template get<1>() + dist_cur * ray.dir().template get<1>();

        if (x > _x_min && x < _x_max && 
            y > _y_min && y < _y_max)
        {
          hit = true;
          dist = dist_cur;
          normal.pos().set(x, y, _z_min);
          normal.set_normalized_dir(0, 0, -1);
        }
      }

      dist_cur = (_z_max - ray.pos().template get<2>()) / ray.dir().template get<2>();
      if (dist_cur > 0 && dist_cur < dist)
      {
        T x = ray.pos().template get<0>() + dist_cur * ray.dir().template get<0>();
        T y = ray.pos().template get<1>() + dist_cur * ray.dir().template get<1>();

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
  template <typename T>
  void axis_aligned_box3d<T>::get_bounding_sphere(ggo::sphere3d<T> & bounding_sphere) const
  {
    bounding_sphere.center().template get<0>() = T(0.5) * (_x_min + _x_max);
    bounding_sphere.center().template get<1>() = T(0.5) * (_y_min + _y_max);
    bounding_sphere.center().template get<2>() = T(0.5) * (_z_min + _z_max);

    T dx = _x_max - _x_min;
    T dy = _y_max - _y_min;
    T dz = _z_max - _z_min;
    bounding_sphere.radius() = T(0.5) * std::sqrt(dx * dx + dy * dy + dz * dz);
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  void axis_aligned_box3d<T>::merge_with(const axis_aligned_box3d & box)
  {
    _x_min = std::min(_x_min, box._x_min);
    _x_max = std::max(_x_max, box._x_max);
    _y_min = std::min(_y_min, box._y_min);
    _y_max = std::max(_y_max, box._y_max);
    _z_min = std::min(_z_min, box._z_min);
    _z_max = std::max(_z_max, box._z_max);
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  void axis_aligned_box3d<T>::merge_with(const ggo::pos3<T> & point)
  {
    _x_min = std::min(_x_min, point.template get<0>());
    _x_max = std::max(_x_max, point.template get<0>());
    _y_min = std::min(_y_min, point.template get<1>());
    _y_max = std::max(_y_max, point.template get<1>());
    _z_min = std::min(_z_min, point.template get<2>());
    _z_max = std::max(_z_max, point.template get<2>());
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  bool axis_aligned_box3d<T>::is_point_inside(const ggo::pos3<T> & point) const
  {
    return point.template get<0>() >= _x_min && point.template get<0>() <= _x_max &&
           point.template get<1>() >= _y_min && point.template get<1>() <= _y_max &&
           point.template get<2>() >= _z_min && point.template get<2>() <= _z_max;
  }
  
  //////////////////////////////////////////////////////////////
  template <typename T>
  std::ostream & axis_aligned_box3d<T>::operator<<(std::ostream & os) const
  {
    os << "aabb (" << _x_min << ", " << _x_max << ", " << _y_min << ", " << _y_max << ", " << _z_min << ", " << _z_max << ")";
    return os;
  }
}