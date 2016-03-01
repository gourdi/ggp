//////////////////////////////////////////////////////////////////
//
// WARNING : NEVER DIRECTLY INCLUDE THIS FILE!
// INCLUDE 'ggo_math.h' INSTEAD.
//
//////////////////////////////////////////////////////////////////

namespace ggo
{
  template <typename T>
  class triangle2d : public paintable_shape2d_abc<T>,
                     public movable_shape2d_abc<T>
  {
  public:

    using samplable_shape2d_abc<T>::is_point_inside;
  
                      triangle2d(ggo::set2<T> v1, ggo::set2<T> v2, ggo::set2<T> v3) : _v1(v1), _v2(v2), _v3(v3) {}

    // Interfaces.
    void	            move(T dx, T dy) override;
    bool	            is_point_inside(T x, T y) const override;
    rect_data<T>      get_bounding_rect() const override;
    rect_intersection get_rect_intersection(const rect_data<T> & rect_data) const override;

  private:

    ggo::set2<T> _v1;
    ggo::set2<T> _v2;
    ggo::set2<T> _v3;
  };
}

namespace ggo
{
  using triangle2d_float  = triangle2d<float>;
  using triangle2d_double = triangle2d<double>;
}

