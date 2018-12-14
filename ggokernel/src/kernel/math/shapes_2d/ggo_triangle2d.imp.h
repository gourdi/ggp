namespace ggo
{
  //////////////////////////////////////////////////////////////////
  template <typename data_t>
  void triangle2d<data_t>::move(const ggo::vec2<data_t> & m)
  {
    _v1 += m;
    _v2 += m;
    _v3 += m;
  }
  
  //////////////////////////////////////////////////////////////////
  template <typename data_t>
  bool triangle2d<data_t>::is_point_inside(const ggo::pos2<data_t> & p) const
  {
    ggo::vec2<data_t> d(p - _v1);
    ggo::vec2<data_t> d2(_v2 - _v1);
    ggo::vec2<data_t> d3(_v3 - _v1);
    
    data_t m[2][2] = {
      { d2.x(), d3.x() },
      { d2.y(), d3.y() } };
    data_t c[2] = {d.x(), d.y()};
    data_t s[2] = {0, 0};
    
    if (ggo::linsolve2d(m, c, s) == false)
    {
      return false;
    }

    if (s[0] < 0 || s[1] < 0)
    {
      return false;
    }
    
    if (s[0] + s[1] > 1)
    {
      return false;
    }
      
    return true;
  }
  
  //////////////////////////////////////////////////////////////////
  template <typename data_t>
  rect_data<data_t> triangle2d<data_t>::get_bounding_rect() const
  {
    data_t left    = ggo::min(_v1.x(), _v2.x(), _v3.x());
    data_t right   = ggo::max(_v1.x(), _v2.x(), _v3.x());
    data_t bottom  = ggo::min(_v1.y(), _v2.y(), _v3.y());
    data_t top     = ggo::max(_v1.y(), _v2.y(), _v3.y());
    
    return { {left, bottom}, right - left, top - bottom };
  }
    
  //////////////////////////////////////////////////////////////////
  template <typename data_t>
  rect_intersection triangle2d<data_t>::get_rect_intersection(const rect_data<data_t> & rect_data) const
  {
    data_t left    = rect_data._pos.x();
    data_t bottom  = rect_data._pos.y();
    data_t right   = left + rect_data._width;
    data_t top     = bottom + rect_data._height;
    
    if ((_v1.x() >= left && _v1.x() <= right && _v1.y() >= bottom && _v1.y() <= top) &&
        (_v2.x() >= left && _v2.x() <= right && _v2.y() >= bottom && _v2.y() <= top) &&
        (_v3.x() >= left && _v3.x() <= right && _v3.y() >= bottom && _v3.y() <= top))
    {
      return rect_intersection::shape_in_rect;
    }
    
    if (is_point_inside({ left, top }) &&
        is_point_inside({ right, top }) &&
        is_point_inside({ right, bottom }) &&
        is_point_inside({ left, bottom }))
    {
      return rect_intersection::rect_in_shape;
    }
    
    if (right  < ggo::min(_v1.x(), _v2.x(), _v3.x()) ||
        left   > ggo::max(_v1.x(), _v2.x(), _v3.x()) ||
        top    < ggo::min(_v1.y(), _v2.y(), _v3.y()) ||
        bottom > ggo::max(_v1.y(), _v2.y(), _v3.y()))
    {
      return rect_intersection::disjoints;
    }
    
    segment<data_t> s12(_v1, _v2);
    if (s12.same_side(_v3.x(), _v3.y(), left,  top   ) == false &&
        s12.same_side(_v3.x(), _v3.y(), right, top   ) == false &&
        s12.same_side(_v3.x(), _v3.y(), right, bottom) == false &&
        s12.same_side(_v3.x(), _v3.y(), left,  bottom) == false)
    {
      return rect_intersection::disjoints;
    }
    
    segment<data_t> s23(_v2, _v3);
    if (s23.same_side(_v1.x(), _v1.y(), left,  top   ) == false &&
        s23.same_side(_v1.x(), _v1.y(), right, top   ) == false &&
        s23.same_side(_v1.x(), _v1.y(), right, bottom) == false &&
        s23.same_side(_v1.x(), _v1.y(), left,  bottom) == false)
    {
      return rect_intersection::disjoints;
    }
    
        segment<data_t> s13(_v1, _v3);
    if (s13.same_side(_v2.x(), _v2.y(), left,  top   ) == false &&
        s13.same_side(_v2.x(), _v2.y(), right, top   ) == false &&
        s13.same_side(_v2.x(), _v2.y(), right, bottom) == false &&
        s13.same_side(_v2.x(), _v2.y(), left,  bottom) == false)
    {
      return rect_intersection::disjoints;
    }
    
    return rect_intersection::partial_overlap;
  }

  //////////////////////////////////////////////////////////////////
  template <typename data_t>
  data_t triangle2d<data_t>::area() const
  {
    return std::abs((_v2.x() - _v1.x()) * (_v3.y() - _v1.y()) -
                    (_v3.x() - _v1.x()) * (_v2.y() - _v1.y())) / 2;
  }

  //////////////////////////////////////////////////////////////////
  template <typename data_t>
  data_t triangle2d<data_t>::angle1() const
  {
    return ggo::angle(_v2 - _v1, _v3 - _v1);
  }

  //////////////////////////////////////////////////////////////////
  template <typename data_t>
  data_t triangle2d<data_t>::angle2() const
  {
    return ggo::angle(_v1 - _v2, _v3 - _v2);
  }

  //////////////////////////////////////////////////////////////////
  template <typename data_t>
  data_t triangle2d<data_t>::angle3() const
  {
    return ggo::angle(_v1 - _v3, _v2 - _v3);
  }
}

