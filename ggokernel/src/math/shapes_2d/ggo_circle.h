//////////////////////////////////////////////////////////////////
//
// WARNING : NEVER DIRECTLY INCLUDE THIS FILE!
// INCLUDE 'ggo_math.h' INSTEAD.
//
//////////////////////////////////////////////////////////////////

namespace ggo
{
  template <typename data_t>
  class circle
  {
  public:

                                    circle() {}
                                    circle(data_t x, data_t y, data_t radius) : _center(x, y), _radius(radius) {}
                                    circle(const ggo::pos2<data_t> & center, data_t radius) : _center(center), _radius(radius) {}

    ggo::pos2<data_t> &		          center() { return _center; }
    const ggo::pos2<data_t> &	      center() const { return _center; }
                
    data_t &					              radius() { return _radius; }
    const data_t &			            radius() const { return _radius; }
                     
    std::vector<ggo::pos2<data_t>>  intersect_line(const ggo::pos2<data_t> & p1, const ggo::pos2<data_t> & p2, bool between_points_only) const;
    std::vector<ggo::pos2<data_t>>  intersect_segment(const ggo::pos2<data_t> & p1, const ggo::pos2<data_t> & p2) const;
    bool                            intersect_segment(data_t x_from, data_t y_from, data_t x_to, data_t y_to) const;

    bool                            is_point_inside(const ggo::pos2<data_t> & p) const { return is_point_inside(p.x(), p.y()); }
    bool                            is_point_inside(data_t x, data_t y) const;

  private:

    ggo::pos2<data_t>  _center;
    data_t             _radius;
  };
}

namespace ggo
{
  using circle_float  = circle<float>;
  using circle_double = circle<double>;
}
