namespace ggo
{
  //////////////////////////////////////////////////////////////////
  template <typename T, boolean_mode mode>
  bool multi_shape<T, mode>::is_point_inside(T x, T y) const
  {
    switch (mode)
    {
    case boolean_mode::UNION:
      return is_point_inside_union(x, y);
    case boolean_mode::INTERSECTION:
      return is_point_inside_intersection(x, y);
    case boolean_mode::DIFFERENCE:
      return is_point_inside_difference(x, y);
    default:
      GGO_FAIL();
      return false;
    }
  }

  //////////////////////////////////////////////////////////////////
  template<typename T, boolean_mode mode>
  bool multi_shape<T, mode>::is_point_inside_union(T x, T y) const
  {
    for (const auto & shape : _shapes)
    {
      if (shape->is_point_inside(x, y) == true)
      {
        return true;
      }
    }
    return false;
  }

  //////////////////////////////////////////////////////////////////
  template<typename T, boolean_mode mode>
  bool multi_shape<T, mode>::is_point_inside_intersection(T x, T y) const
  {
    for (const auto & shape : _shapes)
    {
      if (shape->is_point_inside(x, y) == false)
      {
        return false;
      }
    }
    return true; // Inside all the shapes.
  }

  //////////////////////////////////////////////////////////////////
  template<typename T, boolean_mode mode>
  bool multi_shape<T, mode>::is_point_inside_difference(T x, T y) const
  {
    // The point must be in the first shape but not in the following ones.
    for (auto it = this->_shapes.begin(); it != this->_shapes.end(); ++it)
    {
      auto shape = *it;

      if (it == _shapes.begin())
      {
        if (shape->is_point_inside(x, y) == false)
        {
          return false;
        }
      }
      else
      {
        if (shape->is_point_inside(x, y) == true)
        {
          return false;
        }
      }
    }
    return true;
  }

  //////////////////////////////////////////////////////////////////
  template <typename T, boolean_mode mode>
  rect_data<T> multi_shape<T, mode>::get_bounding_rect() const
  {
    if (_shapes.empty() == true)
    {
      return { { T(0), T(0) }, T(0), T(0) };
    }
    
    switch (mode)
    {
    case boolean_mode::UNION:
      return get_bounding_rect_union();
    case boolean_mode::INTERSECTION:
      return get_bounding_rect_intersection();
    case boolean_mode::DIFFERENCE:
      return get_bounding_rect_difference();
    default:
      GGO_FAIL();
      return{ { T(0), T(0) }, T(0), T(0) };
    }
  }

  //////////////////////////////////////////////////////////////////
  template<typename T, boolean_mode mode>
  rect_data<T> multi_shape<T, mode>::get_bounding_rect_union() const
  {
    auto it = _shapes.begin();
    auto shape_ptr = *it;
    auto rect = shape_ptr->get_bounding_rect();
    it++;

    while (it != _shapes.end())
    {
      auto shape_ptr = *it;
      rect = rect_data_union(rect, shape_ptr->get_bounding_rect());

      it++;
    }

    return rect;
  }

  //////////////////////////////////////////////////////////////////
  template<typename T, boolean_mode mode>
  rect_data<T> multi_shape<T, mode>::get_bounding_rect_intersection() const
  {
    auto it = _shapes.begin();
    auto shape_ptr = *it;
    auto rect = shape_ptr->get_bounding_rect();
    it++;

    while (it != _shapes.end())
    {
      auto shape_ptr = *it;
      if (rect_data_intersection(rect, shape_ptr->get_bounding_rect(), rect) == false)
      {
        return{ { T(0), T(0) }, T(0), T(0) };
      }

      it++;
    }

    return rect;
  }

  //////////////////////////////////////////////////////////////////
  template<typename T, boolean_mode mode>
  rect_data<T> multi_shape<T, mode>::get_bounding_rect_difference() const
  {
    auto it = _shapes.begin();
    auto shape_ptr = *it;
    return shape_ptr->get_bounding_rect();
  }
  
