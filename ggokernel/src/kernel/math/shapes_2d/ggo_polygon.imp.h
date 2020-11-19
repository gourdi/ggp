namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename scalar_t>
  polygon2d<scalar_t> polygon2d<scalar_t>::create_oriented_box(const ggo::pos2<scalar_t> & center, const ggo::vec2<scalar_t> & direction, scalar_t size1, scalar_t size2)
  {
    // Normalize the direction.
    scalar_t len = direction.get_length();
    if (len <= 0)
    {
      throw std::runtime_error("division by zero");
    }

    ggo::vec2<scalar_t> norm(direction / len);

    scalar_t x1 = center.x() + size1 * norm.x() + size2 * norm.y();
    scalar_t y1 = center.y() + size1 * norm.y() - size2 * norm.x();
                                                        
    scalar_t x2 = center.x() + size1 * norm.x() - size2 * norm.y();
    scalar_t y2 = center.y() + size1 * norm.y() + size2 * norm.x();
                                                        
    scalar_t x3 = center.x() - size1 * norm.x() - size2 * norm.y();
    scalar_t y3 = center.y() - size1 * norm.y() + size2 * norm.x();
                                                        
    scalar_t x4 = center.x() - size1 * norm.x() + size2 * norm.y();
    scalar_t y4 = center.y() - size1 * norm.y() - size2 * norm.x();

    return polygon2d<scalar_t>({ {x1, y1}, {x2, y2}, {x3, y3}, {x4, y4} });
  }

  //////////////////////////////////////////////////////////////
  template <typename scalar_t>
  polygon2d<scalar_t> polygon2d<scalar_t>::create_axis_aligned_box(scalar_t left, scalar_t right, scalar_t top, scalar_t bottom)
  {
    if (left > right) { std::swap(left, right); }
    if (bottom > top) { std::swap(top, bottom); }

    return polygon2d<scalar_t>({ {left, top}, {right, top}, {right, bottom}, {left, bottom} });
  }

  //////////////////////////////////////////////////////////////
  template <typename scalar_t>
  scalar_t polygon2d<scalar_t>::dist_to_point(const ggo::pos2<scalar_t> & p) const
  {
    switch (_points.size())
    {
    case 0:
      GGO_FAIL();
      return -1;
    case 1:
      return ggo::distance(p, _points.front());
    }

    if (is_point_inside(p) == true)
    {
      return 0;	
    }

    // First point to last point edge.
    scalar_t hypot = ggo::segment<scalar_t>(_points.front(), _points.back()).hypot_to_point(p);

    // Other edges.
    for (auto it = _points.begin(); it + 1 != _points.end(); ++it)
    {
      hypot = std::min(hypot, ggo::segment<scalar_t>(*it, *(it + 1)).hypot_to_point(p));
    }

    return std::sqrt(hypot);
  }

  //////////////////////////////////////////////////////////////
  template <typename scalar_t>
  rect_data<scalar_t> polygon2d<scalar_t>::get_bounding_rect() const
  {
    if (_points.empty() == true)
    {
      GGO_FAIL();
    }

    auto it 	     = _points.begin();
    scalar_t left    = it->x();
    scalar_t right   = it->x();
    scalar_t bottom  = it->y();
    scalar_t top     = it->y();
    it++;

    while (it != _points.end())
    {
      left	  = std::min<scalar_t>(it->x(), left);
      right	  = std::max<scalar_t>(it->x(), right);
      bottom	= std::min<scalar_t>(it->y(), bottom);
      top		  = std::max<scalar_t>(it->y(), top);

      it++;
    }

    rect_data<scalar_t> rect_data;
    rect_data._pos = { left, bottom };
    rect_data._width = right - left;
    rect_data._height = top - bottom;

    return rect_data;
  }

  //////////////////////////////////////////////////////////////
  template <typename scalar_t>
  bool polygon2d<scalar_t>::is_point_inside(const ggo::pos2<scalar_t> & p) const
  {
    if (_points.size() <= 2)
    {
      return false; 
    }

    bool inside = false;
    for (size_t i = 0, j = _points.size() - 1; i < _points.size(); j = i++)
    {
      if ((_points[i].y() > p.y()) == (_points[j].y() > p.y()))
      {
        continue;
      }

      if (p.x() < (_points[j].x() - _points[i].x()) * (p.y() - _points[i].y()) / (_points[j].y() - _points[i].y()) + _points[i].x())
      {
        inside = !inside;
      }
    }
     
    return inside;

#if 0
    scalar_t angle(0);
    scalar_t angle1 = atan2(_points.back().y() - y, _points.back().x() - x);
    for (const auto & point : _points)
    {
      scalar_t angle2 = atan2(point.y() - y, point.x() - x);
      scalar_t dangle = angle2 - angle1;
      if (dangle >  pi<scalar_t>()) { dangle -= 2 * pi<scalar_t>(); }
      if (dangle < -pi<scalar_t>()) { dangle += 2 * pi<scalar_t>(); }
      angle += dangle;
      angle1 = angle2;
    }

    return std::abs(angle) > pi<scalar_t>();
#endif
  }

  //////////////////////////////////////////////////////////////
  template <typename scalar_t>
  scalar_t polygon2d<scalar_t>::dist_to_segment(const ggo::pos2<scalar_t> & p1, const ggo::pos2<scalar_t> & p2) const
  {
    return dist_to_segment({ p1, p2 });
  }

  //////////////////////////////////////////////////////////////
  template <typename scalar_t>
  scalar_t polygon2d<scalar_t>::dist_to_segment(const ggo::segment<scalar_t> & segment) const
  {
    switch (_points.size())
    {
    case 0:
      GGO_FAIL();
      return -1;
    case 1:
      return segment.dist_to_point(_points[0]);
    }

    if (is_point_inside(segment.p1()) == true || is_point_inside(segment.p2()) == true)
    {
      return 0;
    }

    scalar_t hypot = ggo::segment<scalar_t>(_points[0], _points[_points.size() - 1]).hypot_to_segment(segment);
    if (hypot == 0)
    {
      return 0;
    }

    for (size_t i = 0; i < _points.size() - 1; ++i)
    {
      scalar_t hypot_cur = ggo::segment<scalar_t>(_points[i], _points[i + 1]).hypot_to_segment(segment);
      if (hypot_cur == 0)
      {
        return 0;
      }

      hypot = std::min(hypot, hypot_cur);
    }

    return std::sqrt(hypot);
  }

  //////////////////////////////////////////////////////////////
  template <typename scalar_t>
  ggo::rect_intersection polygon2d<scalar_t>::get_rect_intersection(const rect_data<scalar_t> & rect_data) const
  {
    scalar_t left    = rect_data._pos.x();
    scalar_t bottom  = rect_data._pos.y();
    scalar_t right   = left + rect_data._width;
    scalar_t top     = bottom + rect_data._height;
    
    // Border intersection => partial overlap.
    for (size_t i = 0; i < _points.size(); ++i)
    {
      ggo::segment<scalar_t> segment(_points[i], _points[(i + 1) % _points.size()]);

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

    ggo::rect<scalar_t> rect(rect_data);
    
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
    if (is_point_inside({ left, bottom }) == true)
    {
      return ggo::rect_intersection::rect_in_shape;
    }

    return ggo::rect_intersection::disjoints;
  }

  //////////////////////////////////////////////////////////////
  template <typename scalar_t>
  void polygon2d<scalar_t>::move(const ggo::vec2<scalar_t> & m)
  {
    for (auto & point : _points)
    {
      point += m;
    }
  }

  //////////////////////////////////////////////////////////////
  template <typename scalar_t>
  void polygon2d<scalar_t>::rotate(scalar_t angle, const ggo::pos2<scalar_t> & center)
  {
    for (auto & point : _points)
    {
      point = ggo::rotate(point, center, angle);
    }
  }
}
