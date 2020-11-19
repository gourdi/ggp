//////////////////////////////////////////////////////////////////
//
// WARNING : NEVER DIRECTLY INCLUDE THIS FILE!
// INCLUDE 'ggo_shapes2d.h' INSTEAD.
//
//////////////////////////////////////////////////////////////////

namespace ggo
{
  template <typename scalar_t_>
  class polygon2d final : public paintable_shape2d_abc<scalar_t_>,
                          public distancable_shape2d_abc<scalar_t_>,
                          public movable_shape2d_abc<scalar_t_>,
                          public rotatable_shape2d_abc<scalar_t_>
  {
  public:

    using scalar_t = scalar_t_;
    using samplable_shape2d_abc<scalar_t>::is_point_inside;

                                polygon2d() {}
                                polygon2d(std::initializer_list<ggo::pos2<scalar_t>> points) { for (const auto & p : points) { _points.push_back(p); } }
                                polygon2d(std::vector<ggo::pos2<scalar_t>> points) : _points(std::move(points)) {}
                                polygon2d(int capacity) { _points.reserve(capacity); } 
                                polygon2d(const polygon2d & polygon) = default;      
        
    void				                add_point(scalar_t x, scalar_t y) { add_point(ggo::pos2<scalar_t>(x, y)); }
    void				                add_point(const ggo::pos2<scalar_t> & point) { _points.push_back(point); }

    template<typename First, typename... Rest>
    void                        add_points(const First & point, Rest... rest) { add_point(point); add_points(rest...); }
    void                        add_points(const ggo::pos2<scalar_t> & point) { _points.push_back(point); }       
         
    int					                get_points_count() const { return static_cast<int>(_points.size()); }
    ggo::pos2<scalar_t> &		    get_point(int i) { return _points[i]; }
    const ggo::pos2<scalar_t> & get_point(int i) const { return _points[i]; }

    const ggo::pos2<scalar_t> * cbegin() const { return _points.data(); }
    const ggo::pos2<scalar_t> * cend() const { return _points.data() + _points.size(); }
    const ggo::pos2<scalar_t> * begin() const { return _points.data(); }
    const ggo::pos2<scalar_t> * end() const { return _points.data() + _points.size(); }
    ggo::pos2<scalar_t> *       begin() { return _points.data(); }
    ggo::pos2<scalar_t> *       end() { return _points.data() + _points.size(); }

    const ggo::pos2<scalar_t> & back() const { return _points.back(); }
    ggo::pos2<scalar_t> &       back() { return _points.back(); }
    void                        pop_back() { return _points.pop_back(); }

    void                        set_points(std::vector<ggo::pos2<scalar_t>> points) { _points = std::move(points); }
          
    bool                        empty() const { return _points.empty(); }
    void				                clear() { _points.clear(); };
          
    scalar_t					          dist_to_segment(const segment<scalar_t> & segment) const;
    scalar_t					          dist_to_segment(const ggo::pos2<scalar_t> & p1, const ggo::pos2<scalar_t> & p2) const;
          
    // Interfaces.        
    void	                      move(const ggo::pos2<scalar_t> & m) override;
    void	                      rotate(scalar_t angle, const ggo::pos2<scalar_t> & center) override;
    scalar_t                    dist_to_point(const ggo::pos2<scalar_t> & p) const override;
    bool	                      is_point_inside(const ggo::pos2<scalar_t> & p) const override;
    rect_data<scalar_t>         get_bounding_rect() const override;
    rect_intersection           get_rect_intersection(const rect_data<scalar_t> & rect_data) const override;

  public:

    static  polygon2d<scalar_t>	create_oriented_box(const ggo::pos2<scalar_t> & center, const ggo::vec2<scalar_t> & direction, scalar_t size1, scalar_t size2);
    static  polygon2d<scalar_t> create_axis_aligned_box(scalar_t left, scalar_t right, scalar_t top, scalar_t bottom);

  private:

    std::vector<ggo::pos2<scalar_t>>	_points;
  };
}

namespace ggo
{
  using polygon2d_f = polygon2d<float>;
  using polygon2d_d = polygon2d<double>;
}
