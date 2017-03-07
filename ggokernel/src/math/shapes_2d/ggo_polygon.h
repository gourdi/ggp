//////////////////////////////////////////////////////////////////
//
// WARNING : NEVER DIRECTLY INCLUDE THIS FILE!
// INCLUDE 'ggo_shapes2d.h' INSTEAD.
//
//////////////////////////////////////////////////////////////////

namespace ggo
{
  template <typename data_t>
  class polygon2d final : public paintable_shape2d_abc<data_t>,
                          public distancable_shape2d_abc<data_t>,
                          public movable_shape2d_abc<data_t>,
                          public rotatable_shape2d_abc<data_t>
  {
  public:

    using type = data_t;
    using samplable_shape2d_abc<data_t>::is_point_inside;

                              polygon2d() {}
                              polygon2d(std::vector<ggo::pos2<data_t>> points) : _points(std::move(points)) {}
                              polygon2d(int capacity) { _points.reserve(capacity); } 
                              polygon2d(const polygon2d & polygon) = default;      
        
    void				              add_point(data_t x, data_t y) { add_point(ggo::pos2<data_t>(x, y)); }
    void				              add_point(const ggo::pos2<data_t> & point) { _points.push_back(point); }

    template<typename First, typename... Rest>
    void                      add_points(const First & point, Rest... rest) { add_point(point); add_points(rest...); }
    void                      add_points(const ggo::pos2<data_t> & point) { _points.push_back(point); }       
         
    int					              get_points_count() const { return static_cast<int>(_points.size()); }
    ggo::pos2<data_t> &		    get_point(int i) { return _points[i]; }
    const ggo::pos2<data_t> & get_point(int i) const { return _points[i]; }

    const ggo::pos2<data_t> * cbegin() const { return _points.data(); }
    const ggo::pos2<data_t> * cend() const { return _points.data() + _points.size(); }
    const ggo::pos2<data_t> * begin() const { return _points.data(); }
    const ggo::pos2<data_t> * end() const { return _points.data() + _points.size(); }
    ggo::pos2<data_t> *       begin() { return _points.data(); }
    ggo::pos2<data_t> *       end() { return _points.data() + _points.size(); }

    const ggo::pos2<data_t> & back() const { return _points.back(); }
    ggo::pos2<data_t> &       back() { return _points.back(); }
    void                      pop_back() { return _points.pop_back(); }

    void                      set_points(std::vector<ggo::pos2<data_t>> points) { _points = std::move(points); }
          
    bool                      empty() const { return _points.empty(); }
    void				              clear() { _points.clear(); };
          
    data_t					          dist_to_segment(const segment<data_t> & segment) const;
    data_t					          dist_to_segment(const ggo::pos2<data_t> & p1, const ggo::pos2<data_t> & p2) const;
    data_t					          dist_to_segment(data_t x_from, data_t y_from, data_t x_to, data_t y_to) const;
    data_t					          dist_to_point(const ggo::pos2<data_t> p) const { return dist_to_point(p.x(), p.y()); };
          
    // Interfaces.        
    void	                    move(data_t dx, data_t dy) override;
    void	                    rotate(data_t angle, const ggo::pos2<data_t> & center) override;
    data_t                    dist_to_point(data_t x, data_t y) const override;
    bool	                    is_point_inside(data_t x, data_t y) const override;
    rect_data<data_t>         get_bounding_rect() const override;
    rect_intersection         get_rect_intersection(const rect_data<data_t> & rect_data) const override;

  public:

    static  void	create_oriented_box(const ggo::pos2<data_t> & center, const ggo::pos2<data_t> & direction, data_t size1, data_t size2, polygon2d<data_t> & polygon);
    static  void  create_axis_aligned_box(data_t left, data_t right, data_t top, data_t bottom, polygon2d<data_t> & polygon);

  private:

    std::vector<ggo::pos2<data_t>>	_points;
  };
}

namespace ggo
{
  using polygon2d_float   = polygon2d<float>;
  using polygon2d_double  = polygon2d<double>;
}
