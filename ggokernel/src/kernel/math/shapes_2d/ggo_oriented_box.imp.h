namespace ggo
{
  //////////////////////////////////////////////////////////////////
  template <typename data_t>
  ggo::pos2<data_t> oriented_box<data_t>::operator[](int i) const
  {
    switch (i)
    {
    case 0:
      return _pos + _size_x * dir_x() + _size_y * dir_y();
    case 1:
      return _pos + _size_x * dir_x() - _size_y * dir_y();
    case 2:
      return _pos - _size_x * dir_x() - _size_y * dir_y();
    case 3:
      return _pos - _size_x * dir_x() + _size_y * dir_y();
    default:
      GGO_FAIL();
      return ggo::pos2<data_t>(0.f, 0.f);
    }
  }

  //////////////////////////////////////////////////////////////////
  template <typename data_t>
  void oriented_box<data_t>::rotate(data_t angle, const ggo::pos2<data_t> & center)
  {
    _pos = ggo::rotate(_pos, center, angle);
    _angle += angle;
  }
  
  //////////////////////////////////////////////////////////////////
  template <typename data_t>
  std::array<ggo::pos2<data_t>, 4> oriented_box<data_t>::get_points() const
  {
    const data_t cos = std::cos(_angle);
    const data_t sin = std::sin(_angle);

    const ggo::vec2<data_t> x = { cos, sin };
    const ggo::vec2<data_t> y = { -sin, cos };

    return { _pos + _half_size_x * x + _half_size_y * y,
             _pos + _half_size_x * x - _half_size_y * y,
             _pos - _half_size_x * x - _half_size_y * y,
             _pos - _half_size_x * x + _half_size_y * y };
  }

  //////////////////////////////////////////////////////////////////
  template <typename data_t>
  std::array<typename oriented_box<data_t>::vertex, 4> oriented_box<data_t>::get_vertices() const
  {
    const data_t cos = std::cos(_angle);
    const data_t sin = std::sin(_angle);

    const ggo::vec2<data_t> x = {  _half_size_x * cos, _half_size_x * sin };
    const ggo::vec2<data_t> y = { -_half_size_y * sin, _half_size_y * cos };

    return { { { _pos + x + y, vertex_id::right_top },
               { _pos + x - y, vertex_id::right_bottom },
               { _pos - x + y, vertex_id::left_top },
               { _pos - x - y, vertex_id::left_bottom } } };
  }

  //////////////////////////////////////////////////////////////////
  template <typename data_t>
  std::array<typename oriented_box<data_t>::edge, 4> oriented_box<data_t>::get_edges() const
  {
    const data_t cos = std::cos(_angle);
    const data_t sin = std::sin(_angle);

    const ggo::vec2<data_t> x_norm = {  cos, sin };
    const ggo::vec2<data_t> y_norm = { -sin, cos };

    const ggo::vec2<data_t> x = _half_size_x * x_norm;
    const ggo::vec2<data_t> y = _half_size_y * y_norm;

    const vertex right_top    = { _pos + x + y, oriented_box_vertex_id::right_top };
    const vertex right_bottom = { _pos + x - y, oriented_box_vertex_id::right_bottom };
    const vertex left_top     = { _pos - x + y, oriented_box_vertex_id::left_top };
    const vertex left_bottom  = { _pos - x - y, oriented_box_vertex_id::left_bottom };

    return { { { left_bottom,  left_top,     -x_norm, oriented_box_edge_id::left },
               { right_bottom, right_top,     x_norm, oriented_box_edge_id::right },
               { left_bottom,  right_bottom, -y_norm, oriented_box_edge_id::bottom },
               { left_top,     right_top,     y_norm, oriented_box_edge_id::top } } };
  }

  //////////////////////////////////////////////////////////////////
  template <typename data_t>
  bool oriented_box<data_t>::is_point_inside(const ggo::pos2<data_t> & p) const
  {
    const data_t cos = std::cos(_angle);
    const data_t sin = std::sin(_angle);

    const ggo::vec2<data_t> x{ cos, sin };
    const ggo::vec2<data_t> y{ -sin, cos };

    const ggo::vec2<data_t> diff = p - _pos;

    data_t dot1 = ggo::dot(diff, x);
    if (std::abs(dot1) > _half_size_x)
    {
      return false;
    }

    data_t dot2 = ggo::dot(diff, y);
    if (std::abs(dot2) > _half_size_y)
    {
      return false;
    }

    return true;
  }

  //////////////////////////////////////////////////////////////////
  template <typename data_t>
  rect_data<data_t> oriented_box<data_t>::get_bounding_rect() const
  {
    const data_t cos = std::cos(_angle);
    const data_t sin = std::sin(_angle);

    const ggo::vec2<data_t> x{ cos, sin };
    const ggo::vec2<data_t> y{ -sin, cos };

    auto p1 = _pos + _half_size_x * x + _half_size_y * y;
    auto p2 = _pos + _half_size_x * x - _half_size_y * y;
    auto p3 = _pos - _half_size_x * x - _half_size_y * y;
    auto p4 = _pos - _half_size_x * x + _half_size_y * y;

    auto x_inf = ggo::min(p1.x(), p2.x(), p3.x(), p4.x());
    auto x_sup = ggo::max(p1.x(), p2.x(), p3.x(), p4.x());
    auto y_inf = ggo::min(p1.y(), p2.y(), p3.y(), p4.y());
    auto y_sup = ggo::max(p1.y(), p2.y(), p3.y(), p4.y());

    return rect_data_from_left_right_bottom_top(x_inf, x_sup, y_inf, y_sup);
  }

  //////////////////////////////////////////////////////////////////
  template <typename data_t>
  rect_intersection oriented_box<data_t>::get_rect_intersection(const rect_data<data_t> & rect_data) const
  {
    const data_t cos = std::cos(_angle);
    const data_t sin = std::sin(_angle);

    const ggo::vec2<data_t> x{ cos, sin };
    const ggo::vec2<data_t> y{ -sin, cos };

    auto offset1 = _half_size_x * x;
    auto offset2 = _half_size_y * y;

    // Shape in rect?
    auto obb_p1 = _pos + offset1 + offset2;
    auto obb_p2 = _pos + offset1 - offset2;
    auto obb_p3 = _pos - offset1 - offset2;
    auto obb_p4 = _pos - offset1 + offset2;

    rect<data_t> rect(rect_data);
    if (rect.is_point_inside(obb_p1) == true &&
        rect.is_point_inside(obb_p2) == true &&
        rect.is_point_inside(obb_p3) == true &&
        rect.is_point_inside(obb_p4) == true)
    {
      return rect_intersection::shape_in_rect;
    }

    // Rect in shape?
    ggo::pos2<data_t> rect_p1(rect.left(), rect.bottom());
    ggo::pos2<data_t> rect_p2(rect.left(), rect.top());
    ggo::pos2<data_t> rect_p3(rect.right(), rect.bottom());
    ggo::pos2<data_t> rect_p4(rect.right(), rect.top());

    if (is_point_inside(rect_p1) == true &&
        is_point_inside(rect_p2) == true &&
        is_point_inside(rect_p3) == true &&
        is_point_inside(rect_p4) == true)
    {
      return rect_intersection::rect_in_shape;
    }

    // Disjoint?
    auto x_inf = ggo::min(obb_p1.x(), obb_p2.x(), obb_p3.x(), obb_p4.x());
    if (x_inf > rect.right())
    {
      return rect_intersection::disjoints;
    }
    auto x_sup = ggo::max(obb_p1.x(), obb_p2.x(), obb_p3.x(), obb_p4.x());
    if (x_sup < rect.left())
    {
      return rect_intersection::disjoints;
    }
    auto y_inf = ggo::min(obb_p1.y(), obb_p2.y(), obb_p3.y(), obb_p4.y());
    if (y_inf > rect.top())
    {
      return rect_intersection::disjoints;
    }
    auto y_sup = ggo::max(obb_p1.y(), obb_p2.y(), obb_p3.y(), obb_p4.y());
    if (y_sup < rect.bottom())
    {
      return rect_intersection::disjoints;
    }

    auto dot1 = ggo::dot(x, rect_p1 - _pos);
    auto dot2 = ggo::dot(x, rect_p2 - _pos);
    auto dot3 = ggo::dot(x, rect_p3 - _pos);
    auto dot4 = ggo::dot(x, rect_p4 - _pos);
    if (dot1 > _half_size_x && dot2 > _half_size_x && dot3 > _half_size_x && dot4 > _half_size_x)
    {
      return rect_intersection::disjoints;
    }
    if (dot1 < -_half_size_x && dot2 < -_half_size_x && dot3 < -_half_size_x && dot4 < -_half_size_x)
    {
      return rect_intersection::disjoints;
    }

    dot1 = ggo::dot(y, rect_p1 - _pos);
    dot2 = ggo::dot(y, rect_p2 - _pos);
    dot3 = ggo::dot(y, rect_p3 - _pos);
    dot4 = ggo::dot(y, rect_p4 - _pos);
    if (dot1 > _half_size_y && dot2 > _half_size_y && dot3 > _half_size_y && dot4 > _half_size_y)
    {
      return rect_intersection::disjoints;
    }
    if (dot1 < -_half_size_y && dot2 < -_half_size_y && dot3 < -_half_size_y && dot4 < -_half_size_y)
    {
      return rect_intersection::disjoints;
    }

    return rect_intersection::partial_overlap;
  }

  //////////////////////////////////////////////////////////////////
  template <typename data_t>
  bool test_intersection(const ggo::oriented_box<data_t> & box1, const ggo::oriented_box<data_t> & box2)
  {
    // Separating axis: box1.x
    {
      data_t proj1 = ggo::dot(box1.pos(), box1.dir_x());
      auto proj2 = project(box2, box1.dir_x());
      if (proj2._inf > proj1 + box1.half_size_x() || proj2._sup < proj1 - box1.half_size_x())
      {
        return false;
      }
    }

    // Separating axis: box1.y
    {
      data_t proj1 = ggo::dot(box1.pos(), box1.dir_y());
      auto proj2 = project(box2, box1.dir_y());
      if (proj2._inf > proj1 + box1.half_size_y() || proj2._sup < proj1 - box1.half_size_y())
      {
        return false;
      }
    }

    // Separating axis: box2.x
    {
      data_t proj2 = ggo::dot(box2.pos(), box2.dir_x());
      auto proj1 = project(box1, box2.dir_x());
      if (proj1._inf > proj2 + box2.half_size_x() || proj1._sup < proj2 - box2.half_size_x())
      {
        return false;
      }
    }

    // Separating axis: box2.y
    {
      data_t proj2 = ggo::dot(box2.pos(), box2.dir_y());
      auto proj1 = project(box1, box2.dir_y());
      if (proj1._inf > proj2 + box2.half_size_y() || proj1._sup < proj2 - box2.half_size_y())
      {
        return false;
      }
    }

    return true;
  }
}
