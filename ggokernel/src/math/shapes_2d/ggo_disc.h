//////////////////////////////////////////////////////////////////
//
// WARNING : NEVER DIRECTLY INCLUDE THIS FILE!
// INCLUDE 'ggo_math.h' INSTEAD.
//
//////////////////////////////////////////////////////////////////

namespace ggo
{
  template <typename T>
  class disc : public paintable_shape2d_abc<T>,
               public distancable_shape2d_abc<T>,
               public affine_shape2d_abc<T>
  {
  public:
    
    using samplable_shape2d_abc<T>::is_point_inside;
    using distancable_shape2d_abc<T>::dist_to_point;

                              disc() {}
                              disc(T center_x, T center_y, T radius) : _center(center_x,center_y), _radius(radius) {}
                              disc(const ggo::set2<T> & center, T radius ) :_center(center), _radius(radius) {}
           
    ggo::set2<T> &		        center() { return _center; }
    const ggo::set2<T> &      center() const { return _center; }
           
    T &					              radius() { return _radius; }
    const T &			            radius() const { return _radius; }
           
    T					                get_radius() const { return _radius; }
    void				              set_radius(T radius) { _radius = radius; }
    void				              set_center(const ggo::set2<T> & center) { _center = center; }
    void				              set_center(T x, T y) { _center.x() = x; _center.y() = y; }
           
    bool				              segment_intersect_border(T x_from, T y_from, T x_to, T y_to) const;
           
    // Interfaces.
    ggo::set2<T>              get_center() const override { return _center; }
    void				              move(T dx, T dy) override { _center.move(dx, dy); }
    void				              rotate(T angle, const ggo::set2<T> & center) override { _center.rotate(angle, center); }
    T                         dist_to_point(T x, T y) const override;
    bool	                    is_point_inside(T x, T y) const override;
    rect_data<T>              get_bounding_rect() const override;
    rect_intersection         get_rect_intersection(const rect_data<T> & rect_data) const override;

  private:

    ggo::set2<T>  _center;
    T			        _radius;
  };
}

namespace ggo
{
  using disc_float  = disc<float>;
  using disc_double = disc<double>;
}
