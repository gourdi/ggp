namespace ggo
{
  //////////////////////////////////////////////////////////////////
  template <typename T>
  void oriented_box<T>::rotate(T angle, const ggo::set2<T> & center)
  {
    _dir.rotate(angle);
    _pos.rotate(angle, center);
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
