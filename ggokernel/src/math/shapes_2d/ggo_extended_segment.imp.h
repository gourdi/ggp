namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename T>
  extended_segment<T>::extended_segment(const ggo::set2<T> & p1, const ggo::set2<T> & p2, T width)
    :
    _p1(p1), _p2(p2), _width(width)
  {
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  bool extended_segment<T>::is_point_inside(T x, T y) const
  {
    return ggo::segment<T>(_p1, _p2).dist_to_point(x, y) < _width;
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  rect_data<T> extended_segment<T>::get_bounding_rect() const
  {
    T left    = std::min(_p1.x(), _p2.x()) - _width;
    T right   = std::max(_p1.x(), _p2.x()) + _width;
    T bottom  = std::min(_p1.y(), _p2.y()) - _width;
    T top     = std::max(_p1.y(), _p2.y()) + _width;

    return { {left, bottom}, right - left, top - bottom };
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  rect_intersection extended_segment<T>::get_rect_intersection(const rect_data<T> & rect_data) const
  {
    T left    = rect_data._pos.x();
    T bottom  = rect_data._pos.y();
    T right   = left + rect_data._width;
    T top     = bottom + rect_data._height;

    ggo::segment<T> s1(left,  bottom, left,  top);    // Left rectangle edge.
    ggo::segment<T> s2(right, bottom, right, top);    // Right rectangle edge.
    ggo::segment<T> s3(left,  bottom, right, bottom); // Bottom rectangle edge.
    ggo::segment<T> s4(left,  top,    right, top);    // Top rectangle edge.
    ggo::segment<T> s(_p1, _p2);

    // Rectangle in shape.
    if (is_point_inside(left,  bottom) == true &&
        is_point_inside(right, bottom) == true &&
        is_point_inside(left,  top)    == true &&
        is_point_inside(right, top)    == true)
    {
      return rect_intersection::RECT_IN_SHAPE;
    }

    T squared_width = _width * _width;

    // Shape in rectangle?
    ggo::rect<T> rect(rect_data);
    if (rect.is_point_inside(_p1) == true && rect.is_point_inside(_p2) == true &&
        s.hypot_to_segment(s1) > squared_width &&
        s.hypot_to_segment(s2) > squared_width &&
        s.hypot_to_segment(s3) > squared_width &&
        s.hypot_to_segment(s4) > squared_width)
    {
      return rect_intersection::SHAPE_IN_RECT;
    }

    // Disjoints?
    if (s.hypot_to_segment(s1) > squared_width &&
        s.hypot_to_segment(s2) > squared_width &&
        s.hypot_to_segment(s3) > squared_width &&
        s.hypot_to_segment(s4) > squared_width)
    {
      return rect_intersection::DISJOINTS;
    }

    return rect_intersection::PARTIAL_OVERLAP;
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  T extended_segment<T>::dist_to_point(T x, T y) const 
  {
    T hypot = ggo::segment<T>(_p1, _p2).hypot_to_point(x, y);

    if (hypot < _width * _width)
    {
      return 0;
    }
    else
    {
      return std::sqrt(hypot) - _width;
    }
  }
}
