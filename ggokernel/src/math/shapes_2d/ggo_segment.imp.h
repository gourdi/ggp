namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename T>
  T segment<T>::get_length() const
  {
    return ggo::distance(_p1, _p2); 
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  T segment<T>::get_hypot() const
  {
    return ggo::hypot(_p1, _p2); 
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  T segment<T>::hypot_to_point(T x, T y) const
  {
    ggo::set2<T> p(x, y);
    ggo::set2<T> v1(_p2 - _p1);
    ggo::set2<T> v2(p - _p1);
    T hypot = ggo::dot(v1, v1);

    if (hypot < T(0.001))
    {
      return ggo::hypot(x, y, _p1.x(), _p1.y());
    }
    else
    {
      T dot = ggo::dot(v1, v2);

      if (dot < 0) 
      {
        return ggo::hypot(p, _p1);
      }
      else
      if (dot > hypot)
      {
        return ggo::hypot(p, _p2);
      }
      else
      {
        ggo::set2<T> proj(_p1 + v1 * dot / hypot);
        return ggo::hypot(p, proj);
      }
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  T segment<T>::dist_to_point(T x, T y) const
  {
    return std::sqrt(hypot_to_point(x, y));
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  T segment<T>::hypot_to_segment(T x_p1, T y_p1, T x_p2, T y_p2) const
  {
    ggo::set2<T> intersect;
    if (intersect_segment(segment<T>(x_p1, y_p1, x_p2, y_p2), intersect)  == true)
    {
      return 0;		
    }

    segment<T> segment(x_p1, y_p1, x_p2, y_p2);

    T d1 = hypot_to_point(segment.p1().x(), segment.p1().y());
    T d2 = hypot_to_point(segment.p2().x(), segment.p2().y());
    T d3 = segment.hypot_to_point(p1().x(), p1().y());
    T d4 = segment.hypot_to_point(p2().x(), p2().y());

    return std::min(std::min(d1, d2), std::min(d3, d4));
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  T segment<T>::hypot_to_segment(const ggo::set2<T> & p1, const ggo::set2<T> & p2) const
  {
    return hypot_to_segment(p1.x(), p1.y(), p2.x(), p2.y());
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  T segment<T>::hypot_to_segment(const segment<T> & segment) const
  {
    return hypot_to_segment(segment.p1(), segment.p2());
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  T segment<T>::dist_to_segment(T x_p1, T y_p1, T x_p2, T y_p2) const
  {
    return std::sqrt(hypot_to_segment(x_p1, y_p1, x_p2, y_p2));
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  T segment<T>::dist_to_segment(const segment<T> & segment) const
  {
    return dist_to_segment(segment.p1().x(), segment.p1().y(), segment.p2().x(), segment.p2().y());
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  bool segment<T>::same_side(T x1, T y1, T x2, T y2) const
  {
    T dx		  = _p1.x() - _p2.x();
    T dy		  = _p1.y() - _p2.y();
    T dx1		  = _p1.x() - x1;
    T dy1		  = _p1.y() - y1;
    T dx2		  = _p1.x() - x2;
    T dy2		  = _p1.y() - y2;
    T cross1	= dx * dy1 - dy * dx1;
    T cross2	= dx * dy2 - dy * dx2;

    return cross1 * cross2 > 0;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  void segment<T>::get_line(T & a, T & b, T & c) const
  {
    T det = _p2.x() * _p1.y() - _p1.x() * _p2.y();

    if (std::abs(det) < 0.00001)
    {
      // The line equation is ax+by=0. So let us compute a and b.
      if (std::abs(_p1.x()) + std::abs(_p1.y()) > std::abs(_p2.x()) + std::abs(_p2.y()))
      { 
        a = _p1.y();
        b = -_p1.x();
      }
      else
      {
        a = _p2.y();
        b = -_p2.x();
      }
      c = 0;
    }
    else
    {
      // The line equation is ax+by+c=0. So let us compute a and b.
      a = _p2.y() - _p1.y();
      b = _p1.x() - _p2.x();
      c = det;
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  bool segment<T>::intersect_horizontal_segment(T x_inf, T x_sup, T y) const
  {
    GGO_ASSERT(x_inf <= x_sup);

    T dy = _p2.y() - _p1.y();
    if (std::abs(dy) < T(0.00001))
    {
      return false; // Current segment is horizontal => non intersection.
    }

    T k = (y - _p1.y()) / dy; 
    if (k < 0 || k > 1)
    {
      return false;
    }

    T x = _p1.x() + k * (_p2.x() - _p1.x());
    if (x < x_inf || x > x_sup)
    {
      return false;
    }

    return true;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  bool segment<T>::intersect_vertical_segment(T x, T y_inf, T y_sup) const
  {
    GGO_ASSERT(y_inf <= y_sup);

    T dx = _p2.x() - _p1.x();
    if (std::abs(dx) < T(0.00001))
    {
      return false; // Current segment is horizontal => non intersection.
    }

    T k = (x - _p1.x()) / dx; 
    if (k < 0 || k > 1)
    {
      return false;
    }

    T y = _p1.y() + k * (_p2.y() - _p1.y());
    if (y < y_inf || y > y_sup)
    {
      return false;
    }

    return true;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  bool segment<T>::intersect_segment(const segment<T> & segment) const
  {
    ggo::set2<T> intersect;
    return intersect_segment(segment, intersect);
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  bool segment<T>::intersect_segment(const segment<T> & segment, ggo::set2<T> & intersect) const
  {
    T dx1 = _p2.x() - _p1.x();
    T dy1 = _p2.y() - _p1.y();
    T dx2 = segment._p2.x() - segment._p1.x();
    T dy2 = segment._p2.y() - segment._p1.y();

    T det = dx1 * dy2 - dy1 * dx2;
    if (std::fabs(det) < T(0.00001))
    {
      return false;
    }

    T dx3 = _p1.x() - segment._p1.x();
    T dy3 = _p1.y() - segment._p1.y();

    T r1 = (dy3 * dx2 - dx3 * dy2) / det;
    if ((r1 < 0) || (r1 > 1))
    {
      return false;
    }

    T r2 = (dy3 * dx1 - dx3 * dy1) / det;
    if ((r2 < 0) || (r2 > 1))
    {
      return false;
    }

    intersect.x() = _p1.x() + r1 * dx1; 
    intersect.y() = _p1.y() + r1 * dy1; 

    return true;
  }
}


