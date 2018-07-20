namespace ggo
{
  //////////////////////////////////////////////////////////////////
  template <typename data_t>
  ggo::pos2<data_t> oriented_box<data_t>::operator[](int i) const
  {
    switch (i)
    {
    case 0:
      return _pos + _size1 * _dir + _size2 * dir2();
    case 1:
      return _pos + _size1 * _dir - _size2 * dir2();
    case 2:
      return _pos - _size1 * _dir - _size2 * dir2();
    case 3:
      return _pos - _size1 * _dir + _size2 * dir2();
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
    _dir = ggo::rotate(_dir, angle);
  }
  
  //////////////////////////////////////////////////////////////////
  template <typename data_t>
  std::vector<ggo::pos2<data_t>> oriented_box<data_t>::get_points() const
  {
    return std::vector<ggo::pos2<data_t>> { _pos + _size1 * _dir + _size2 * dir2(),
                                            _pos + _size1 * _dir - _size2 * dir2(),
                                            _pos - _size1 * _dir - _size2 * dir2(),
                                            _pos - _size1 * _dir + _size2 * dir2() };
  }

  //////////////////////////////////////////////////////////////////
  template <typename data_t>
  bool oriented_box<data_t>::is_point_inside(const ggo::pos2<data_t> & p) const
  {
    ggo::pos2<data_t> diff = p - _pos;

    data_t dot1 = ggo::dot(diff, _dir);
    if (std::abs(dot1) > _size1)
    {
      return false;
    }

    data_t dot2 = ggo::dot(diff, dir2());
    if (std::abs(dot2) > _size2)
    {
      return false;
    }

    return true;
  }

  //////////////////////////////////////////////////////////////////
  template <typename data_t>
  rect_data<data_t> oriented_box<data_t>::get_bounding_rect() const
  {
    auto p1 = _pos + _size1 * _dir + _size2 * dir2();
    auto p2 = _pos + _size1 * _dir - _size2 * dir2();
    auto p3 = _pos - _size1 * _dir - _size2 * dir2();
    auto p4 = _pos - _size1 * _dir + _size2 * dir2();

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
    auto dir2 = this->dir2();
    auto offset1 = _size1 * _dir;
    auto offset2 = _size2 * dir2;

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

    auto dot1 = ggo::dot(_dir, rect_p1 - _pos);
    auto dot2 = ggo::dot(_dir, rect_p2 - _pos);
    auto dot3 = ggo::dot(_dir, rect_p3 - _pos);
    auto dot4 = ggo::dot(_dir, rect_p4 - _pos);
    if (dot1 > _size1 && dot2 > _size1 && dot3 > _size1 && dot4 > _size1)
    {
      return rect_intersection::disjoints;
    }
    if (dot1 < -_size1 && dot2 < -_size1 && dot3 < -_size1 && dot4 < -_size1)
    {
      return rect_intersection::disjoints;
    }

    dot1 = ggo::dot(dir2, rect_p1 - _pos);
    dot2 = ggo::dot(dir2, rect_p2 - _pos);
    dot3 = ggo::dot(dir2, rect_p3 - _pos);
    dot4 = ggo::dot(dir2, rect_p4 - _pos);
    if (dot1 > _size2 && dot2 > _size2 && dot3 > _size2 && dot4 > _size2)
    {
      return rect_intersection::disjoints;
    }
    if (dot1 < -_size2 && dot2 < -_size2 && dot3 < -_size2 && dot4 < -_size2)
    {
      return rect_intersection::disjoints;
    }

    return rect_intersection::partial_overlap;
  }
}
