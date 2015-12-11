//////////////////////////////////////////////////////////////////
//
// WARNING : NEVER DIRECTLY INCLUDE THIS FILE!
// INCLUDE 'ggo_math.h' INSTEAD.
//
//////////////////////////////////////////////////////////////////

namespace ggo
{
  template <typename T>
  class extended_segment: public paintable_shape2d_abc<T>,
                          public distancable_shape2d_abc<T>,
                          public movable_shape2d_abc<T>,
                          public rotatable_shape2d_abc<T>
  {
  public:

                          extended_segment(const ggo::set2<T> & p1, const ggo::set2<T> & p2, T width);
          
    float                 width() const { return _width; }
    float &               width() { return _width; }
          
    const ggo::set2<T> &  p1() const { return _p1; }
    ggo::set2<T> &        p1() { return _p1; }
          
    const ggo::set2<T> &  p2() const { return _p2; }
    ggo::set2<T> &        p2() { return _p2; }
          
    // Interfaces.        
    void				          move(T dx, T dy) override { _p1.move(dx, dy); _p2.move(dx, dy); }
    void				          rotate(T angle, const ggo::set2<T> & center) override { _p1.rotate(angle, center); _p2.rotate(angle, center); }
    T                     dist_to_point(T x, T y) const override;
    bool	                is_point_inside(T x, T y) const override;
    rect_data<T>          get_bounding_rect() const override;
    rect_intersection     get_rect_intersection(const rect_data<T> & rect_data) const override;

  private:

    ggo::set2<T>  _p1;
    ggo::set2<T>  _p2;
    T             _width;
  };
}

namespace ggo
{
  using extended_segment_float    = extended_segment<float>;
}

