//////////////////////////////////////////////////////////////////
//
// WARNING : NEVER DIRECTLY INCLUDE THIS FILE!
// INCLUDE 'ggo_math.h' INSTEAD.
//
//////////////////////////////////////////////////////////////////

namespace ggo
{
  template <typename data_t>
  class segment final : public movable_shape2d_abc<data_t>,
                        public rotatable_shape2d_abc<data_t>,
                        public distancable_shape2d_abc<data_t>
  {
  public:

                              segment() {};
                              segment(const ggo::pos2<data_t> & p1, const ggo::pos2<data_t> & p2) : _p1(p1), _p2(p2) {};
                              segment(data_t x1, data_t y1, data_t x2, data_t y2) : _p1(x1, y1), _p2(x2, y2) {};

    const ggo::pos2<data_t> & p1() const { return _p1; };
    const ggo::pos2<data_t> & p2() const { return _p2; };

    ggo::pos2<data_t> &		    p1() { return _p1; };
    ggo::pos2<data_t> &		    p2() { return _p2; };

    data_t					          get_hypot() const;
    data_t					          get_length() const;
  
    data_t                    hypot_to_point(data_t x, data_t y) const;
    data_t                    hypot_to_point(const ggo::pos2<data_t> & p) const { return hypot_to_point(p.x(), p.y()); };
    data_t                    dist_to_point(const ggo::pos2<data_t> & p) const { return dist_to_point(p.x(), p.y()); };
  
    data_t                    hypot_to_segment(data_t x_from, data_t y_from, data_t x_to, data_t y_to) const;
    data_t                    hypot_to_segment(const ggo::pos2<data_t> & p1, const ggo::pos2<data_t> & p2) const;
    data_t                    hypot_to_segment(const segment<data_t> & segment) const;
    data_t                    dist_to_segment(data_t x_from, data_t y_from, data_t x_to, data_t y_to) const;
    data_t                    dist_to_segment(const segment<data_t> & segment) const;
  
    bool				              same_side(data_t x1, data_t y1, data_t x2, data_t y2) const;
    bool				              intersect_segment(const segment<data_t> & segment) const;
    bool				              intersect_segment(const segment<data_t> & segment, ggo::pos2<data_t> & intersect) const;
    bool                      intersect_horizontal_segment(data_t x_inf, data_t x_sup, data_t y) const;
    bool                      intersect_vertical_segment(data_t x, data_t y_inf, data_t y_sup) const;

    // Line equation is ax+by+c=0.
    void				              get_line(data_t & a, data_t & b, data_t & c) const;

    // Interfaces.
    void	                    move(data_t dx, data_t dy) override { _p1.move(dx, dy); _p2.move(dx, dy); };
    void	                    rotate(data_t angle, const ggo::pos2<data_t> & center) override { _p1 = ggo::rotate(_p1, center, angle); _p2 = ggo::rotate(_p2, center, angle); };
    data_t                    dist_to_point(data_t x, data_t y) const override;

  private:

    ggo::pos2<data_t>	_p1;
    ggo::pos2<data_t>	_p2;
  };
}

namespace ggo
{
  using segment_int     = segment<int>;
  using segment_float   = segment<float>;
  using segment_double  = segment<double>;
}

namespace ggo
{
  template <typename data_t>
  std::ostream &	operator<<(std::ostream & os, const segment<data_t> & segment)
  {
    os << segment.p1() << ' ' << segment.p2();
    return os;
  }

  template <typename data_t>
  std::istream &	operator>>(std::istream & is, segment<data_t> & segment)
  {
    is >> segment.p1() >> segment.p2();
    return is;
  }
}
