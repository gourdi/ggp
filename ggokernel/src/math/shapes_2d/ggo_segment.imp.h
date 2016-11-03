namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  data_t segment<data_t>::get_length() const
  {
    return ggo::distance(_p1, _p2); 
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  data_t segment<data_t>::get_hypot() const
  {
    return ggo::hypot(_p1, _p2); 
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  data_t segment<data_t>::hypot_to_point(data_t x, data_t y) const
  {
    ggo::pos2<data_t> p(x, y);
    ggo::pos2<data_t> v1(_p2 - _p1);
    ggo::pos2<data_t> v2(p - _p1);
    data_t hypot = ggo::dot(v1, v1);

    if (hypot < data_t(0.001))
    {
      return ggo::hypot(x, y, _p1.x(), _p1.y());
    }
    else
    {
      data_t dot = ggo::dot(v1, v2);

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
        ggo::pos2<data_t> proj(_p1 + v1 * dot / hypot);
        return ggo::hypot(p, proj);
      }
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  data_t segment<data_t>::dist_to_point(data_t x, data_t y) const
  {
    return std::sqrt(hypot_to_point(x, y));
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  data_t segment<data_t>::hypot_to_segment(data_t x_p1, data_t y_p1, data_t x_p2, data_t y_p2) const
  {
    ggo::pos2<data_t> intersect;
    if (intersect_segment(segment<data_t>(x_p1, y_p1, x_p2, y_p2), intersect)  == true)
    {
      return 0;		
    }

    segment<data_t> segment(x_p1, y_p1, x_p2, y_p2);

    data_t d1 = hypot_to_point(segment.p1().x(), segment.p1().y());
    data_t d2 = hypot_to_point(segment.p2().x(), segment.p2().y());
    data_t d3 = segment.hypot_to_point(p1().x(), p1().y());
    data_t d4 = segment.hypot_to_point(p2().x(), p2().y());

    return std::min(std::min(d1, d2), std::min(d3, d4));
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  data_t segment<data_t>::hypot_to_segment(const ggo::pos2<data_t> & p1, const ggo::pos2<data_t> & p2) const
  {
    return hypot_to_segment(p1.x(), p1.y(), p2.x(), p2.y());
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  data_t segment<data_t>::hypot_to_segment(const segment<data_t> & segment) const
  {
    return hypot_to_segment(segment.p1(), segment.p2());
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  data_t segment<data_t>::dist_to_segment(data_t x_p1, data_t y_p1, data_t x_p2, data_t y_p2) const
  {
    return std::sqrt(hypot_to_segment(x_p1, y_p1, x_p2, y_p2));
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  data_t segment<data_t>::dist_to_segment(const segment<data_t> & segment) const
  {
    return dist_to_segment(segment.p1().x(), segment.p1().y(), segment.p2().x(), segment.p2().y());
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  bool segment<data_t>::same_side(data_t x1, data_t y1, data_t x2, data_t y2) const
  {
    data_t dx		  = _p1.x() - _p2.x();
    data_t dy		  = _p1.y() - _p2.y();
    data_t dx1		= _p1.x() - x1;
    data_t dy1		= _p1.y() - y1;
    data_t dx2		= _p1.x() - x2;
    data_t dy2		= _p1.y() - y2;
    data_t cross1	= dx * dy1 - dy * dx1;
    data_t cross2	= dx * dy2 - dy * dx2;

    return cross1 * cross2 > 0;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  void segment<data_t>::get_line(data_t & a, data_t & b, data_t & c) const
  {
    data_t det = _p2.x() * _p1.y() - _p1.x() * _p2.y();

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
  template <typename data_t>
  bool segment<data_t>::intersect_horizontal_segment(data_t x_inf, data_t x_sup, data_t y) const
  {
    GGO_ASSERT(x_inf <= x_sup);

    data_t dy = _p2.y() - _p1.y();
    if (std::abs(dy) < data_t(0.00001))
    {
      return false; // Current segment is horizontal => non intersection.
    }

    data_t k = (y - _p1.y()) / dy; 
    if (k < 0 || k > 1)
    {
      return false;
    }

    data_t x = _p1.x() + k * (_p2.x() - _p1.x());
    if (x < x_inf || x > x_sup)
    {
      return false;
    }

    return true;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  bool segment<data_t>::intersect_vertical_segment(data_t x, data_t y_inf, data_t y_sup) const
  {
    GGO_ASSERT(y_inf <= y_sup);

    data_t dx = _p2.x() - _p1.x();
    if (std::abs(dx) < data_t(0.00001))
    {
      return false; // Current segment is horizontal => non intersection.
    }

    data_t k = (x - _p1.x()) / dx; 
    if (k < 0 || k > 1)
    {
      return false;
    }

    data_t y = _p1.y() + k * (_p2.y() - _p1.y());
    if (y < y_inf || y > y_sup)
    {
      return false;
    }

    return true;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  bool segment<data_t>::intersect_segment(const segment<data_t> & segment) const
  {
    ggo::pos2<data_t> intersect;
    return intersect_segment(segment, intersect);
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  bool segment<data_t>::intersect_segment(const segment<data_t> & segment, ggo::pos2<data_t> & intersect) const
  {
    data_t dx1 = _p2.x() - _p1.x();
    data_t dy1 = _p2.y() - _p1.y();
    data_t dx2 = segment._p2.x() - segment._p1.x();
    data_t dy2 = segment._p2.y() - segment._p1.y();

    data_t det = dx1 * dy2 - dy1 * dx2;
    if (std::fabs(det) < data_t(0.00001))
    {
      return false;
    }

    data_t dx3 = _p1.x() - segment._p1.x();
    data_t dy3 = _p1.y() - segment._p1.y();

    data_t r1 = (dy3 * dx2 - dx3 * dy2) / det;
    if ((r1 < 0) || (r1 > 1))
    {
      return false;
    }

    data_t r2 = (dy3 * dx1 - dx3 * dy1) / det;
    if ((r2 < 0) || (r2 > 1))
    {
      return false;
    }

    intersect.x() = _p1.x() + r1 * dx1; 
    intersect.y() = _p1.y() + r1 * dy1; 

    return true;
  }
}


