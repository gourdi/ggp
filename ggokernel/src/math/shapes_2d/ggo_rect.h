//////////////////////////////////////////////////////////////////
//
// WARNING : NEVER DIRECTLY INCLUDE THIS FILE!
// INCLUDE 'LiwMath.h' INSTEAD.
//
//////////////////////////////////////////////////////////////////

namespace ggo
{
  template <typename data_t>
  class rect : public paintable_shape2d_abc<data_t>,
               public distancable_shape2d_abc<data_t>,
               public movable_shape2d_abc<data_t>
  {
  public:

    using ggo::samplable_shape2d_abc<data_t>::is_point_inside;
    using ggo::distancable_shape2d_abc<data_t>::dist_to_point;

                      rect();
                      rect(const rect<data_t> & rect);
                      rect(const pos2<data_t> & p1, const pos2<data_t> & p2);
                      rect(data_t left, data_t bottom, data_t width, data_t height);
                      rect(const rect_data<data_t> & rect_data);
                      
                      // Conversion operator.
                      operator rect_data<data_t>() const { return _rect_data; }
                          
    data_t					  left() const { return _rect_data._pos.template get<0>(); }
    data_t					  bottom() const { return _rect_data._pos.template get<1>(); }
    data_t					  width() const { return _rect_data._width; }
    data_t					  height() const { return _rect_data._height; }

    data_t & 			    left() { return _rect_data._pos.template get<0>(); }
    data_t &				  bottom() { return _rect_data._pos.template get<1>(); }
    data_t &				  width() { return _rect_data._width; }
    data_t & 			    height() { return _rect_data._height; }

    data_t            right() const { return _rect_data._pos.template get<0>() + _rect_data._width; }
    data_t            top() const { return _rect_data._pos.template get<1>() + _rect_data._height; }

    ggo::pos2<data_t> center() const { return ggo::pos2<data_t>(_rect_data._pos.template get<0>() + _rect_data._pos._width / 2, _rect_data._pos.template get<1>() + _rect_data._pos._height / 2); }
                
    void              inflate(data_t extent);
    void              inflate(data_t horz_extent, data_t vert_extent);
    void              extend(const ggo::pos2f & p);
  
    // Interfaces.  
    void				      move(data_t dx, data_t dy) override { _rect_data._pos.move(dx, dy); }
    data_t            dist_to_point(data_t x, data_t y) const override;
    bool	            is_point_inside(data_t x, data_t y) const override;
    rect_data<data_t> get_bounding_rect() const override { return _rect_data; }
    rect_intersection get_rect_intersection(const rect_data<data_t> & rect_data) const override;

  public:
      
    static rect<data_t> from_left_right_bottom_top(data_t left, data_t right, data_t bottom, data_t top);
    static rect<data_t> from_union(const rect<data_t> & rect1, const rect<data_t> & rect2);

  private:

    rect_data<data_t>  _rect_data;
  };
}

namespace ggo
{
  using rect_float    = rect<float>;
  using rect_double   = rect<double>;
}
