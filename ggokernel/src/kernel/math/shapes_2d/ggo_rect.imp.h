namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  rect<data_t>::rect()
  {
    _rect_data._pos = { 0, 0 };
    _rect_data._width = 0;
    _rect_data._height = 0;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  rect<data_t>::rect(const rect<data_t> & rect)
  :
  _rect_data(rect._rect_data)
  {
    
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  rect<data_t>::rect(const pos2<data_t> & p1, const pos2<data_t> & p2)
  {
    auto horz = std::minmax(p1.x(), p2.x());
    auto vert = std::minmax(p1.y(), p2.y());

    _rect_data._pos = { horz.first, vert.first };
    _rect_data._width = horz.second - horz.first;
    _rect_data._height = vert.second - vert.first;
    GGO_ASSERT(_rect_data._width >= 0);
    GGO_ASSERT(_rect_data._height >= 0);
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  rect<data_t>::rect(data_t left, data_t bottom, data_t width, data_t height)
  {
    _rect_data._pos = { left, bottom };
    _rect_data._width = width;
    _rect_data._height = height;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  rect<data_t>::rect(const rect_data<data_t> & rect_data)
  :
  _rect_data(rect_data)
  {
    
  }
  
  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  bool rect<data_t>::is_point_inside(const ggo::pos2<data_t> & p) const
  {
    return (p.x() >= left() && p.x() <= right() && p.y() >= bottom() && p.y() <= top());
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  rect<data_t> rect<data_t>::from_left_right_bottom_top(data_t left, data_t right, data_t bottom, data_t top)
  {
    return rect(ggo::rect_data_from_left_right_bottom_top(left, right, bottom, top));
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  rect<data_t> rect<data_t>::from_union(const rect<data_t> & rect1, const rect<data_t> & rect2)
  {
    return rect(ggo::rect_data_union(rect1.data(), rect2.data()));
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  data_t rect<data_t>::dist_to_point(const ggo::pos2<data_t> & p) const
  {
    if (is_point_inside(p) == true)
    {
      return 0;
    }

    ggo::pos2<data_t> p1(left(),  bottom());
    ggo::pos2<data_t> p2(left(),  top());
    ggo::pos2<data_t> p3(right(), top());
    ggo::pos2<data_t> p4(right(), bottom());

    data_t d1 = ggo::segment<data_t>(p1, p2).dist_to_point(p);
    data_t d2 = ggo::segment<data_t>(p2, p3).dist_to_point(p);
    data_t d3 = ggo::segment<data_t>(p3, p4).dist_to_point(p);
    data_t d4 = ggo::segment<data_t>(p4, p1).dist_to_point(p);

    return std::min(std::min(d1, d2), std::min(d3, d4));
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  rect_intersection rect<data_t>::get_rect_intersection(const rect_data<data_t> & rect_data) const
  {
    data_t left    = rect_data._pos.x();
    data_t bottom  = rect_data._pos.y();
    data_t right   = left + rect_data._width;
    data_t top     = bottom + rect_data._height;

    GGO_ASSERT(left <= right);
    GGO_ASSERT(bottom <= top);

    // Rectangle in shape?
    if (left > this->left() && right < this->right() && bottom > this->bottom() && top < this->top())
    {
      return rect_intersection::rect_in_shape;
    }

    // Shape in rectangle?
    if (this->left() > left && this->right() < right && this->bottom() > bottom && this->top() < top)
    {
      return rect_intersection::shape_in_rect;
    }

    // Disjoint?
    if (right < this->left() || left > this->right() || top < this->bottom() || bottom > this->top())
    {
      return rect_intersection::disjoints;
    }

    // One case left: partial overlap.
    return rect_intersection::partial_overlap;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  void rect<data_t>::inflate(data_t extent)
  {
    _rect_data._pos.x() -= extent;
    _rect_data._pos.y() -= extent;
    _rect_data._width += 2 * extent;
    _rect_data._height += 2 * extent;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  void rect<data_t>::inflate(data_t horz_extent, data_t vert_extent)
  {
    _rect_data._pos.x() -= horz_extent;
    _rect_data._pos.y() -= vert_extent;
    _rect_data._width  += 2 * horz_extent;
    _rect_data._height += 2 * vert_extent;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  void rect<data_t>::extend(const ggo::pos2f & p)
  {
    data_t left   = _rect_data._pos.x();
    data_t bottom = _rect_data._pos.y();
    data_t right  = left   + _rect_data._width;
    data_t top    = bottom + _rect_data._height;

    left    = std::min(left,   p.x());
    bottom  = std::min(bottom, p.y());
    right   = std::max(right,  p.x());
    top     = std::max(top,    p.y());

    _rect_data._pos.x() = left;
    _rect_data._pos.y() = bottom;
    _rect_data._width = right - left;
    _rect_data._height = top - bottom;
  }
}
