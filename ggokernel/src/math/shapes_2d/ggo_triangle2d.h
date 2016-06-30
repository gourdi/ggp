//////////////////////////////////////////////////////////////////
//
// WARNING : NEVER DIRECTLY INCLUDE THIS FILE!
// INCLUDE 'ggo_math.h' INSTEAD.
//
//////////////////////////////////////////////////////////////////

namespace ggo
{
  template <typename data_t>
  class triangle2d : public paintable_shape2d_abc<data_t>,
                     public movable_shape2d_abc<data_t>
  {
  public:

    using samplable_shape2d_abc<data_t>::is_point_inside;
  
                      triangle2d(ggo::pos2<data_t> v1, ggo::pos2<data_t> v2, ggo::pos2<data_t> v3) : _v1(v1), _v2(v2), _v3(v3) {}

    // Interfaces.
    void	            move(data_t dx, data_t dy) override;
    bool	            is_point_inside(data_t x, data_t y) const override;
    rect_data<data_t> get_bounding_rect() const override;
    rect_intersection get_rect_intersection(const rect_data<data_t> & rect_data) const override;

  private:

    ggo::pos2<data_t> _v1;
    ggo::pos2<data_t> _v2;
    ggo::pos2<data_t> _v3;
  };
}

namespace ggo
{
  using triangle2d_float  = triangle2d<float>;
  using triangle2d_double = triangle2d<double>;
}

