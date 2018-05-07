namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  capsule<data_t>::capsule(const ggo::pos2<data_t> & p1, const ggo::pos2<data_t> & p2, data_t width)
    :
    _p1(p1), _p2(p2), _width(width)
  {
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  bool capsule<data_t>::is_point_inside(const ggo::pos2<data_t> & p) const
  {
    return ggo::segment<data_t>(_p1, _p2).dist_to_point(p) < _width;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  rect_data<data_t> capsule<data_t>::get_bounding_rect() const
  {
    data_t left    = std::min(_p1.x(), _p2.x()) - _width;
    data_t right   = std::max(_p1.x(), _p2.x()) + _width;
    data_t bottom  = std::min(_p1.y(), _p2.y()) - _width;
    data_t top     = std::max(_p1.y(), _p2.y()) + _width;

    return { {left, bottom}, right - left, top - bottom };
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  rect_intersection capsule<data_t>::get_rect_intersection(const rect_data<data_t> & rect_data) const
  {
    data_t left    = rect_data._pos.x();
    data_t bottom  = rect_data._pos.y();
    data_t right   = left + rect_data._width;
    data_t top     = bottom + rect_data._height;

    ggo::segment<data_t> s1({ left,  bottom }, { left, top });      // Left rectangle edge.
    ggo::segment<data_t> s2({ right, bottom }, { right, top });     // Right rectangle edge.
    ggo::segment<data_t> s3({ left,  bottom }, { right, bottom });  // Bottom rectangle edge.
    ggo::segment<data_t> s4({ left,  top }, { right, top });        // data_top rectangle edge.
    ggo::segment<data_t> s(_p1, _p2);

    // Rectangle in shape.
    if (is_point_inside({ left,  bottom }) == true &&
        is_point_inside({ right, bottom }) == true &&
        is_point_inside({ left,  top }) == true &&
        is_point_inside({ right, top }) == true)
    {
      return rect_intersection::rect_in_shape;
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
      return rect_intersection::shape_in_rect;
    }

    // Disjoints?
    if (s.hypot_to_segment(s1) > squared_width &&
        s.hypot_to_segment(s2) > squared_width &&
        s.hypot_to_segment(s3) > squared_width &&
        s.hypot_to_segment(s4) > squared_width)
    {
      return rect_intersection::disjoints;
    }

    return rect_intersection::partial_overlap;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  data_t capsule<data_t>::dist_to_point(const ggo::pos2<data_t> & p) const
  {
    data_t hypot = ggo::segment<data_t>(_p1, _p2).hypot_to_point(p);

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
