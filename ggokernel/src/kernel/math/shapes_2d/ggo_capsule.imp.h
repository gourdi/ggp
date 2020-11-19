namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename scalar_t>
  capsule<scalar_t>::capsule(const ggo::pos2<scalar_t> & p1, const ggo::pos2<scalar_t> & p2, scalar_t width)
    :
    _p1(p1), _p2(p2), _width(width)
  {
  }

  //////////////////////////////////////////////////////////////
  template <typename scalar_t>
  bool capsule<scalar_t>::is_point_inside(const ggo::pos2<scalar_t> & p) const
  {
    return ggo::segment<scalar_t>(_p1, _p2).dist_to_point(p) < _width;
  }

  //////////////////////////////////////////////////////////////
  template <typename scalar_t>
  rect_data<scalar_t> capsule<scalar_t>::get_bounding_rect() const
  {
    scalar_t left    = std::min(_p1.x(), _p2.x()) - _width;
    scalar_t right   = std::max(_p1.x(), _p2.x()) + _width;
    scalar_t bottom  = std::min(_p1.y(), _p2.y()) - _width;
    scalar_t top     = std::max(_p1.y(), _p2.y()) + _width;

    return { {left, bottom}, right - left, top - bottom };
  }

  //////////////////////////////////////////////////////////////
  template <typename scalar_t>
  rect_intersection capsule<scalar_t>::get_rect_intersection(const rect_data<scalar_t> & rect_data) const
  {
    scalar_t left    = rect_data._pos.x();
    scalar_t bottom  = rect_data._pos.y();
    scalar_t right   = left + rect_data._width;
    scalar_t top     = bottom + rect_data._height;

    ggo::segment<scalar_t> s1({ left,  bottom }, { left, top });      // Left rectangle edge.
    ggo::segment<scalar_t> s2({ right, bottom }, { right, top });     // Right rectangle edge.
    ggo::segment<scalar_t> s3({ left,  bottom }, { right, bottom });  // Bottom rectangle edge.
    ggo::segment<scalar_t> s4({ left,  top }, { right, top });        // scalar_top rectangle edge.
    ggo::segment<scalar_t> s(_p1, _p2);

    // Rectangle in shape.
    if (is_point_inside({ left,  bottom }) == true &&
        is_point_inside({ right, bottom }) == true &&
        is_point_inside({ left,  top }) == true &&
        is_point_inside({ right, top }) == true)
    {
      return rect_intersection::rect_in_shape;
    }

    scalar_t squared_width = _width * _width;

    // Shape in rectangle?
    ggo::rect<scalar_t> rect(rect_data);
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
  template <typename scalar_t>
  scalar_t capsule<scalar_t>::dist_to_point(const ggo::pos2<scalar_t> & p) const
  {
    scalar_t hypot = ggo::segment<scalar_t>(_p1, _p2).hypot_to_point(p);

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
