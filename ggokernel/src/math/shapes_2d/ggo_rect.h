//////////////////////////////////////////////////////////////////
//
// WARNING : NEVER DIRECTLY INCLUDE THIS FILE!
// INCLUDE 'LiwMath.h' INSTEAD.
//
//////////////////////////////////////////////////////////////////

namespace ggo
{
  template <typename T>
  class rect : public paintable_shape2d_abc<T>,
               public distancable_shape2d_abc<T>,
               public movable_shape2d_abc<T>
  {
  public:

    using ggo::samplable_shape2d_abc<T>::is_point_inside;
    using ggo::distancable_shape2d_abc<T>::dist_to_point;

                      rect(const rect<T> & rect);
                      rect(const set2<T> & p1, const set2<T> & p2);
                      rect(T left, T bottom, T width, T height);
                      rect(const rect_data<T> & rect_data);
                      
                      // Conversion operator.
                      operator rect_data<T>() const { return _rect_data; }
                          
    T					        left() const { return _rect_data._pos.x(); }
    T					        bottom() const { return _rect_data._pos.y(); }
    T					        width() const { return _rect_data._width; }
    T					        height() const { return _rect_data._height; }

    T & 			        left() { return _rect_data._pos.x(); }
    T &				        bottom() { return _rect_data._pos.y(); }
    T &				        width() { return _rect_data._width; }
    T & 			        height() { return _rect_data._height; }

    T                 right() const { return _rect_data._pos.x() + _rect_data._width; }
    T                 top() const { return _rect_data._pos.y() + _rect_data._height; }

    ggo::set2<T>      center() const { return ggo::set2<T>(_rect_data._pos.x() + _rect_data._pos._width / 2, _rect_data._pos.y() + _rect_data._pos._height / 2); }
                
    void              inflate(T extent);
    void              inflate(T horz_extent, T vert_extent);
  
    // Interfaces.  
    void				      move(T dx, T dy) override { _rect_data._pos.move(dx, dy); }
    T                 dist_to_point(T x, T y) const override;
    bool	            is_point_inside(T x, T y) const override;
    rect_data<T>      get_bounding_rect() const override { return _rect_data; }
    rect_intersection get_rect_intersection(const rect_data<T> & rect_data) const override;

  public:
      
    static rect<T>    from_left_right_bottom_top(T left, T right, T bottom, T top);
    static rect<T>    from_union(const rect<T> & rect1, const rect<T> & rect2);

  private:

    rect_data<T>  _rect_data;
  };
}

namespace ggo
{
  using rect_float    = rect<float>;
  using rect_double   = rect<double>;
}
