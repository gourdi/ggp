//////////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename data_t>
  struct box3d_data
  {
    box3d_data(const box3d_data<data_t> & data)
      :
      _x_min(data._x_min),
      _x_max(data._x_max),
      _y_min(data._y_min),
      _y_max(data._y_max),
      _z_min(data._z_min),
      _z_max(data._z_max)
    {
    }

    box3d_data(data_t x_min, data_t x_max, data_t y_min, data_t y_max, data_t z_min, data_t z_max)
      :
      _x_min(x_min),
      _x_max(x_max),
      _y_min(y_min),
      _y_max(y_max),
      _z_min(z_min),
      _z_max(z_max)
    {
    }

    data_t get_size_x() const { return _x_max - _x_min; }
    data_t get_size_y() const { return _y_max - _y_min; }
    data_t get_size_z() const { return _z_max - _z_min; }

    ggo::pos3<data_t> get_center() const { return { (_x_min + _x_max) / 2, (_y_min + _y_max) / 2, (_z_min + _z_max) / 2 }; }

    static box3d_data from(std::initializer_list<ggo::pos3<data_t>> points)
    {
      if (points.begin() == points.end())
      {
        throw std::runtime_error("empty points list");
      }

      auto it = points.begin();

      data_t x_min = it->x();
      data_t x_max = it->x();
      data_t y_min = it->y();
      data_t y_max = it->y();
      data_t z_min = it->z();
      data_t z_max = it->z();

      ++it;

      for (; it != points.end(); ++it)
      {
        x_min = std::min(x_min, it->x());
        x_max = std::max(x_max, it->x());
        y_min = std::min(y_min, it->y());
        y_max = std::max(y_max, it->y());
        z_min = std::min(z_min, it->z());
        z_max = std::max(z_max, it->z());
      }

      return { x_min, x_max, y_min, y_max, z_min, z_max };
    }

    data_t	_x_min;
    data_t	_x_max;
    data_t	_y_min;
    data_t	_y_max;
    data_t	_z_min;
    data_t	_z_max;
  };
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  box3d_data<data_t> get_union(const box3d_data<data_t> & b1, const box3d_data<data_t> & b2)
  {
    return {
      std::min(b1._x_min, b2._x_min),
      std::max(b1._x_max, b2._x_max),
      std::min(b1._y_min, b2._y_min),
      std::max(b1._y_max, b2._y_max),
      std::min(b1._z_min, b2._z_min),
      std::max(b1._z_max, b2._z_max) };
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  box3d_data<data_t> get_union(const box3d_data<data_t> & box, const pos3<data_t> & p)
  {
    return {
      std::min(box._x_min, p.x()),
      std::max(box._x_max, p.x()),
      std::min(box._y_min, p.y()),
      std::max(box._y_max, p.y()),
      std::min(box._z_min, p.z()),
      std::max(box._z_max, p.z()) };
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  bool test_intersection(const box3d_data<data_t> & b1, const box3d_data<data_t> & b2)
  {
    auto intersect1d = [](data_t inf1, data_t sup1, data_t inf2, data_t sup2)
    {
      return sup2 >= inf1 && inf2 <= sup1;
    };

    return
      intersect1d(b1._x_min, b1._x_max, b2._x_min, b2._x_max) &&
      intersect1d(b1._y_min, b1._y_max, b2._y_min, b2._y_max) &&
      intersect1d(b1._z_min, b1._z_max, b2._z_min, b2._z_max);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, typename it, typename get_shape_func>
  box3d_data<data_t> get_bounding_box(it begin, it end, get_shape_func get_shape)
  {

  }
}
