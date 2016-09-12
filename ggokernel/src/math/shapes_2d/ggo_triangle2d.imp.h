namespace ggo
{
  //////////////////////////////////////////////////////////////////
  template <typename data_t>
  void triangle2d<data_t>::move(data_t dx, data_t dy)
  {
    _v1.template get<0>() += dx;
    _v1.template get<1>() += dy;
    _v2.template get<0>() += dx;
    _v2.template get<1>() += dy;
    _v3.template get<0>() += dx;
    _v3.template get<1>() += dy;
  }
  
  //////////////////////////////////////////////////////////////////
  template <typename data_t>
  bool triangle2d<data_t>::is_point_inside(data_t x, data_t y) const
  {
    ggo::pos2<data_t> p(x, y);
    ggo::vec2<data_t> d(p - _v1);
    ggo::vec2<data_t> d2(_v2 - _v1);
    ggo::vec2<data_t> d3(_v3 - _v1);
    
    data_t m[2][2] = {{d2.template get<0>(), d3.template get<0>()},
                 {d2.template get<1>(), d3.template get<1>()}};
    data_t c[2] = {d.template get<0>(), d.template get<1>()};
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
    data_t left    = ggo::min(_v1.template get<0>(), _v2.template get<0>(), _v3.template get<0>());
    data_t right   = ggo::max(_v1.template get<0>(), _v2.template get<0>(), _v3.template get<0>());
    data_t bottom  = ggo::min(_v1.template get<1>(), _v2.template get<1>(), _v3.template get<1>());
    data_t top     = ggo::max(_v1.template get<1>(), _v2.template get<1>(), _v3.template get<1>());
    
    return { {left, bottom}, right - left, top - bottom };
  }
    
  //////////////////////////////////////////////////////////////////
  template <typename data_t>
  rect_intersection triangle2d<data_t>::get_rect_intersection(const rect_data<data_t> & rect_data) const
  {
    data_t left    = rect_data._pos.template get<0>();
    data_t bottom  = rect_data._pos.template get<1>();
    data_t right   = left + rect_data._width;
    data_t top     = bottom + rect_data._height;
    
    if ((_v1.template get<0>() >= left && _v1.template get<0>() <= right && _v1.template get<1>() >= bottom && _v1.template get<1>() <= top) &&
        (_v2.template get<0>() >= left && _v2.template get<0>() <= right && _v2.template get<1>() >= bottom && _v2.template get<1>() <= top) &&
        (_v3.template get<0>() >= left && _v3.template get<0>() <= right && _v3.template get<1>() >= bottom && _v3.template get<1>() <= top))
    {
      return rect_intersection::shape_in_rect;
    }
    
    if (is_point_inside(left, top) && 
        is_point_inside(right, top) &&
        is_point_inside(right, bottom) &&
        is_point_inside(left, bottom))
    {
      return rect_intersection::rect_in_shape;
    }
    
    if (right  < ggo::min(_v1.template get<0>(), _v2.template get<0>(), _v3.template get<0>()) ||
        left   > ggo::max(_v1.template get<0>(), _v2.template get<0>(), _v3.template get<0>()) ||
        top    < ggo::min(_v1.template get<1>(), _v2.template get<1>(), _v3.template get<1>()) ||
        bottom > ggo::max(_v1.template get<1>(), _v2.template get<1>(), _v3.template get<1>()))
    {
      return rect_intersection::disjoints;
    }
    
    segment<data_t> s12(_v1, _v2);
    if (s12.same_side(_v3.template get<0>(), _v3.template get<1>(), left,  top   ) == false &&
        s12.same_side(_v3.template get<0>(), _v3.template get<1>(), right, top   ) == false &&
        s12.same_side(_v3.template get<0>(), _v3.template get<1>(), right, bottom) == false &&
        s12.same_side(_v3.template get<0>(), _v3.template get<1>(), left,  bottom) == false)
    {
      return rect_intersection::disjoints;
    }
    
    segment<data_t> s23(_v2, _v3);
    if (s23.same_side(_v1.template get<0>(), _v1.template get<1>(), left,  top   ) == false &&
        s23.same_side(_v1.template get<0>(), _v1.template get<1>(), right, top   ) == false &&
        s23.same_side(_v1.template get<0>(), _v1.template get<1>(), right, bottom) == false &&
        s23.same_side(_v1.template get<0>(), _v1.template get<1>(), left,  bottom) == false)
    {
      return rect_intersection::disjoints;
    }
    
        segment<data_t> s13(_v1, _v3);
    if (s13.same_side(_v2.template get<0>(), _v2.template get<1>(), left,  top   ) == false &&
        s13.same_side(_v2.template get<0>(), _v2.template get<1>(), right, top   ) == false &&
        s13.same_side(_v2.template get<0>(), _v2.template get<1>(), right, bottom) == false &&
        s13.same_side(_v2.template get<0>(), _v2.template get<1>(), left,  bottom) == false)
    {
      return rect_intersection::disjoints;
    }
    
    return rect_intersection::partial_overlap;
  }

  //////////////////////////////////////////////////////////////////
  template <typename data_t>
  data_t triangle2d<data_t>::area() const
  {
    return std::abs((_v2.get<0>() - _v1.get<0>()) * (_v3.get<1>() - _v1.get<1>()) -
                    (_v3.get<0>() - _v1.get<0>()) * (_v2.get<1>() - _v1.get<1>())) / 2;
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
