namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename T>
  T disc<T>::dist_to_point(T x, T y) const
  {
    T dx	        = _center.x() - x;
    T dy	        = _center.y() - y;
    T hypot	      = dx * dx + dy * dy;
    T sqrd_radius = _radius * _radius;

    if (hypot > sqrd_radius)
    {
      return std::sqrt(hypot) - _radius;
    }
    else
    {
      return 0;
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  rect_data<T> disc<T>::get_bounding_rect() const
  {
    T diameter = 2 * _radius;

    return { { _center.x() - _radius, _center.y() - _radius }, diameter, diameter };
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  bool disc<T>::is_point_inside(T x, T y) const
  {
    T dx 	  = _center.x() - x;
    T dy 	  = _center.y() - y;
    T hypot	= dx * dx + dy * dy;

    return hypot <= _radius * _radius;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  rect_intersection disc<T>::get_rect_intersection(const rect_data<T> & rect_data) const
  {
    T left    = rect_data._pos.x();
    T bottom  = rect_data._pos.y();
    T right   = left + rect_data._width;
    T top     = bottom + rect_data._height;

    // Rectangle in circle?
    ggo::set2<T> p1(left,  bottom);
    ggo::set2<T> p2(left,  top);
    ggo::set2<T> p3(right, bottom);
    ggo::set2<T> p4(right, top);

    T hypot = _radius * _radius;
    T hypot1 = ggo::hypot(_center, p1);
    T hypot2 = ggo::hypot(_center, p2);
    T hypot3 = ggo::hypot(_center, p3);
    T hypot4 = ggo::hypot(_center, p4);

    if (hypot1 < hypot && hypot2 < hypot && hypot3 < hypot && hypot4 < hypot)
    {
      return rect_intersection::RECT_IN_SHAPE;
    }

    // Circle in rectangle?
    if (_center.x() - left   > _radius &&
        _center.y() - bottom > _radius &&
        right - _center.x()  > _radius &&
        top - _center.y()    > _radius)
    {
      return rect_intersection::SHAPE_IN_RECT;
    }

    // Partial overlap?
    if (segment_intersect_border(left, bottom, left, top) == true ||
        segment_intersect_border(right, bottom, right, top) == true ||
        segment_intersect_border(left, bottom, right, bottom) == true ||
        segment_intersect_border(left, top, right, top) == true)
    {
      return rect_intersection::PARTIAL_OVERLAP;
    }

    return rect_intersection::DISJOINTS;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  bool disc<T>::segment_intersect_border(T x_from, T y_from, T x_to, T y_to) const
  {
    return ggo::circle<T>(_center, _radius).intersect_segment(x_from, y_from, x_to, y_to);
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  std::vector<ggo::set2<T>> disc<T>::get_draw_points() const
  {
    const int POINTS_COUNT = 32;
    std::vector<ggo::set2<T>> points;
    for (int i = 0; i < POINTS_COUNT; ++i)
    {
      float angle = i * 2 * ggo::PI<T>() / POINTS_COUNT;
      points.push_back(_center + ggo::set2<T>::from_polar(angle, _radius));
    }
    return points;
  }
}