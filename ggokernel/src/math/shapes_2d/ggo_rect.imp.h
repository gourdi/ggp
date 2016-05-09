namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename T>
  rect<T>::rect(const rect<T> & rect)
  :
  _rect_data(rect._rect_data)
  {
    
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  rect<T>::rect(const set2<T> & p1, const set2<T> & p2)
  {
    T left    = std::min(p1.x(), p2.x());
    T right   = std::max(p1.x(), p2.x());
    T bottom  = std::min(p1.y(), p2.y());
    T top     = std::max(p1.y(), p2.y());

    _rect_data._pos = { left, bottom };
    _rect_data._width = right - left;
    _rect_data._height = top - bottom;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  rect<T>::rect(T left, T bottom, T width, T height)
  {
    _rect_data._pos = { left, bottom };
    _rect_data._width = width;
    _rect_data._height = height;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  rect<T>::rect(const rect_data<T> & rect_data)
  :
  _rect_data(rect_data)
  {
    
  }
  
  /////////////////////////////////////////////////////////////////////
  template <typename T>
  bool rect<T>::is_point_inside(T x, T y) const
  {
    return (x >= left() && x <= right() && y >= bottom() && y <= top());
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  rect<T> rect<T>::from_left_right_bottom_top(T left, T right, T bottom, T top)
  {
    GGO_ASSERT(left <= right);
    GGO_ASSERT(bottom <= top);

    return rect(left, bottom, right - left, top - bottom);
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  rect<T> rect<T>::from_union(const rect<T> & rect1, const rect<T> & rect2)
  {
    return from_left_right_bottom_top(std::min(rect1.left(), rect2.left()),
                                      std::max(rect1.right(), rect2.right()),
                                      std::min(rect1.bottom(), rect2.bottom()),
                                      std::max(rect1.top(), rect2.top()));
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  T rect<T>::dist_to_point(T x, T y) const
  {
    if (is_point_inside(x, y) == true)
    {
      return 0;
    }

    ggo::set2<T> p1(left(),  bottom());
    ggo::set2<T> p2(left(),  top());
    ggo::set2<T> p3(right(), top());
    ggo::set2<T> p4(right(), bottom());

    T d1 = ggo::segment<T>(p1, p2).dist_to_point(x, y);
    T d2 = ggo::segment<T>(p2, p3).dist_to_point(x, y);
    T d3 = ggo::segment<T>(p3, p4).dist_to_point(x, y);
    T d4 = ggo::segment<T>(p4, p1).dist_to_point(x, y);

    return std::min(std::min(d1, d2), std::min(d3, d4));
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  rect_intersection rect<T>::get_rect_intersection(const rect_data<T> & rect_data) const
  {
    T left    = rect_data._pos.x();
    T bottom  = rect_data._pos.y();
    T right   = left + rect_data._width;
    T top     = bottom + rect_data._height;

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
  template <typename T>
  void rect<T>::inflate(T extent)
  {
    _rect_data._pos.x() -= extent;
    _rect_data._pos.y() -= extent;
    _rect_data._width += 2 * extent;
    _rect_data._height += 2 * extent;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  void rect<T>::inflate(T horz_extent, T vert_extent)
  {
    _rect_data._pos.x() -= horz_extent;
    _rect_data._pos.y() -= vert_extent;
    _rect_data._width  += 2 * horz_extent;
    _rect_data._height += 2 * vert_extent;
  }
}
