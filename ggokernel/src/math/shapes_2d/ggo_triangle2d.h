//////////////////////////////////////////////////////////////////
//
// WARNING : NEVER DIRECTLY INCLUDE THIS FILE!
// INCLUDE 'ggo_math.h' INSTEAD.
//
//////////////////////////////////////////////////////////////////

namespace ggo
{
  template <typename data_type>
  class triangle2d final : public paintable_shape2d_abc<data_type>,
                           public movable_shape2d_abc<data_type>
  {
  public:

    using data_t = data_type;

    using samplable_shape2d_abc<data_t>::is_point_inside;
  
                              triangle2d(ggo::pos2<data_t> v1, ggo::pos2<data_t> v2, ggo::pos2<data_t> v3) : _v1(v1), _v2(v2), _v3(v3) {}

    // Interfaces.
    void	                    move(const ggo::vec2<data_t> & m) override;
    bool	                    is_point_inside(const ggo::pos2<data_t> & p) const override;
    rect_data<data_t>         get_bounding_rect() const override;
    rect_intersection         get_rect_intersection(const rect_data<data_t> & rect_data) const override;

    data_t                    area() const;

    const ggo::pos2<data_t> & v1() const { return _v1; }
    const ggo::pos2<data_t> & v2() const { return _v2; }
    const ggo::pos2<data_t> & v3() const { return _v3; }

    ggo::pos2<data_t> &       v1() { return _v1; }
    ggo::pos2<data_t> &       v2() { return _v2; }
    ggo::pos2<data_t> &       v3() { return _v3; }

    data_t                    angle1() const;
    data_t                    angle2() const;
    data_t                    angle3() const;

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

namespace ggo
{
  enum class triangle_intersection
  {
    triangle1_in_triangle2,
    triangle2_in_triangle1,
    partial_overlap,
    disjoints,
  };

  template <typename data_t>
  triangle_intersection get_triangle_intersection(const triangle2d<data_t> & triangle1, const triangle2d<data_t> & triangle2);
}
