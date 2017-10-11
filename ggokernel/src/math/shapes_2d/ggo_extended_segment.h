//////////////////////////////////////////////////////////////////
//
// WARNING : NEVER DIRECTLY INCLUDE THIS FILE!
// INCLUDE 'ggo_math.h' INSTEAD.
//
//////////////////////////////////////////////////////////////////

namespace ggo
{
  template <typename data_t>
  class extended_segment final : public paintable_shape2d_abc<data_t>,
                                 public distancable_shape2d_abc<data_t>,
                                 public movable_shape2d_abc<data_t>,
                                 public rotatable_shape2d_abc<data_t>
  {
  public:

    using type = data_t;

                              extended_segment(const ggo::pos2<data_t> & p1, const ggo::pos2<data_t> & p2, data_t width);
          
    float                     width() const { return _width; }
    float &                   width() { return _width; }
          
    const ggo::pos2<data_t> & p1() const { return _p1; }
    ggo::pos2<data_t> &       p1() { return _p1; }
          
    const ggo::pos2<data_t> & p2() const { return _p2; }
    ggo::pos2<data_t> &       p2() { return _p2; }
          
    // Interfaces.        
    void				              move(const ggo::vec2<data_t> & m) override { _p1 += m; _p2 += m; }
    void				              rotate(data_t angle, const ggo::pos2<data_t> & center) override { _p1 = ggo::rotate(_p1,center, angle); _p2 = ggo::rotate(_p2, center, angle); }
    data_t                    dist_to_point(const ggo::pos2<data_t> & p) const override;
    bool	                    is_point_inside(const ggo::pos2<data_t> & p) const override;
    rect_data<data_t>         get_bounding_rect() const override;
    rect_intersection         get_rect_intersection(const rect_data<data_t> & rect_data) const override;

  private:

    ggo::pos2<data_t>  _p1;
    ggo::pos2<data_t>  _p2;
    data_t             _width;
  };
}

namespace ggo
{
  using extended_segment_float    = extended_segment<float>;
}

namespace ggo
{
  template <typename data_t>
  std::ostream & operator<<(std::ostream & os, const ggo::extended_segment<data_t> & s)
  {
    os << "(" << s.p1() << ", " << s.p2() << ", " << s.width() << ")";
    return os;
  }
}

