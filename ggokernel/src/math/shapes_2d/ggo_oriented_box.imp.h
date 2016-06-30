namespace ggo
{
  //////////////////////////////////////////////////////////////////
  template <typename data_t>
  ggo::pos2<data_t> oriented_box<data_t>::operator[](int i) const
  {
    switch (i)
    {
    case 0:
      return _pos + _size1 * _dir + _size2 * dir2();
    case 1:
      return _pos + _size1 * _dir - _size2 * dir2();
    case 2:
      return _pos - _size1 * _dir - _size2 * dir2();
    case 3:
      return _pos - _size1 * _dir + _size2 * dir2();
    default:
      GGO_FAIL();
      return ggo::pos2<data_t>(0, 0);
    }
  }

  //////////////////////////////////////////////////////////////////
  template <typename data_t>
  void oriented_box<data_t>::rotate(data_t angle, const ggo::pos2<data_t> & center)
  {
    _pos = ggo::rotate(_pos, center, angle);
    _dir = ggo::rotate(_dir, angle);
  }
  
  //////////////////////////////////////////////////////////////////
  template <typename data_t>
  std::vector<ggo::pos2<data_t>> oriented_box<data_t>::get_points() const
  {
    return std::vector<ggo::pos2<data_t>> { _pos + _size1 * _dir + _size2 * dir2(),
                                            _pos + _size1 * _dir - _size2 * dir2(),
                                            _pos - _size1 * _dir - _size2 * dir2(),
                                            _pos - _size1 * _dir + _size2 * dir2() };
  }

  //////////////////////////////////////////////////////////////////
  template <typename data_t>
  bool oriented_box<data_t>::is_point_inside(data_t x, data_t y) const
  {
    ggo::pos2<data_t> diff(x - _pos.template get<0>(), y - _pos.template get<1>());

    data_t dot1 = ggo::dot(diff, _dir);
    if (std::abs(dot1) > _size1)
    {
      return false;
    }

    data_t dot2 = ggo::dot(diff, dir2());
    if (std::abs(dot2) > _size2)
    {
      return false;
    }

    return true;
  }
}
