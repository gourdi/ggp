//////////////////////////////////////////////////////////////////
//
// WARNING : NEVER DIRECTLY INCLUDE THIS FILE!
// INCLUDE 'ggo_math.h' INSTEAD.
//
//////////////////////////////////////////////////////////////////

namespace ggo
{
  template <typename data_type>
  class disc final : public paintable_shape2d_abc<data_type>,
                     public distancable_shape2d_abc<data_type>,
                     public affine_shape2d_abc<data_type>
  {
  public:
    
    using data_t = data_type;
    using samplable_shape2d_abc<data_t>::is_point_inside;
    using distancable_shape2d_abc<data_t>::dist_to_point;

                              disc() {}
                              disc(data_t center_x, data_t center_y, data_t radius) : _center(center_x,center_y), _radius(radius) {}
                              disc(const ggo::pos2<data_t> & center, data_t radius) : _center(center), _radius(radius) {}
                              disc(const ggo::pos2<data_t> & p1, const ggo::pos2<data_t> & p2);
           
    ggo::pos2<data_t> &		    center() { return _center; }
    const ggo::pos2<data_t> & center() const { return _center; }
           
    data_t &					        radius() { return _radius; }
    const data_t &			      radius() const { return _radius; }
           
    data_t					          get_radius() const { return _radius; }
    void				              set_radius(data_t radius) { _radius = radius; }
    void				              set_center(const ggo::pos2<data_t> & center) { _center = center; }
    void				              set_center(data_t x, data_t y) { _center.template get<0>() = x; _center.template get<1>() = y; }
           
    // Interfaces.
    ggo::pos2<data_t>         get_center() const override { return _center; }
    void				              move(const ggo::vec2<data_t> & m) override { _center += m; }
    void				              rotate(data_t angle, const ggo::pos2<data_t> & center) override { _center = ggo::rotate(_center, center, angle); }
    data_t                    dist_to_point(const ggo::pos2<data_t> & p) const override;
    bool	                    is_point_inside(const ggo::pos2<data_t> & p) const override;
    rect_data<data_t>         get_bounding_rect() const override;
    rect_intersection         get_rect_intersection(const rect_data<data_t> & rect_data) const override;

  private:

    ggo::pos2<data_t> _center;
    data_t			      _radius;
  };
}

namespace ggo
{
  using disc_float  = disc<float>;
  using disc_double = disc<double>;
}

namespace ggo
{
  template <typename data_t, bool orthonormal, bool cross_product_up>
  ggo::disc<data_t> from_local_to_world(const ggo::disc<data_t> & d, const orthogonal_basis2d<data_t, orthonormal, cross_product_up> & basis);
}
