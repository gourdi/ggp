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
  void oriented_box<T>::rotate(T angle)
  {
    _dir.rotate(angle);
  }
  
  //////////////////////////////////////////////////////////////////
  template <typename T>
  std::vector<ggo::set2<T>> oriented_box<T>::get_draw_points() const
  {
    return std::vector<ggo::set2<T>> { _pos + _size1 * _dir + _size2 * dir2(),
                                       _pos + _size1 * _dir - _size2 * dir2(),
                                       _pos - _size1 * _dir - _size2 * dir2(),
                                       _pos - _size1 * _dir + _size2 * dir2() };
  }
}
