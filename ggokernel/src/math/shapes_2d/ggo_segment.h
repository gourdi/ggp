//////////////////////////////////////////////////////////////////
//
// WARNING : NEVER DIRECTLY INCLUDE THIS FILE!
// INCLUDE 'ggo_math.h' INSTEAD.
//
//////////////////////////////////////////////////////////////////

namespace ggo
{
  template <typename T>
  class segment : public movable_shape2d_abc<T>,
                  public rotatable_shape2d_abc<T>,
                  public distancable_shape2d_abc<T>
  {
  public:

                          segment() {};
                          segment(const ggo::set2<T> & p1, const ggo::set2<T> & p2) : _p1(p1), _p2(p2) {};
                          segment(T x1, T y1, T x2, T y2) : _p1(x1, y1), _p2(x2, y2) {};

    const ggo::set2<T> &  p1() const { return _p1; };
    const ggo::set2<T> &	p2() const { return _p2; };

    ggo::set2<T> &		    p1() { return _p1; };
    ggo::set2<T> &		    p2() { return _p2; };

    T					            get_hypot() const;
    T					            get_length() const;
  
    T                     hypot_to_point(T x, T y) const;
    T                     hypot_to_point(const ggo::set2<T> & p) const { return hypot_to_point(p.x(), p.y()); };
    T                     dist_to_point(const ggo::set2<T> & p) const { return dist_to_point(p.x(), p.y()); };
  
    T                     hypot_to_segment(T x_from, T y_from, T x_to, T y_to) const;
    T                     hypot_to_segment(const ggo::set2<T> & p1, const ggo::set2<T> & p2) const;
    T                     hypot_to_segment(const segment<T> & segment) const;
    T                     dist_to_segment(T x_from, T y_from, T x_to, T y_to) const;
    T                     dist_to_segment(const segment<T> & segment) const;
  
    bool				          same_side(T x1, T y1, T x2, T y2) const;
    bool				          intersect_segment(const segment<T> & segment) const;
    bool				          intersect_segment(const segment<T> & segment, ggo::set2<T> & intersect) const;
    bool                  intersect_horizontal_segment(T x_inf, T x_sup, T y) const;
    bool                  intersect_vertical_segment(T x, T y_inf, T y_sup) const;

    // Line equation is ax+by+c=0.
    void				          get_line(T & a, T & b, T & c) const;

    // Interfaces.
    void	                move(T dx, T dy) override { _p1.move(dx, dy); _p2.move(dx, dy); };
    void	                rotate(T angle, const ggo::set2<T> & center) override { _p1.rotate(angle, center); _p2.rotate(angle, center); };
    T                     dist_to_point(T x, T y) const override;

  private:

    ggo::set2<T>	_p1;
    ggo::set2<T>	_p2;
  };
}

namespace ggo
{
#if __cplusplus >= 201103L
  using segment_int       = segment<int>;
  using segment_float     = segment<float>;
  using segment_double    = segment<double>;
#else
  typedef segment<int>    segment_int;
  typedef segment<float>  segment_float;
  typedef segment<double> segment_double;
#endif
}

namespace ggo
{
  template <typename T>
  std::ostream &	operator<<(std::ostream & os, const segment<T> & segment)
  {
    os << segment.p1() << ' ' << segment.p2();
    return os;
  }

  template <typename T>
  std::istream &	operator>>(std::istream & is, segment<T> & segment)
  {
    is >> segment.p1() >> segment.p2();
    return is;
  }
}
