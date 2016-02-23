//////////////////////////////////////////////////////////////////
//
// WARNING : NEVER DIRECTLY INCLUDE THIS FILE!
// INCLUDE 'ggo_math.h' INSTEAD.
//
//////////////////////////////////////////////////////////////////

namespace ggo
{
  template <typename T>
  class half_plane : public samplable_shape2d_abc<T>,
                     public distancable_shape2d_abc<T>
  {
  public:

    using distancable_shape2d_abc<T>::dist_to_point;

                          // The normal points towards the outside of the half-plane.
                          half_plane(const ggo::set2<T> & normal, T dist_to_origin) : _normal(normal.get_normalized()), _dist_to_origin(dist_to_origin) {}

    bool                  is_point_inside(T x, T y) const override;
    T                     dist_to_point(T x, T y) const override;

    const ggo::set2<T> &  normal() const { return _normal; }
    float                 dist_to_origin() const { return _dist_to_origin; }

  private:

    const ggo::set2<T>  _normal;
    const float         _dist_to_origin;
  };
}
