namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  extended_segment<data_t>::extended_segment(const ggo::pos2<data_t> & p1, const ggo::pos2<data_t> & p2, data_t width)
    :
    _p1(p1), _p2(p2), _width(width)
  {
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  bool extended_segment<data_t>::is_point_inside(data_t x, data_t y) const
  {
    return ggo::segment<data_t>(_p1, _p2).dist_to_point(x, y) < _width;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  rect_data<data_t> extended_segment<data_t>::get_bounding_rect() const
  {
    data_t left    = std::min(_p1.template get<0>(), _p2.template get<0>()) - _width;
    data_t right   = std::max(_p1.template get<0>(), _p2.template get<0>()) + _width;
    data_t bottom  = std::min(_p1.template get<1>(), _p2.template get<1>()) - _width;
    data_t top     = std::max(_p1.template get<1>(), _p2.template get<1>()) + _width;

    return { {left, bottom}, right - left, top - bottom };
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  rect_intersection extended_segment<data_t>::get_rect_intersection(const rect_data<data_t> & rect_data) const
  {
    data_t left    = rect_data._pos.template get<0>();
    data_t bottom  = rect_data._pos.template get<1>();
    data_t right   = left + rect_data._width;
    data_t top     = bottom + rect_data._height;

    ggo::segment<data_t> s1(left,  bottom, left,  top);    // Left rectangle edge.
    ggo::segment<data_t> s2(right, bottom, right, top);    // Right rectangle edge.
    ggo::segment<data_t> s3(left,  bottom, right, bottom); // Bottom rectangle edge.
    ggo::segment<data_t> s4(left,  top,    right, top);    // data_top rectangle edge.
    ggo::segment<data_t> s(_p1, _p2);

    // Rectangle in shape.
    if (is_point_inside(left,  bottom) == true &&
        is_point_inside(right, bottom) == true &&
        is_point_inside(left,  top)    == true &&
        is_point_inside(right, top)    == true)
    {
      return rect_intersection::RECT_IN_SHAPE;
    }

    data_t squared_width = _width * _width;

    // Shape in rectangle?
    ggo::rect<data_t> rect(rect_data);
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
  template <typename data_t>
  data_t extended_segment<data_t>::dist_to_point(data_t x, data_t y) const 
  {
    data_t hypot = ggo::segment<data_t>(_p1, _p2).hypot_to_point(x, y);

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
