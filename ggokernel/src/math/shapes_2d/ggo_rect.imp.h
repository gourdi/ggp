namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  rect<data_t>::rect()
  {
    _rect_data._pos.set<0>(0);
    _rect_data._pos.set<1>(0);
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
    data_t left    = std::min(p1.template get<0>(), p2.template get<0>());
    data_t right   = std::max(p1.template get<0>(), p2.template get<0>());
    data_t bottom  = std::min(p1.template get<1>(), p2.template get<1>());
    data_t top     = std::max(p1.template get<1>(), p2.template get<1>());

    _rect_data._pos = { left, bottom };
    _rect_data._width = right - left;
    _rect_data._height = top - bottom;
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
  bool rect<data_t>::is_point_inside(data_t x, data_t y) const
  {
    return (x >= left() && x <= right() && y >= bottom() && y <= top());
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  rect<data_t> rect<data_t>::from_left_right_bottom_top(data_t left, data_t right, data_t bottom, data_t top)
  {
    GGO_ASSERT(left <= right);
    GGO_ASSERT(bottom <= top);

    return rect(left, bottom, right - left, top - bottom);
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  rect<data_t> rect<data_t>::from_union(const rect<data_t> & rect1, const rect<data_t> & rect2)
  {
    return from_left_right_bottom_top(std::min(rect1.left(), rect2.left()),
                                      std::max(rect1.right(), rect2.right()),
                                      std::min(rect1.bottom(), rect2.bottom()),
                                      std::max(rect1.top(), rect2.top()));
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  data_t rect<data_t>::dist_to_point(data_t x, data_t y) const
  {
    if (is_point_inside(x, y) == true)
    {
      return 0;
    }

    ggo::pos2<data_t> p1(left(),  bottom());
    ggo::pos2<data_t> p2(left(),  top());
    ggo::pos2<data_t> p3(right(), top());
    ggo::pos2<data_t> p4(right(), bottom());

    data_t d1 = ggo::segment<data_t>(p1, p2).dist_to_point(x, y);
    data_t d2 = ggo::segment<data_t>(p2, p3).dist_to_point(x, y);
    data_t d3 = ggo::segment<data_t>(p3, p4).dist_to_point(x, y);
    data_t d4 = ggo::segment<data_t>(p4, p1).dist_to_point(x, y);

    return std::min(std::min(d1, d2), std::min(d3, d4));
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  rect_intersection rect<data_t>::get_rect_intersection(const rect_data<data_t> & rect_data) const
  {
    data_t left    = rect_data._pos.template get<0>();
    data_t bottom  = rect_data._pos.template get<1>();
    data_t right   = left + rect_data._width;
    data_t top     = bottom + rect_data._height;

    GGO_ASSERT(left <= right);
    GGO_ASSERT(bottom <= top);

    // Rectangle in shape?
    if (left > this->left() && right < this->right() && bottom > this->bottom() && top < this->top())
    {
      return rect_intersection::RECT_IN_SHAPE;
    }

    // Shape in rectangle?
    if (this->left() > left && this->right() < right && this->bottom() > bottom && this->top() < top)
    {
      return rect_intersection::SHAPE_IN_RECT;
    }

    // Disjoint?
    if (right < this->left() || left > this->right() || top < this->bottom() || bottom > this->top())
    {
      return rect_intersection::DISJOINTS;
    }

    // One case left: partial overlap.
    return rect_intersection::PARTIAL_OVERLAP;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  void rect<data_t>::inflate(data_t extent)
  {
    _rect_data._pos.template get<0>() -= extent;
    _rect_data._pos.template get<1>() -= extent;
    _rect_data._width += 2 * extent;
    _rect_data._height += 2 * extent;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  void rect<data_t>::inflate(data_t horz_extent, data_t vert_extent)
  {
    _rect_data._pos.template get<0>() -= horz_extent;
    _rect_data._pos.template get<1>() -= vert_extent;
    _rect_data._width  += 2 * horz_extent;
    _rect_data._height += 2 * vert_extent;
  }
}
