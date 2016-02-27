//////////////////////////////////////////////////////////////////
//
// WARNING : NEVER DIRECTLY INCLUDE THIS FILE!
// INCLUDE 'ggo_shapes2d.h' INSTEAD.
//
//////////////////////////////////////////////////////////////////

namespace ggo
{
  template <typename T>
  class polygon2d : public paintable_shape2d_abc<T>,
                    public distancable_shape2d_abc<T>,
                    public movable_shape2d_abc<T>,
                    public rotatable_shape2d_abc<T>
  {
  public:

                          polygon2d() {}
                          polygon2d(int capacity) { _points.reserve(capacity); } 
                          polygon2d(const polygon2d & polygon) = default;      
        
    void				          add_point(T x, T y) { add_point(ggo::set2<T>(x, y)); }
    void				          add_point(const ggo::set2<T> & point) { _points.push_back(point); }

    template<typename First, typename... Rest>
    void                  add_points(const First & point, Rest... rest) { add_point(point); add_points(rest...); }
    void                  add_points(const ggo::set2<T> & point) { _points.push_back(point); }       
         
    int					          get_points_count() const { return static_cast<int>(_points.size()); }
    ggo::set2<T> &		    get_point(int i) { return _points[i]; }
    const ggo::set2<T> &  get_point(int i) const { return _points[i]; }
          
    void				          clear() { _points.clear(); };
          
    T					            dist_to_segment(const segment<T> & segment) const;
    T					            dist_to_segment(const ggo::set2<T> & p1, const ggo::set2<T> & p2) const;
    T					            dist_to_segment(T x_from, T y_from, T x_to, T y_to) const;
    T					            dist_to_point(const ggo::set2<T> p) const { return dist_to_point(p.x(), p.y()); };
          
    // Interfaces.        
    void	                move(T dx, T dy) override;
    void	                rotate(T angle) override;
    T                     dist_to_point(T x, T y) const override;
    bool	                is_point_inside(T x, T y) const override;
    rect_data<T>          get_bounding_rect() const override;
    rect_intersection     get_rect_intersection(const rect_data<T> & rect_data) const override;

  public:

    static  void	create_oriented_box(const ggo::set2<T> & center, const ggo::set2<T> & direction, T size1, T size2, polygon2d<T> & polygon);
    static  void  create_axis_aligned_box(T left, T right, T top, T bottom, polygon2d<T> & polygon);

  private:

    std::vector<ggo::set2<T>>	_points;
  };
}

namespace ggo
{
  using polygon2d_float   = polygon2d<float>;
  using polygon2d_double  = polygon2d<double>;
}
