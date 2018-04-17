namespace ggo
{
  //////////////////////////////////////////////////////////////////
  template <typename data_t, boolean_mode mode>
  bool multi_shape<data_t, mode>::is_point_inside(const ggo::pos2<data_t> & p) const
  {
    switch (mode)
    {
    case boolean_mode::UNION:
      return is_point_inside_union(p);
    case boolean_mode::INTERSECTION:
      return is_point_inside_intersection(p);
    case boolean_mode::DIFFERENCE:
      return is_point_inside_difference(p);
    default:
      GGO_FAIL();
      return false;
    }
  }

  //////////////////////////////////////////////////////////////////
  template<typename data_t, boolean_mode mode>
  bool multi_shape<data_t, mode>::is_point_inside_union(const ggo::pos2<data_t> & p) const
  {
    for (const auto & shape : _shapes)
    {
      if (shape->is_point_inside(p) == true)
      {
        return true;
      }
    }
    return false;
  }

  //////////////////////////////////////////////////////////////////
  template<typename data_t, boolean_mode mode>
  bool multi_shape<data_t, mode>::is_point_inside_intersection(const ggo::pos2<data_t> & p) const
  {
    for (const auto & shape : _shapes)
    {
      if (shape->is_point_inside(p) == false)
      {
        return false;
      }
    }
    return true; // Inside all the shapes.
  }

  //////////////////////////////////////////////////////////////////
  template<typename data_t, boolean_mode mode>
  bool multi_shape<data_t, mode>::is_point_inside_difference(const ggo::pos2<data_t> & p) const
  {
    // The point must be in the first shape but not in the following ones.
    for (auto it = this->_shapes.begin(); it != this->_shapes.end(); ++it)
    {
      auto shape = *it;

      if (it == _shapes.begin())
      {
        if (shape->is_point_inside(p) == false)
        {
          return false;
        }
      }
      else
      {
        if (shape->is_point_inside(p) == true)
        {
          return false;
        }
      }
    }
    return true;
  }

  //////////////////////////////////////////////////////////////////
  template <typename data_t, boolean_mode mode>
  rect_data<data_t> multi_shape<data_t, mode>::get_bounding_rect() const
  {
    if (_shapes.empty() == true)
    {
      return { { data_t(0), data_t(0) }, data_t(0), data_t(0) };
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
      return{ { data_t(0), data_t(0) }, data_t(0), data_t(0) };
    }
  }

  //////////////////////////////////////////////////////////////////
  template<typename data_t, boolean_mode mode>
  rect_data<data_t> multi_shape<data_t, mode>::get_bounding_rect_union() const
  {
    auto it = _shapes.begin();
    auto shape_ptr = *it;
    auto rect = shape_ptr->get_bounding_rect();
    it++;

    while (it != _shapes.end())
    {
      auto shape_ptr = *it;
      rect = get_union(rect, shape_ptr->get_bounding_rect());

      it++;
    }

    return rect;
  }

  //////////////////////////////////////////////////////////////////
  template<typename data_t, boolean_mode mode>
  rect_data<data_t> multi_shape<data_t, mode>::get_bounding_rect_intersection() const
  {
    auto it = _shapes.begin();
    auto shape_ptr = *it;
    auto rect = shape_ptr->get_bounding_rect();
    it++;

    while (it != _shapes.end())
    {
      auto shape_ptr = *it;
      auto intersection = get_intersection(rect, shape_ptr->get_bounding_rect());
      if (!intersection)
      {
        return{ { data_t(0), data_t(0) }, data_t(0), data_t(0) };
      }

      rect = *intersection;

      it++;
    }

    return rect;
  }

  //////////////////////////////////////////////////////////////////
  template<typename data_t, boolean_mode mode>
  rect_data<data_t> multi_shape<data_t, mode>::get_bounding_rect_difference() const
  {
    auto it = _shapes.begin();
    auto shape_ptr = *it;
    return shape_ptr->get_bounding_rect();
  }
  
  //////////////////////////////////////////////////////////////////
  template <typename data_t, boolean_mode mode>
  rect_intersection multi_shape<data_t, mode>::get_rect_intersection(const rect_data<data_t> & rect_data) const
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
      return rect_intersection::unknown;
    }
  }

  //////////////////////////////////////////////////////////////////
  template <typename data_t, boolean_mode mode>
  rect_intersection multi_shape<data_t, mode>::get_rect_intersection_union(const rect_data<data_t> & rect_data) const
  {
    bool disjoint_all = true;
    bool shape_in_rect_all = true;

    for (const auto & shape : _shapes)
    {
      switch (shape->get_rect_intersection(rect_data))
      {
      case rect_intersection::rect_in_shape:
        return rect_intersection::rect_in_shape;
      case rect_intersection::shape_in_rect:
        disjoint_all = false;
        break;
      case rect_intersection::disjoints:
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
      return rect_intersection::disjoints;
    }
    else if (shape_in_rect_all == true)
    {
      return rect_intersection::shape_in_rect;
    }
    else
    {
      return rect_intersection::unknown;
    }
  }

  //////////////////////////////////////////////////////////////////
  template <typename data_t, boolean_mode mode>
  rect_intersection multi_shape<data_t, mode>::get_rect_intersection_intersection(const rect_data<data_t> & rect_data) const
  {
    bool rect_in_shape_all = true;
    bool shape_in_rect_all = true;

    for (const auto & shape : _shapes)
    {
      switch (shape->get_rect_intersection(rect_data))
      {
      case rect_intersection::rect_in_shape:
        shape_in_rect_all = false;
        break;
      case rect_intersection::shape_in_rect:
        rect_in_shape_all = false;
        break;
      case rect_intersection::disjoints:
        return rect_intersection::disjoints;
      default:
        rect_in_shape_all = false;
        shape_in_rect_all = false;
        break;
      }
    }

    if (rect_in_shape_all == true)
    {
      return rect_intersection::rect_in_shape;
    }
    else if (shape_in_rect_all == true)
    {
      return rect_intersection::shape_in_rect;
    }
    else
    {
      return rect_intersection::unknown;
    }
  }

  //////////////////////////////////////////////////////////////////
  template <typename data_t, boolean_mode mode>
  rect_intersection multi_shape<data_t, mode>::get_rect_intersection_difference(const rect_data<data_t> & rect_data) const
  {
    auto it = _shapes.begin();
    auto shape = *it++;

    // Process the first shape.
    switch (shape->get_rect_intersection(rect_data))
    {
    case rect_intersection::disjoints:
      return rect_intersection::disjoints;
    case rect_intersection::shape_in_rect:
      return rect_intersection::shape_in_rect;
    case rect_intersection::rect_in_shape:
    {
      bool disjoints_all = true;
      auto it2 = it;
      for (; it2 != _shapes.end(); ++it2)
      {
        shape = *it2;
        if (shape->get_rect_intersection(rect_data) != rect_intersection::disjoints)
        {
          disjoints_all = false;
          break;
        }
      }
      if (disjoints_all == true)
      {
        return rect_intersection::rect_in_shape;
      }
    }
    default:
      break;
    }

    // 'it' references now the 2nd item (and not the first one).
    for (; it != _shapes.end(); ++it)
    {
      if (shape->get_rect_intersection(rect_data) == rect_intersection::rect_in_shape)
      {
        return rect_intersection::disjoints;
      }
    }

    return rect_intersection::unknown;
  }
}