  //////////////////////////////////////////////////////////////////
  template <typename T, boolean_mode mode>
  rect_intersection multi_shape<T, mode>::get_rect_intersection(const rect_data<T> & rect_data) const
  {
    switch (mode)
    {
    case boolean_mode::UNION:
      return get_rect_intersection_union(rect_data);
    case boolean_mode::INTERSECTION:
      return get_rect_intersection_intersection(rect_data);
    case boolean_mode::DIFFERENCE:
      return get_rect_intersection_difference(rect_data);
    default:
      GGO_FAIL();
      return rect_intersection::UNKNOWN;
    }
  }

  //////////////////////////////////////////////////////////////////
  template <typename T, boolean_mode mode>
  rect_intersection multi_shape<T, mode>::get_rect_intersection_union(const rect_data<T> & rect_data) const
  {
    bool disjoint_all = true;
    bool shape_in_rect_all = true;

    for (const auto & shape : _shapes)
    {
      switch (shape->get_rect_intersection(rect_data))
      {
      case rect_intersection::RECT_IN_SHAPE:
        return rect_intersection::RECT_IN_SHAPE;
      case rect_intersection::SHAPE_IN_RECT:
        disjoint_all = false;
        break;
      case rect_intersection::DISJOINTS:
        shape_in_rect_all = false;
        break;
      default:
        disjoint_all = false;
        shape_in_rect_all = false;
        break;
      }
    }

    if (disjoint_all == true)
    {
      return rect_intersection::DISJOINTS;
    }
    else if (shape_in_rect_all == true)
    {
      return rect_intersection::SHAPE_IN_RECT;
    }
    else
    {
      return rect_intersection::UNKNOWN;
    }
  }

  //////////////////////////////////////////////////////////////////
  template <typename T, boolean_mode mode>
  rect_intersection multi_shape<T, mode>::get_rect_intersection_intersection(const rect_data<T> & rect_data) const
  {
    bool rect_in_shape_all = true;
    bool shape_in_rect_all = true;

    for (const auto & shape : _shapes)
    {
      switch (shape->get_rect_intersection(rect_data))
      {
      case rect_intersection::RECT_IN_SHAPE:
        shape_in_rect_all = false;
        break;
      case rect_intersection::SHAPE_IN_RECT:
        rect_in_shape_all = false;
        break;
      case rect_intersection::DISJOINTS:
        return rect_intersection::DISJOINTS;
      default:
        rect_in_shape_all = false;
        shape_in_rect_all = false;
        break;
      }
    }

    if (rect_in_shape_all == true)
    {
      return rect_intersection::RECT_IN_SHAPE;
    }
    else if (shape_in_rect_all == true)
    {
      return rect_intersection::SHAPE_IN_RECT;
    }
    else
    {
      return rect_intersection::UNKNOWN;
    }
  }

  //////////////////////////////////////////////////////////////////
  template <typename T, boolean_mode mode>
  rect_intersection multi_shape<T, mode>::get_rect_intersection_difference(const rect_data<T> & rect_data) const
  {
    auto it = _shapes.begin();
    auto shape = *it++;

    // Process the first shape.
    switch (shape->get_rect_intersection(rect_data))
    {
    case rect_intersection::DISJOINTS:
      return rect_intersection::DISJOINTS;
    case rect_intersection::SHAPE_IN_RECT:
      return rect_intersection::SHAPE_IN_RECT;
    case rect_intersection::RECT_IN_SHAPE:
    {
      bool disjoints_all = true;
      auto it2 = it;
      for (; it2 != _shapes.end(); ++it2)
      {
        shape = *it2;
        if (shape->get_rect_intersection(rect_data) != rect_intersection::DISJOINTS)
        {
          disjoints_all = false;
          break;
        }
      }
      if (disjoints_all == true)
      {
        return rect_intersection::RECT_IN_SHAPE;
      }
    }
    default:
      break;
    }

    // 'it' references now the 2nd item (and not the first one).
    for (; it != _shapes.end(); ++it)
    {
      if (shape->get_rect_intersection(rect_data) == rect_intersection::RECT_IN_SHAPE)
      {
        return rect_intersection::DISJOINTS;
      }
    }

    return rect_intersection::UNKNOWN;
  }
}
