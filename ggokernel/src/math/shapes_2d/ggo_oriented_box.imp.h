namespace ggo
{
  //////////////////////////////////////////////////////////////////
  template <typename T>
  ggo::set2<T> oriented_box<T>::operator[](int i) const
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
      return ggo::set2<T>(0, 0);
    }
  }

  //////////////////////////////////////////////////////////////////
  template <typename T>
  void oriented_box<T>::rotate(T angle, const ggo::set2<T> & center)
  {
    _pos.rotate(angle, center);
    _dir.rotate(angle);
  }
  
  //////////////////////////////////////////////////////////////////
  template <typename T>
  std::vector<ggo::set2<T>> oriented_box<T>::get_points() const
  {
    return std::vector<ggo::set2<T>> { _pos + _size1 * _dir + _size2 * dir2(),
                                       _pos + _size1 * _dir - _size2 * dir2(),
                                       _pos - _size1 * _dir - _size2 * dir2(),
                                       _pos - _size1 * _dir + _size2 * dir2() };
  }

  //////////////////////////////////////////////////////////////////
  template <typename T>
  bool oriented_box<T>::is_point_inside(T x, T y) const
  {
    ggo::set2<T> diff(x - _pos.x(), y - _pos.y());

    T dot1 = ggo::dot(diff, _dir);
    if (std::abs(dot1) > _size1)
    {
      return false;
    }

    T dot2 = ggo::dot(diff, dir2());
    if (std::abs(dot2) > _size2)
    {
      return false;
    }

    return true;
  }
}
