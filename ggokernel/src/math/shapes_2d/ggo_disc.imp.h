namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  data_t disc<data_t>::dist_to_point(data_t x, data_t y) const
  {
    data_t dx	        = _center.template get<0>() - x;
    data_t dy	        = _center.template get<1>() - y;
    data_t hypot	      = dx * dx + dy * dy;
    data_t sqrd_radius = _radius * _radius;

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
  template <typename data_t>
  rect_data<data_t> disc<data_t>::get_bounding_rect() const
  {
    data_t diameter = 2 * _radius;

    return { { _center.template get<0>() - _radius, _center.template get<1>() - _radius }, diameter, diameter };
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  bool disc<data_t>::is_point_inside(data_t x, data_t y) const
  {
    data_t dx 	  = _center.template get<0>() - x;
    data_t dy 	  = _center.template get<1>() - y;
    data_t hypot	= dx * dx + dy * dy;

    return hypot <= _radius * _radius;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  rect_intersection disc<data_t>::get_rect_intersection(const rect_data<data_t> & rect_data) const
  {
    data_t left    = rect_data._pos.template get<0>();
    data_t bottom  = rect_data._pos.template get<1>();
    data_t right   = left + rect_data._width;
    data_t top     = bottom + rect_data._height;

    // Rectangle in circle?
    ggo::pos2<data_t> p1(left,  bottom);
    ggo::pos2<data_t> p2(left,  top);
    ggo::pos2<data_t> p3(right, bottom);
    ggo::pos2<data_t> p4(right, top);

    data_t hypot = _radius * _radius;
    data_t hypot1 = ggo::hypot(_center, p1);
    data_t hypot2 = ggo::hypot(_center, p2);
    data_t hypot3 = ggo::hypot(_center, p3);
    data_t hypot4 = ggo::hypot(_center, p4);

    if (hypot1 < hypot && hypot2 < hypot && hypot3 < hypot && hypot4 < hypot)
    {
      return rect_intersection::RECT_IN_SHAPE;
    }

    // Circle in rectangle?
    if (_center.template get<0>() - left   > _radius &&
        _center.template get<1>() - bottom > _radius &&
        right - _center.template get<0>()  > _radius &&
        top - _center.template get<1>()    > _radius)
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
  template <typename data_t>
  bool disc<data_t>::segment_intersect_border(data_t x_from, data_t y_from, data_t x_to, data_t y_to) const
  {
    return ggo::circle<data_t>(_center, _radius).intersect_segment(x_from, y_from, x_to, y_to);
  }
}