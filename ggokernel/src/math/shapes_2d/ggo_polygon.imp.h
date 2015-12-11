namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename T>
  void polygon2d<T>::create_oriented_box(const ggo::set2<T> & center, const ggo::set2<T> & direction, T size1, T size2, polygon2d<T> & polygon)
  {
    // Normalize the direction.
    T len = direction.get_length();
    ggo::set2<T> norm(direction);
    if (len > 0)
    {
      norm /= len;
    }

    polygon.clear();

    T x1 = center.x() + size1*norm.x() + size2*norm.y();
    T y1 = center.y() + size1*norm.y() - size2*norm.x();
    polygon.add_point(x1, y1);

    T x2 = center.x() + size1*norm.x() - size2*norm.y();
    T y2 = center.y() + size1*norm.y() + size2*norm.x();
    polygon.add_point(x2, y2);

    T x3 = center.x() - size1*norm.x() - size2*norm.y();
    T y3 = center.y() - size1*norm.y() + size2*norm.x();
    polygon.add_point(x3, y3);

    T x4 = center.x() - size1*norm.x() + size2*norm.y();
    T y4 = center.y() - size1*norm.y() - size2*norm.x();
    polygon.add_point(x4, y4);
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  void polygon2d<T>::create_axis_aligned_box(T left, T right, T top, T bottom, polygon2d<T> & polygon)
  {
    polygon.clear();

    if (left > right) { std::swap(left, right); }
    if (bottom > top) { std::swap(top, bottom); }

    polygon.add_point(left, top);
    polygon.add_point(right, top);
    polygon.add_point(right, bottom);
    polygon.add_point(left, bottom);
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  T polygon2d<T>::dist_to_point(T x, T y) const
  {
    switch (_points.size())
    {
    case 0:
      GGO_FAIL();
      return -1;
    case 1:
      return ggo::distance(x, y, _points.front().x(), _points.front().y());
    }

    if (is_point_inside(x, y) == true)
    {
      return 0;	
    }

    // First point to last point edge.
    T hypot = ggo::segment<T>(_points.front(), _points.back()).hypot_to_point(x, y);

    // Other edges.
    for (auto it = _points.begin(); it + 1 != _points.end(); ++it)
    {
      hypot = std::min(hypot, ggo::segment<T>(*it, *(it + 1)).hypot_to_point(x, y));
    }

    return std::sqrt(hypot);
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  rect_data<T> polygon2d<T>::get_bounding_rect() const
  {
    if (_points.empty() == true)
    {
      GGO_FAIL();
    }

    auto it 	= _points.begin();
    T left    = it->x();
    T right   = it->x();
    T bottom  = it->y();
    T top     = it->y();
    it++;

    while (it != _points.end())
    {
      left	  = std::min<T>(it->x(), left);
      right	  = std::max<T>(it->x(), right);
      bottom	= std::min<T>(it->y(), bottom);
      top		  = std::max<T>(it->y(), top);

      it++;
    }

    rect_data<T> rect_data;
    rect_data._pos = { left, bottom };
    rect_data._width = right - left;
    rect_data._height = top - bottom;

    return rect_data;
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  bool polygon2d<T>::is_point_inside(T x, T y) const
  {
    if (_points.size() <= 2)
    {
      return false; 
    }

    T angle(0);
    T angle1 = atan2(_points.back().y() - y, _points.back().x() - x);
    for (const auto & point : _points)
    {
      T angle2 = atan2(point.y() - y, point.x() - x);
      T dangle = angle2 - angle1;
      if (dangle >  PI<T>()) { dangle -= 2 * PI<T>(); }
      if (dangle < -PI<T>()) { dangle += 2 * PI<T>(); }
      angle += dangle;
      angle1 = angle2;
    }

    return std::abs(angle) > PI<T>();
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  T polygon2d<T>::dist_to_segment(T x_from, T y_from, T x_to, T y_to) const
  {
    ggo::segment<T> segment(x_from, y_from, x_to, y_to);

    switch (_points.size())
    {
    case 0:
      GGO_FAIL();
      return -1;
    case 1:
      return segment.dist_to_point(_points[0]);
    }

    if (is_point_inside(x_from, y_from) == true || is_point_inside(x_to, y_to) == true) 
    {
      return 0;
    }

    T hypot = ggo::segment<T>(_points[0], _points[_points.size() - 1]).hypot_to_segment(segment);
    if (hypot == 0)
    {
      return 0;
    }

    for (size_t i = 0; i < _points.size() - 1; ++i)
    {
      T hypot_cur = ggo::segment<T>(_points[i], _points[i + 1]).hypot_to_segment(segment);
      if (hypot_cur == 0)
      {
        return 0;
      }

      hypot = std::min(hypot, hypot_cur);
    }

    return std::sqrt(hypot);
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  T polygon2d<T>::dist_to_segment(const ggo::set2<T> & p1, const ggo::set2<T> & p2) const
  {
    return dist_to_segment(p1.x(), p1.y(), p2.x(), p2.y());
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  T polygon2d<T>::dist_to_segment(const ggo::segment<T> & segment) const
  {
    return dist_to_segment(segment.from(), segment.to());
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  ggo::rect_intersection polygon2d<T>::get_rect_intersection(const rect_data<T> & rect_data) const
  {
    T left    = rect_data._pos.x();
    T bottom  = rect_data._pos.y();
    T right   = left + rect_data._width;
    T top     = bottom + rect_data._height;
    
    // Border intersection => partial overlap.
    for (size_t i = 0; i < _points.size(); ++i)
    {
      ggo::segment<T> segment(_points[i], _points[(i + 1) % _points.size()]);

      if (segment.intersect_horizontal_segment(left, right, bottom) == true ||
          segment.intersect_horizontal_segment(left, right, top   ) == true)
      {
        return ggo::rect_intersection::PARTIAL_OVERLAP;
      }

      if (segment.intersect_vertical_segment(left,  bottom, top) == true ||
          segment.intersect_vertical_segment(right, bottom, top) == true)
      {
        return ggo::rect_intersection::PARTIAL_OVERLAP;
      }
    }

    // If there is no border intersection, there are 3 cases left:
    // - polygon in rect
    // - rect in polygon
    // - disjoint

    ggo::rect<T> rect(rect_data);
    
    // Check if the polygon is inside the rect. Since there is no border
    // intersection, we just need to check if a point of the polygon 
    // is inside the rect.
    
    if (rect.is_point_inside(_points[0]) == true)
    {
      return ggo::rect_intersection::SHAPE_IN_RECT;
    }

    // Check if the rect in inside the polygon. Since there is no border
    // intersection, we just need to check that a point of the rectangle 
    // is inside the polygon.
    if (is_point_inside(left, bottom) == true)
    {
      return ggo::rect_intersection::RECT_IN_SHAPE;
    }

    return ggo::rect_intersection::DISJOINTS;
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  void polygon2d<T>::move(T dx, T dy)
  {
    for (auto & point : _points)
    {
      point.x() += dx;
      point.y() += dy;
    }
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  void polygon2d<T>::rotate(T angle, const ggo::set2<T> & center)
  {
    for (auto & point : _points)
    {
      point.rotate(angle, center);
    }
  }
}