namespace ggo
{
  template <typename data_t>
  triangle_intersection get_triangle_intersection(const triangle2d<data_t> & triangle1, const triangle2d<data_t> & triangle2)
  {
    if (triangle2.is_point_inside(triangle1.v1()) == true &&
        triangle2.is_point_inside(triangle1.v2()) == true &&
        triangle2.is_point_inside(triangle1.v3()) == true)
    {
      return triangle_intersection::triangle1_in_triangle2;
    }

    if (triangle1.is_point_inside(triangle2.v1()) == true &&
        triangle1.is_point_inside(triangle2.v2()) == true &&
        triangle1.is_point_inside(triangle2.v3()) == true)
    {
      return triangle_intersection::triangle2_in_triangle1;
    }

    auto check_disjoint = [](
      const ggo::pos2<data_t> & edge_v1,
      const ggo::pos2<data_t> & edge_v2,
      const ggo::pos2<data_t> & v,
      const triangle2d<data_t> & triangle)
    {
      ggo::vec2<data_t> diff = edge_v2 - edge_v1;
      data_t ortho_dot = ggo::ortho_dot(diff, v - edge_v1);
      return ((ortho_dot * ggo::ortho_dot(diff, triangle.v1() - edge_v1) <= 0) &&
              (ortho_dot * ggo::ortho_dot(diff, triangle.v2() - edge_v1) <= 0) &&
              (ortho_dot * ggo::ortho_dot(diff, triangle.v3() - edge_v1) <= 0));
    };

    if ((check_disjoint(triangle1.v1(), triangle1.v2(), triangle1.v3(), triangle2) == true) ||
        (check_disjoint(triangle1.v2(), triangle1.v3(), triangle1.v1(), triangle2) == true) ||
        (check_disjoint(triangle1.v3(), triangle1.v1(), triangle1.v2(), triangle2) == true) ||
        (check_disjoint(triangle2.v1(), triangle2.v2(), triangle2.v3(), triangle1) == true) ||
        (check_disjoint(triangle2.v2(), triangle2.v3(), triangle2.v1(), triangle1) == true) ||
        (check_disjoint(triangle2.v3(), triangle2.v1(), triangle2.v2(), triangle1) == true))
    {
      return triangle_intersection::disjoints;
    }

    return triangle_intersection::partial_overlap;
  }
}
