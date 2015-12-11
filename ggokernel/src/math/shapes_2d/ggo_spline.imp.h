namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename T>
  void spline<T>::add_control_point(const ggo::set2<T> & point)
  {
    _control_points.push_back(point);
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  void spline<T>::clear_control_points()
  {
    _control_points.clear();
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  ggo::set2<T> spline<T>::eval(const ggo::set2<T> & pp, const ggo::set2<T> & pc, const ggo::set2<T> & pn, const ggo::set2<T> & pnn, T t)
  {
    ggo::set2<T>	p;
    
    p  = t*t*t*(T(-1)*pp + T( 3)*pc + T(-3)*pn + pnn);
    p +=   t*t*(T( 3)*pp + T(-6)*pc + T( 3)*pn);
    p +=     t*(T(-3)*pp            + T( 3)*pn);
    p +=              pp + T( 4)*pc +       pn;
    
    p /= T(6);
    
    return p;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  std::vector<ggo::set2<T>> spline<T>::evaluate(int steps) const
  {
    std::vector<ggo::set2<T>> result;
    
    if (_control_points.size() >= 4)
    {
      auto it = _control_points.begin() + 1;
      for (; it != _control_points.end() - 2; ++it)
      {
        for (int i = 0; i < steps; ++i)
        {
          T t = T(i) / steps;
          ggo::set2<T> p = eval( *(it-1), *it, *(it+1), *(it+2), t );
          
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
  template <typename T>
  void spline<T>::move(T dx, T dy)
  {
    for (auto & point : _control_points)
    {
      point.x() += dx;
      point.y() += dy;
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  void spline<T>::rotate(T angle, const ggo::set2<T> & center)
  {
    for (auto & point : _control_points)
    {
      point.rotate(angle, center);
    }
  }
}
