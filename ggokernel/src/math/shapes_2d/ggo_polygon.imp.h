namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  void polygon2d<data_t>::create_oriented_box(const ggo::pos2<data_t> & center, const ggo::pos2<data_t> & direction, data_t size1, data_t size2, polygon2d<data_t> & polygon)
  {
    // Normalize the direction.
    data_t len = direction.get_length();
    ggo::pos2<data_t> norm(direction);
    if (len > 0)
    {
      norm /= len;
    }

    polygon.clear();

    data_t x1 = center.x() + size1*norm.x() + size2*norm.y();
    data_t y1 = center.y() + size1*norm.y() - size2*norm.x();
    polygon.add_point(x1, y1);

    data_t x2 = center.x() + size1*norm.x() - size2*norm.y();
    data_t y2 = center.y() + size1*norm.y() + size2*norm.x();
    polygon.add_point(x2, y2);

    data_t x3 = center.x() - size1*norm.x() - size2*norm.y();
    data_t y3 = center.y() - size1*norm.y() + size2*norm.x();
    polygon.add_point(x3, y3);

    data_t x4 = center.x() - size1*norm.x() + size2*norm.y();
    data_t y4 = center.y() - size1*norm.y() - size2*norm.x();
    polygon.add_point(x4, y4);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  void polygon2d<data_t>::create_axis_aligned_box(data_t left, data_t right, data_t top, data_t bottom, polygon2d<data_t> & polygon)
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
  template <typename data_t>
  data_t polygon2d<data_t>::dist_to_point(data_t x, data_t y) const
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
    data_t hypot = ggo::segment<data_t>(_points.front(), _points.back()).hypot_to_point(x, y);

    // Other edges.
    for (auto it = _points.begin(); it + 1 != _points.end(); ++it)
    {
      hypot = std::min(hypot, ggo::segment<data_t>(*it, *(it + 1)).hypot_to_point(x, y));
    }

    return std::sqrt(hypot);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  rect_data<data_t> polygon2d<data_t>::get_bounding_rect() const
  {
    if (_points.empty() == true)
    {
      GGO_FAIL();
    }

    auto it 	     = _points.begin();
    data_t left    = it->x();
    data_t right   = it->x();
    data_t bottom  = it->y();
    data_t top     = it->y();
    it++;

    while (it != _points.end())
    {
      left	  = std::min<data_t>(it->x(), left);
      right	  = std::max<data_t>(it->x(), right);
      bottom	= std::min<data_t>(it->y(), bottom);
      top		  = std::max<data_t>(it->y(), top);

      it++;
    }

    rect_data<data_t> rect_data;
    rect_data._pos = { left, bottom };
    rect_data._width = right - left;
    rect_data._height = top - bottom;

    return rect_data;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  bool polygon2d<data_t>::is_point_inside(data_t x, data_t y) const
  {
    if (_points.size() <= 2)
    {
      return false; 
    }

    data_t angle(0);
    data_t angle1 = atan2(_points.back().y() - y, _points.back().x() - x);
    for (const auto & point : _points)
    {
      data_t angle2 = atan2(point.y() - y, point.x() - x);
      data_t dangle = angle2 - angle1;
      if (dangle >  pi<data_t>()) { dangle -= 2 * pi<data_t>(); }
      if (dangle < -pi<data_t>()) { dangle += 2 * pi<data_t>(); }
      angle += dangle;
      angle1 = angle2;
    }

    return std::abs(angle) > pi<data_t>();
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  data_t polygon2d<data_t>::dist_to_segment(data_t x_from, data_t y_from, data_t x_to, data_t y_to) const
  {
    ggo::segment<data_t> segment(x_from, y_from, x_to, y_to);

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

    data_t hypot = ggo::segment<data_t>(_points[0], _points[_points.size() - 1]).hypot_to_segment(segment);
    if (hypot == 0)
    {
      return 0;
    }

    for (size_t i = 0; i < _points.size() - 1; ++i)
    {
      data_t hypot_cur = ggo::segment<data_t>(_points[i], _points[i + 1]).hypot_to_segment(segment);
      if (hypot_cur == 0)
      {
        return 0;
      }

      hypot = std::min(hypot, hypot_cur);
    }

    return std::sqrt(hypot);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  data_t polygon2d<data_t>::dist_to_segment(const ggo::pos2<data_t> & p1, const ggo::pos2<data_t> & p2) const
  {
    return dist_to_segment(p1.x(), p1.y(), p2.x(), p2.y());
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  data_t polygon2d<data_t>::dist_to_segment(const ggo::segment<data_t> & segment) const
  {
    return dist_to_segment(segment.from(), segment.to());
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  ggo::rect_intersection polygon2d<data_t>::get_rect_intersection(const rect_data<data_t> & rect_data) const
  {
    data_t left    = rect_data._pos.x();
    data_t bottom  = rect_data._pos.y();
    data_t right   = left + rect_data._width;
    data_t top     = bottom + rect_data._height;
    
    // Border intersection => partial overlap.
    for (size_t i = 0; i < _points.size(); ++i)
    {
      ggo::segment<data_t> segment(_points[i], _points[(i + 1) % _points.size()]);

      if (segment.intersect_horizontal_segment(left, right, bottom) == true ||
          segment.intersect_horizontal_segment(left, right, top   ) == true)
      {
        return ggo::rect_intersection::partial_overlap;
      }

      if (segment.intersect_vertical_segment(left,  bottom, top) == true ||
          segment.intersect_vertical_segment(right, bottom, top) == true)
      {
        return ggo::rect_intersection::partial_overlap;
      }
    }

    // If there is no border intersection, there are 3 cases left:
    // - polygon in rect
    // - rect in polygon
    // - disjoint

    ggo::rect<data_t> rect(rect_data);
    
    // Check if the polygon is inside the rect. Since there is no border
    // intersection, we just need to check if a point of the polygon 
    // is inside the rect.
    
    if (rect.is_point_inside(_points[0]) == true)
    {
      return ggo::rect_intersection::shape_in_rect;
    }

    // Check if the rect in inside the polygon. Since there is no border
    // intersection, we just need to check that a point of the rectangle 
    // is inside the polygon.
    if (is_point_inside(left, bottom) == true)
    {
      return ggo::rect_intersection::rect_in_shape;
    }

    return ggo::rect_intersection::disjoints;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  void polygon2d<data_t>::move(data_t dx, data_t dy)
  {
    for (auto & point : _points)
    {
      point.x() += dx;
      point.y() += dy;
    }
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  void polygon2d<data_t>::rotate(data_t angle, const ggo::pos2<data_t> & center)
  {
    for (auto & point : _points)
    {
      point = ggo::rotate(point, center, angle);
    }
  }
}
