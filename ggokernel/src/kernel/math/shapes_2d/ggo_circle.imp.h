namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  bool circle<data_t>::is_point_inside(data_t x, data_t y) const
  {
    data_t dx 	= _center.x() - x;
    data_t dy 	= _center.y() - y;
    data_t hypot	= dx * dx + dy * dy;

    return hypot <= _radius * _radius;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  std::vector<ggo::pos2<data_t>> circle<data_t>::get_segment_intersection(const ggo::pos2<data_t> & p1, const ggo::pos2<data_t> & p2) const
  {
    return get_line_intersection(p1, p2, true);
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  std::vector<ggo::pos2<data_t>> circle<data_t>::get_line_intersection(const ggo::pos2<data_t> & p1, const ggo::pos2<data_t> & p2, bool between_points_only) const
  {
    ggo::pos2<data_t> dir = p2 - p1;
    data_t dx = p1.x() - _center.x();
    data_t dy = p1.y() - _center.y();
    
    data_t sol1, sol2;
    data_t deg2 = dir.x() * dir.x() + dir.y() * dir.y();
    data_t deg1 = 2 * (dir.x() * dx + dir.y() * dy);
    data_t deg0 = dx * dx + dy * dy - _radius * _radius;

    std::vector<ggo::pos2<data_t>> result;
    
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
  template <typename data_t>
  bool circle<data_t>::test_segment_intersection(const ggo::pos2<data_t> & p1, const ggo::pos2<data_t> & p2) const
  {
    bool p1_inside = is_point_inside(p1);
    bool p2_inside = is_point_inside(p2);
    
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
    
    data_t hypot = ggo::segment<data_t>(p1, p2).hypot_to_point(_center);
    return hypot < _radius * _radius;
  }
}



