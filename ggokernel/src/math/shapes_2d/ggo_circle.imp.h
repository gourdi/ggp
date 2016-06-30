namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  bool circle<data_t>::is_point_inside(data_t x, data_t y) const
  {
    data_t dx 	= _center.template get<0>() - x;
    data_t dy 	= _center.template get<1>() - y;
    data_t hypot	= dx * dx + dy * dy;

    return hypot <= _radius * _radius;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  std::vector<ggo::pos2<data_t>> circle<data_t>::intersect_segment(const ggo::pos2<data_t> & p1, const ggo::pos2<data_t> & p2) const
  {
    return intersect_line(p1, p2, true);
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  std::vector<ggo::pos2<data_t>> circle<data_t>::intersect_line(const ggo::pos2<data_t> & p1, const ggo::pos2<data_t> & p2, bool between_points_only) const
  {
    ggo::pos2<data_t> dir = p2 - p1;
    data_t dx = p1.template get<0>() - _center.template get<0>();
    data_t dy = p1.template get<1>() - _center.template get<1>();
    
    data_t sol1, sol2;
    data_t deg2 = dir.template get<0>() * dir.template get<0>() + dir.template get<1>() * dir.template get<1>();
    data_t deg1 = 2 * (dir.template get<0>() * dx + dir.template get<1>() * dy);
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
  bool circle<data_t>::intersect_segment(data_t x_from, data_t y_from, data_t x_to, data_t y_to) const
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
    
    data_t hypot = ggo::segment<data_t>(x_from, y_from, x_to, y_to).hypot_to_point(_center);
    return hypot < _radius * _radius;
  }
}



