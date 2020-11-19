//////////////////////////////////////////////////////////////////
//
// WARNING : NEVER DIRECTLY INCLUDE THIS FILE!
// INCLUDE 'ggo_math.h' INSTEAD.
//
//////////////////////////////////////////////////////////////////

namespace ggo
{
  template <typename scalar_t_>
  class capsule final : public paintable_shape2d_abc<scalar_t_>,
                        public distancable_shape2d_abc<scalar_t_>,
                        public movable_shape2d_abc<scalar_t_>,
                        public rotatable_shape2d_abc<scalar_t_>
  {
  public:

    using scalar_t = scalar_t_;

                                capsule(const ggo::pos2<scalar_t> & p1, const ggo::pos2<scalar_t> & p2, scalar_t width);
          
    float                       width() const { return _width; }
    float &                     width() { return _width; }
          
    const ggo::pos2<scalar_t> & p1() const { return _p1; }
    ggo::pos2<scalar_t> &       p1() { return _p1; }
          
    const ggo::pos2<scalar_t> & p2() const { return _p2; }
    ggo::pos2<scalar_t> &       p2() { return _p2; }
          
    // Interfaces.        
    void				                move(const ggo::vec2<scalar_t> & m) override { _p1 += m; _p2 += m; }
    void				                rotate(scalar_t angle, const ggo::pos2<scalar_t> & center) override { _p1 = ggo::rotate(_p1,center, angle); _p2 = ggo::rotate(_p2, center, angle); }
    scalar_t                    dist_to_point(const ggo::pos2<scalar_t> & p) const override;
    bool	                      is_point_inside(const ggo::pos2<scalar_t> & p) const override;
    rect_data<scalar_t>         get_bounding_rect() const override;
    rect_intersection           get_rect_intersection(const rect_data<scalar_t> & rect_data) const override;

  private:

    ggo::pos2<scalar_t>  _p1;
    ggo::pos2<scalar_t>  _p2;
    scalar_t             _width;
  };
}

namespace ggo
{
  using capsule_f = capsule<float>;
}

namespace ggo
{
  template <typename scalar_t>
  std::ostream & operator<<(std::ostream & os, const ggo::capsule<scalar_t> & s)
  {
    os << "(" << s.p1() << ", " << s.p2() << ", " << s.width() << ")";
    return os;
  }
}

