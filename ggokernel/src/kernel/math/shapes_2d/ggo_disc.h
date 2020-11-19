//////////////////////////////////////////////////////////////////
//
// WARNING : NEVER DIRECTLY INCLUDE THIS FILE!
// INCLUDE 'ggo_math.h' INSTEAD.
//
//////////////////////////////////////////////////////////////////

namespace ggo
{
  template <typename scalar_t_>
  class disc final : public paintable_shape2d_abc<scalar_t_>,
                     public distancable_shape2d_abc<scalar_t_>,
                     public affine_shape2d_abc<scalar_t_>
  {
  public:
    
    using scalar_t = scalar_t_;
    using samplable_shape2d_abc<scalar_t>::is_point_inside;
    using distancable_shape2d_abc<scalar_t>::dist_to_point;

                              disc() {}
                              disc(const ggo::pos2<scalar_t> & center, scalar_t radius) : _center(center), _radius(radius) {}
                              disc(const ggo::pos2<scalar_t> & p1, const ggo::pos2<scalar_t> & p2);
           
    ggo::pos2<scalar_t> &		    center() { return _center; }
    const ggo::pos2<scalar_t> & center() const { return _center; }
           
    scalar_t &					        radius() { return _radius; }
    const scalar_t &			      radius() const { return _radius; }
           
    scalar_t					          get_radius() const { return _radius; }
    void				              set_radius(scalar_t radius) { _radius = radius; }
    void				              set_center(const ggo::pos2<scalar_t> & center) { _center = center; }
           
    // Interfaces.
    ggo::pos2<scalar_t>         get_center() const override { return _center; }
    void				              move(const ggo::vec2<scalar_t> & m) override { _center += m; }
    void				              rotate(scalar_t angle, const ggo::pos2<scalar_t> & center) override { _center = ggo::rotate(_center, center, angle); }
    scalar_t                    dist_to_point(const ggo::pos2<scalar_t> & p) const override;
    bool	                    is_point_inside(const ggo::pos2<scalar_t> & p) const override;
    rect_data<scalar_t>         get_bounding_rect() const override;
    rect_intersection         get_rect_intersection(const rect_data<scalar_t> & rect_data) const override;

  private:

    ggo::pos2<scalar_t> _center;
    scalar_t			      _radius;
  };
}

namespace ggo
{
  using disc_f = disc<float>;
  using disc_d = disc<double>;
}

namespace ggo
{
  template <typename scalar_t, bool orthonormal, bool cross_product_up>
  ggo::disc<scalar_t> from_local_to_world(const ggo::disc<scalar_t> & d, const orthogonal_basis2d<scalar_t, orthonormal, cross_product_up> & basis);
}
