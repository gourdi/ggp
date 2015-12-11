namespace ggo
{
  //////////////////////////////////////////////////////////////////
  template <typename T>
  void triangle2d<T>::move(T dx, T dy)
  {
    _v1.x() += dx;
    _v1.y() += dy;
    _v2.x() += dx;
    _v2.y() += dy;
    _v3.x() += dx;
    _v3.y() += dy;
  }
  
  //////////////////////////////////////////////////////////////////
  template <typename T>
  bool triangle2d<T>::is_point_inside(T x, T y) const
  {
    ggo::set2<T> p(x, y);
    ggo::set2<T> d(p - _v1);
    ggo::set2<T> d2(_v2 - _v1);
    ggo::set2<T> d3(_v3 - _v1);
    
    T m[2][2] = {{d2.x(), d3.x()},
                 {d2.y(), d3.y()}};
    T c[2] = {d.x(), d.y()};
    T s[2] = {0, 0};
    
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
  template <typename T>
  rect_data<T> triangle2d<T>::get_bounding_rect() const
  {
    T left    = ggo::min(_v1.x(), _v2.x(), _v3.x());
    T right   = ggo::max(_v1.x(), _v2.x(), _v3.x());
    T bottom  = ggo::min(_v1.y(), _v2.y(), _v3.y());
    T top     = ggo::max(_v1.y(), _v2.y(), _v3.y());
    
    return { {left, bottom}, right - left, top - bottom };
  }
    
  //////////////////////////////////////////////////////////////////
  template <typename T>
  rect_intersection triangle2d<T>::get_rect_intersection(const rect_data<T> & rect_data) const
  {
    T left    = rect_data._pos.x();
    T bottom  = rect_data._pos.y();
    T right   = left + rect_data._width;
    T top     = bottom + rect_data._height;
    
    if ((_v1.x() >= left && _v1.x() <= right && _v1.y() >= bottom && _v1.y() <= top) &&
        (_v2.x() >= left && _v2.x() <= right && _v2.y() >= bottom && _v2.y() <= top) &&
        (_v3.x() >= left && _v3.x() <= right && _v3.y() >= bottom && _v3.y() <= top))
    {
      return rect_intersection::SHAPE_IN_RECT;
    }
    
    if (is_point_inside(left, top) && 
        is_point_inside(right, top) &&
        is_point_inside(right, bottom) &&
        is_point_inside(left, bottom))
    {
      return rect_intersection::RECT_IN_SHAPE;
    }
    
    if (right  < ggo::min(_v1.x(), _v2.x(), _v3.x()) ||
        left   > ggo::max(_v1.x(), _v2.x(), _v3.x()) ||
        top    < ggo::min(_v1.y(), _v2.y(), _v3.y()) ||
        bottom > ggo::max(_v1.y(), _v2.y(), _v3.y()))
    {
      return rect_intersection::DISJOINTS;
    }
    
    segment<T> s12(_v1, _v2);
    if (s12.same_side(_v3.x(), _v3.y(), left,  top   ) == false &&
        s12.same_side(_v3.x(), _v3.y(), right, top   ) == false &&
        s12.same_side(_v3.x(), _v3.y(), right, bottom) == false &&
        s12.same_side(_v3.x(), _v3.y(), left,  bottom) == false)
    {
      return rect_intersection::DISJOINTS;
    }
    
    segment<T> s23(_v2, _v3);
    if (s23.same_side(_v1.x(), _v1.y(), left,  top   ) == false &&
        s23.same_side(_v1.x(), _v1.y(), right, top   ) == false &&
        s23.same_side(_v1.x(), _v1.y(), right, bottom) == false &&
        s23.same_side(_v1.x(), _v1.y(), left,  bottom) == false)
    {
      return rect_intersection::DISJOINTS;
    }
    
        segment<T> s13(_v1, _v3);
    if (s13.same_side(_v2.x(), _v2.y(), left,  top   ) == false &&
        s13.same_side(_v2.x(), _v2.y(), right, top   ) == false &&
        s13.same_side(_v2.x(), _v2.y(), right, bottom) == false &&
        s13.same_side(_v2.x(), _v2.y(), left,  bottom) == false)
    {
      return rect_intersection::DISJOINTS;
    }
    
    return rect_intersection::PARTIAL_OVERLAP;
  }
}
