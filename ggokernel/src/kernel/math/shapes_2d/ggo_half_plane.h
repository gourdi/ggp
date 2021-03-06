//////////////////////////////////////////////////////////////////
//
// WARNING : NEVER DIRECTLY INCLUDE THIS FILE!
// INCLUDE 'ggo_math.h' INSTEAD.
//
//////////////////////////////////////////////////////////////////

namespace ggo
{
  template <typename data_t>
  class half_plane final : public samplable_shape2d_abc<data_t>,
                           public distancable_shape2d_abc<data_t>
  {
  public:

    using distancable_shape2d_abc<data_t>::dist_to_point;

                              // The normal points towards the outside of the half-plane.
                              half_plane(const ggo::pos2<data_t> & normal, data_t dist_to_origin) : _normal(normalize(normal)), _dist_to_origin(dist_to_origin) {}

    bool                      is_point_inside(const ggo::pos2<data_t> & p) const override;
    data_t                    dist_to_point(const ggo::pos2<data_t> & p) const override;

    const ggo::pos2<data_t> & normal() const { return _normal; }
    float                     dist_to_origin() const { return _dist_to_origin; }

  private:

    const ggo::pos2<data_t> _normal;
    const float             _dist_to_origin;
  };
}

namespace ggo
{
  using half_plane_f = half_plane<float>;
  using half_plane_d = half_plane<double>;
}

