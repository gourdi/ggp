namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  basis2d<data_t>::basis2d()
  :
  _pos(0, 0), _x(1, 0), _y(0, 1)
  {
  }


  //////////////////////////////////////////////////////////////
  template <typename data_t>
  basis2d<data_t>::basis2d(const ggo::pos2<data_t> & pos, const ggo::pos2<data_t> & x, const ggo::pos2<data_t> & y)
  :
  _pos(pos), _x(x), _y(y)
  {
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  void basis2d<data_t>::move(const vec2<data_t> & d)
  {
    _pos += d;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  void basis2d<data_t>::rotate(data_t angle)
  {
    data_t cos_tmp = std::cos(angle);
    data_t sin_tmp = std::sin(angle);

    vec2<data_t> new_x(_x.x() * cos_tmp - _x.y() * sin_tmp, _x.x() * sin_tmp + _x.y() * cos_tmp);
    _x = new_x;

    vec2<data_t> new_y(_y.x() * cos_tmp - _y.y() * sin_tmp, _y.x() * sin_tmp + _y.y() * cos_tmp);
    _y = new_y;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  void basis2d<data_t>::rotate(const vec2<data_t> & center, data_t angle)
  {
    data_t cos_tmp = std::cos(angle);
    data_t sin_tmp = std::sin(angle);

    _pos -= center;
    pos2<data_t> new_pos(_pos.x() * cos_tmp - _pos.y() * sin_tmp, _pos.x() * sin_tmp + _pos.y() * cos_tmp);
    _pos = new_pos + center;

    vec2<data_t> new_x(_x.x() * cos_tmp - _x.y() * sin_tmp, _x.x() * sin_tmp + _x.y() * cos_tmp);
    _x = new_x;

    vec2<data_t> new_y(_y.x() * cos_tmp - _y.y() * sin_tmp, _y.x() * sin_tmp + _y.y() * cos_tmp);
    _y = new_y;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  ggo::vec2<data_t> basis2d<data_t>::vector_from_local_to_world(const ggo::vec2<data_t> & v) const
  {
    return v.x() * _x + v.y() * _y;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  ggo::vec2<data_t> basis2d<data_t>::vector_from_world_to_local(const ggo::vec2<data_t> & v) const
  {
    const data_t m[2][2] = {
      { _x.x(), _y.x() },
      { _x.y(), _y.y() } };
    const data_t c[2] = { v.x(), v.y() };

    ggo::vec2<data_t> result;

    if (linsolve2d(m, c, result._coefs) == false)
    {
      throw std::runtime_error("invalid basis");
    }

    return result;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  ggo::pos2<data_t> basis2d<data_t>::point_from_local_to_world(const ggo::pos2<data_t> & p) const
  {
    return vector_from_local_to_world(p) + _pos;
  }
  
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  ggo::pos2<data_t> basis2d<data_t>::point_from_world_to_local(const ggo::pos2<data_t> & p) const
  {
    return vector_from_world_to_local(p - _pos);
  }
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t, bool orthonormal, bool cross_product_up>
  orthogonal_basis2d<data_t, orthonormal, cross_product_up>::orthogonal_basis2d()
    :
    _pos(0, 0), _x(1, 0)
  {
  }


  //////////////////////////////////////////////////////////////
  template <typename data_t, bool orthonormal, bool cross_product_up>
  orthogonal_basis2d<data_t, orthonormal, cross_product_up>::orthogonal_basis2d(const ggo::pos2<data_t> & pos, const ggo::pos2<data_t> & x)
    :
    _pos(pos), _x(orthonormal ? normalize(x) : x)
  {
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, bool orthonormal, bool cross_product_up>
  void orthogonal_basis2d<data_t, orthonormal, cross_product_up>::move(const vec2<data_t> & d)
  {
    _pos += d;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, bool orthonormal, bool cross_product_up>
  void orthogonal_basis2d<data_t, orthonormal, cross_product_up>::rotate(data_t angle)
  {
    data_t cos_tmp = std::cos(angle);
    data_t sin_tmp = std::sin(angle);

    _x = vec2<data_t>(_x.x() * cos_tmp - _x.y() * sin_tmp, _x.x() * sin_tmp + _x.y() * cos_tmp);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, bool orthonormal, bool cross_product_up>
  void orthogonal_basis2d<data_t, orthonormal, cross_product_up>::rotate(const vec2<data_t> & center, data_t angle)
  {
    data_t cos_tmp = std::cos(angle);
    data_t sin_tmp = std::sin(angle);

    _pos -= center;
    pos2<data_t> new_pos(_pos.x() * cos_tmp - _pos.y() * sin_tmp, _pos.x() * sin_tmp + _pos.y() * cos_tmp);
    _pos = new_pos + center;

    _x = vec2<data_t>(_x.x() * cos_tmp - _x.y() * sin_tmp, _x.x() * sin_tmp + _x.y() * cos_tmp);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, bool orthonormal, bool cross_product_up>
  data_t orthogonal_basis2d<data_t, orthonormal, cross_product_up>::from_local_to_world(data_t length) const
  {
    return orthonormal ? length : length * _x.get_length();
  }
  
  //////////////////////////////////////////////////////////////
  template <typename data_t, bool orthonormal, bool cross_product_up>
  data_t orthogonal_basis2d<data_t, orthonormal, cross_product_up>::from_world_to_local(data_t length) const
  {
    return orthonormal ? length : length / _x.get_length();
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, bool orthonormal, bool cross_product_up>
  ggo::vec2<data_t> orthogonal_basis2d<data_t, orthonormal, cross_product_up>::vector_from_local_to_world(const ggo::vec2<data_t> & v) const
  {
    return v.x() * _x + v.y() * y();
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, bool orthonormal, bool cross_product_up>
  ggo::vec2<data_t> orthogonal_basis2d<data_t, orthonormal, cross_product_up>::vector_from_world_to_local(const ggo::vec2<data_t> & v) const
  {
    if constexpr(orthonormal == true)
    {
      return { ggo::dot(v, _x), ggo::dot(v, y()) };
    }
    else
    {
      float inv_hypot = 1 / hypot(_x);
      return { ggo::dot(v, _x) * inv_hypot, ggo::dot(v, y()) * inv_hypot };
    }
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, bool orthonormal, bool cross_product_up>
  ggo::pos2<data_t> orthogonal_basis2d<data_t, orthonormal, cross_product_up>::point_from_local_to_world(const ggo::pos2<data_t> & p) const
  {
    return vector_from_local_to_world(p) + _pos;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, bool orthonormal, bool cross_product_up>
  ggo::pos2<data_t> orthogonal_basis2d<data_t, orthonormal, cross_product_up>::point_from_world_to_local(const ggo::pos2<data_t> & p) const
  {
    return vector_from_world_to_local(p - _pos);
  }
}


