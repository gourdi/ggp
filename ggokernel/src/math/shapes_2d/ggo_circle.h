//////////////////////////////////////////////////////////////////
//
// WARNING : NEVER DIRECTLY INCLUDE THIS FILE!
// INCLUDE 'ggo_math.h' INSTEAD.
//
//////////////////////////////////////////////////////////////////

namespace ggo
{
  template <typename T>
  class circle
  {
  public:

                              circle() {}
                              circle(T x, T y, T radius) : _center(x, y), _radius(radius) {}
                              circle(const ggo::set2<T> & center, T radius) : _center(center), _radius(radius) {}

    ggo::set2<T> &		        center() { return _center; }
    const ggo::set2<T> &	    center() const { return _center; }
                
    T &					              radius() { return _radius; }
    const T &			            radius() const { return _radius; }
                     
    std::vector<ggo::set2<T>> intersect_line(const ggo::set2<T> & p1, const ggo::set2<T> & p2, bool between_points_only) const;
    std::vector<ggo::set2<T>> intersect_segment(const ggo::set2<T> & p1, const ggo::set2<T> & p2) const;
    bool                      intersect_segment(T x_from, T y_from, T x_to, T y_to) const;

    bool                      is_point_inside(const ggo::set2<T> & p) const { return is_point_inside(p.x(), p.y()); }
    bool                      is_point_inside(T x, T y) const;

  private:

    ggo::set2<T>  _center;
    T             _radius;
  };
}

namespace ggo
{
#if __cplusplus >= 201103L
  using circle_float  = circle<float>;
  using circle_double = circle<double>;
#else
  typedef circle<float>   circle_float;
  typedef circle<double>  circle_double;
#endif
}
