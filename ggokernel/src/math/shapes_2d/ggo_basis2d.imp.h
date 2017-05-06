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
}