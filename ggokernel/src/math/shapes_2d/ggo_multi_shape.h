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

  template <typename T, boolean_mode mode = boolean_mode::UNION>
  class multi_shape final : public paintable_shape2d_abc<T>
  {
  public:

    using type = T;

                      multi_shape() {}
          
    void              add_shape(std::shared_ptr<paintable_shape2d_abc<T>> shape) { _shapes.push_back(shape); }  
    
    template<typename First, typename... Rest>
    void              add_shapes(const First & shape, Rest... rest) { add_shape(shape); add_shapes(rest...); }  
    void              add_shapes() { }
          
    // Interfaces.
    bool	            is_point_inside(T x, T y) const override;
    rect_data<T>      get_bounding_rect() const override;
    rect_intersection get_rect_intersection(const rect_data<T> & rect_data) const override;

  private:

    bool	            is_point_inside_union(T x, T y) const;
    bool	            is_point_inside_intersection(T x, T y) const;
    bool	            is_point_inside_difference(T x, T y) const;

    rect_data<T>      get_bounding_rect_union() const;
    rect_data<T>      get_bounding_rect_intersection() const;
    rect_data<T>      get_bounding_rect_difference() const;

    rect_intersection get_rect_intersection_union(const rect_data<T> & rect_data) const;
    rect_intersection get_rect_intersection_intersection(const rect_data<T> & rect_data) const;
    rect_intersection get_rect_intersection_difference(const rect_data<T> & rect_data) const;

  private:

    std::vector<std::shared_ptr<paintable_shape2d_abc<T>>> _shapes;
  };
}

namespace ggo
{
  using multi_shape_float   = multi_shape<float>;
  using multi_shape_double  = multi_shape<double>;
}
