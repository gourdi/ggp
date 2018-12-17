//////////////////////////////////////////////////////////////////
//
// WARNING : NEVER DIRECTLY INCLUDE THIS FILE!
// INCLUDE 'ggo_shapes2d.h' INSTEAD.
//
//////////////////////////////////////////////////////////////////

#include <memory>

namespace ggo
{
  enum class boolean_mode
  {
    UNION,
    INTERSECTION,
    DIFFERENCE
  };

  template <typename data_type, boolean_mode mode = boolean_mode::UNION>
  class multi_shape final : public paintable_shape2d_abc<data_type>
  {
  public:

    using data_t = data_type;

                      multi_shape() {}
          
    void              add_shape(std::shared_ptr<paintable_shape2d_abc<data_t>> shape) { _shapes.push_back(shape); }
    
    template<typename First, typename... Rest>
    void              add_shapes(const First & shape, Rest... rest) { add_shape(shape); add_shapes(rest...); }  
    void              add_shapes() { }
          
    // Interfaces.
    bool	            is_point_inside(const ggo::pos2<data_t> & p) const override;
    rect_data<data_t> get_bounding_rect() const override;
    rect_intersection get_rect_intersection(const rect_data<data_t> & rect_data) const override;

  private:

    bool	            is_point_inside_union(const ggo::pos2<data_t> & p) const;
    bool	            is_point_inside_intersection(const ggo::pos2<data_t> & p) const;
    bool	            is_point_inside_difference(const ggo::pos2<data_t> & p) const;

    rect_data<data_t> get_bounding_rect_union() const;
    rect_data<data_t> get_bounding_rect_intersection() const;
    rect_data<data_t> get_bounding_rect_difference() const;

    rect_intersection get_rect_intersection_union(const rect_data<data_t> & rect_data) const;
    rect_intersection get_rect_intersection_intersection(const rect_data<data_t> & rect_data) const;
    rect_intersection get_rect_intersection_difference(const rect_data<data_t> & rect_data) const;

  private:

    std::vector<std::shared_ptr<paintable_shape2d_abc<data_t>>> _shapes;
  };
}

namespace ggo
{
  using multi_shape_f = multi_shape<float>;
  using multi_shape_d = multi_shape<double>;
}
