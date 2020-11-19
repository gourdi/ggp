//////////////////////////////////////////////////////////////////
//
// WARNING : NEVER DIRECTLY INCLUDE THIS FILE!
// INCLUDE 'LiwMath.h' INSTEAD.
//
//////////////////////////////////////////////////////////////////

namespace ggo
{
  template <typename scalar_t_>
  class rect final : public paintable_shape2d_abc<scalar_t_>,
                     public distancable_shape2d_abc<scalar_t_>,
                     public movable_shape2d_abc<scalar_t_>
  {
  public:

    using scalar_t = scalar_t_;
    using ggo::samplable_shape2d_abc<scalar_t>::is_point_inside;
    using ggo::distancable_shape2d_abc<scalar_t>::dist_to_point;

                      rect();
                      rect(const rect<scalar_t> & rect);
                      rect(const rect_data<scalar_t> & rect_data);

    static rect       from_points(const pos2<scalar_t> & p1, const pos2<scalar_t> & p2);
    static rect       from_left_bottom_width_height(scalar_t left, scalar_t bottom, scalar_t width, scalar_t height);
    static rect       from_left_right_bottom_top(scalar_t left, scalar_t right, scalar_t bottom, scalar_t top);
    static rect       from_union(const rect<scalar_t> & rect1, const rect<scalar_t> & rect2);

                      // Conversion operator.
    const rect_data<scalar_t> data() const { return _rect_data; }
                          
    pos2<scalar_t>      pos() const { return _rect_data._pos; }
    scalar_t					  left() const { return _rect_data._pos.x(); }
    scalar_t					  bottom() const { return _rect_data._pos.y(); }
    scalar_t					  width() const { return _rect_data._width; }
    scalar_t					  height() const { return _rect_data._height; }

    scalar_t & 			    left() { return _rect_data._pos.x(); }
    scalar_t &				  bottom() { return _rect_data._pos.y(); }
    scalar_t &				  width() { return _rect_data._width; }
    scalar_t & 			    height() { return _rect_data._height; }

    scalar_t            right() const { return _rect_data._pos.x() + _rect_data._width; }
    scalar_t            top() const { return _rect_data._pos.y() + _rect_data._height; }

    ggo::pos2<scalar_t> center() const { return ggo::pos2<scalar_t>(_rect_data._pos.x() + _rect_data._width / 2, _rect_data._pos.y() + _rect_data._height / 2); }
                
    void                inflate(scalar_t extent);
    void                inflate(scalar_t horz_extent, scalar_t vert_extent);

    void                extend(const ggo::pos2_f & p);
  
    // Interfaces.  
    void				        move(const ggo::vec2<scalar_t> & m) override { _rect_data._pos += m; }
    scalar_t            dist_to_point(const ggo::pos2<scalar_t> & p) const override;
    bool	              is_point_inside(const ggo::pos2<scalar_t> & p) const override;
    rect_data<scalar_t> get_bounding_rect() const override { return _rect_data; }
    rect_intersection   get_rect_intersection(const rect_data<scalar_t> & rect_data) const override;

  private:

    rect_data<scalar_t>  _rect_data;
  };
}

namespace ggo
{
  using rect_f = rect<float>;
  using rect_d = rect<double>;
}
