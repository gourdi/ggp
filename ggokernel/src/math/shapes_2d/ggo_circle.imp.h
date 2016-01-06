namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename T>
  bool circle<T>::is_point_inside(T x, T y) const
  {
    T dx 	= _center.x() - x;
    T dy 	= _center.y() - y;
    T hypot	= dx * dx + dy * dy;

    return hypot <= _radius * _radius;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  std::vector<ggo::set2<T> > circle<T>::intersect_segment(const ggo::set2<T> & p1, const ggo::set2<T> & p2) const
  {
    return intersect_line(p1, p2, true);
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  std::vector<ggo::set2<T> > circle<T>::intersect_line(const ggo::set2<T> & p1, const ggo::set2<T> & p2, bool between_points_only) const
  {
    ggo::set2<T> dir = p2 - p1;
    T dx = p1.x() - _center.x();
    T dy = p1.y() - _center.y();
    
    T sol1, sol2;
    T deg2 = dir.x() * dir.x() + dir.y() * dir.y();
    T deg1 = 2 * (dir.x() * dx + dir.y() * dy);
    T deg0 = dx * dx + dy * dy - _radius * _radius;

    std::vector<ggo::set2<T> > result;
    
    if (ggo::quadratic_solve(deg2, deg1, deg0, sol1, sol2) == true)
    {
      if (between_points_only == false || (sol1 >= 0 && sol1 <= 1))
      {
        result.push_back(p1 + sol1 * dir);
      }
      
      if (between_points_only == false || (sol2 >= 0 && sol2 <= 1))
      {
        result.push_back(p1 + sol2 * dir);
      }
    }
      
    return result;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  bool circle<T>::intersect_segment(T x_from, T y_from, T x_to, T y_to) const
  {
    bool p1_inside = is_point_inside(x_from, y_from);
    bool p2_inside = is_point_inside(x_to, y_to);
    
    if (p1_inside == true && p2_inside == true)
    {
      return false;
    }
    
    if (p1_inside == true && p2_inside == false)
    {
      return true;
    }
    
    if (p1_inside == false && p2_inside == true)
    {
      return true;
    }
    
    GGO_ASSERT(p1_inside == false);
    GGO_ASSERT(p2_inside == false);
    
    T hypot = ggo::segment<T>(x_from, y_from, x_to, y_to).hypot_to_point(_center);
    return hypot < _radius * _radius;
  }
}



