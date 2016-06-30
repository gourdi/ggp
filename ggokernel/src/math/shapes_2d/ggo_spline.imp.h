namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  void spline<data_t>::add_control_point(const ggo::pos2<data_t> & point)
  {
    _control_points.push_back(point);
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  void spline<data_t>::clear_control_points()
  {
    _control_points.clear();
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  ggo::pos2<data_t> spline<data_t>::eval(const ggo::pos2<data_t> & pp, const ggo::pos2<data_t> & pc, const ggo::pos2<data_t> & pn, const ggo::pos2<data_t> & pnn, data_t t)
  {
    ggo::pos2<data_t>	p;
    
    p  = t*t*t*(data_t(-1)*pp + data_t( 3)*pc + data_t(-3)*pn + pnn);
    p +=   t*t*(data_t( 3)*pp + data_t(-6)*pc + data_t( 3)*pn);
    p +=     t*(data_t(-3)*pp                 + data_t( 3)*pn);
    p +=                   pp + data_t( 4)*pc +            pn;
    
    p /= data_t(6);
    
    return p;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  std::vector<ggo::pos2<data_t>> spline<data_t>::evaluate(int steps) const
  {
    std::vector<ggo::pos2<data_t>> result;
    
    if (_control_points.size() >= 4)
    {
      auto it = _control_points.begin() + 1;
      for (; it != _control_points.end() - 2; ++it)
      {
        for (int i = 0; i < steps; ++i)
        {
          data_t t = data_t(i) / steps;
          ggo::pos2<data_t> p = eval( *(it-1), *it, *(it+1), *(it+2), t );
          
          result.push_back(p);
        }
      }
      
      // The very last point.
      --it;
      result.push_back(eval( *(it-1), *it, *(it+1), *(it+2), 1));
    }
    
    return result;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  void spline<data_t>::move(data_t dx, data_t dy)
  {
    for (auto & point : _control_points)
    {
      point.move(dx, dy);
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  void spline<data_t>::rotate(data_t angle, const ggo::pos2<data_t> & center)
  {
    for (auto & point : _control_points)
    {
      point = ggo::rotate(point, center, angle);
    }
  }
}
