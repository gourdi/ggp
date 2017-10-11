//////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename data_t>
  class axis_aligned_box3d : public raytracable_shape3d_abc<data_t>
  {
  public:

                      axis_aligned_box3d() {};
                      axis_aligned_box3d(data_t x_min, data_t x_max, data_t y_min, data_t y_max, data_t z_min, data_t z_max);
                      axis_aligned_box3d(const ggo::pos3<data_t> & p);
  
    data_t			      x_min() const { return _x_min; }
    data_t			      x_max() const { return _x_max; }
    data_t			      y_min() const { return _y_min; }
    data_t			      y_max() const { return _y_max; }
    data_t			      z_min() const { return _z_min; }
    data_t			      z_max() const { return _z_max; }
  
    data_t &	        x_min() { return _x_min; }
    data_t &	        x_max() { return _x_max; }
    data_t &	        y_min() { return _y_min; }
    data_t &	        y_max() { return _y_max; }
    data_t &	        z_min() { return _z_min; }
    data_t &	        z_max() { return _z_max; }
  
    bool              is_point_inside(const ggo::pos3<data_t> & point) const;
  
    bool              intersect_ray(const ggo::ray3d<data_t> & ray) const; // Fast intersection test.
    bool              intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const override;
    bool              is_convex() const override { return true; }

    std::optional<axis_aligned_box3d_data<data_t>>  get_bounding_box(const ggo::basis3d<data_t> & basis) const override;
  
    void              get_bounding_sphere(ggo::sphere3d<data_t> & bounding_sphere) const;

    ggo::pos3<data_t> get_center() const;
    data_t			      get_size_x() const { return _x_max - _x_min; }
    data_t			      get_size_y() const { return _y_max - _y_min; }
    data_t			      get_size_z() const { return _z_max - _z_min; }
  
    void              merge_with(const axis_aligned_box3d & box);
    void              merge_with(const ggo::pos3<data_t> & point);

  private:

    data_t	_x_min;
    data_t	_x_max;
    data_t	_y_min;
    data_t	_y_max;
    data_t	_z_min;
    data_t	_z_max;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename data_t>
  bool test_intersection(const axis_aligned_box3d<data_t> & b1, const axis_aligned_box3d<data_t> & b2);

  template <typename data_t>
  std::ostream & operator<<(std::ostream & os, const axis_aligned_box3d<data_t> & box);
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  using aabox3d_float   = axis_aligned_box3d<float>;
  using aabox3d_double  = axis_aligned_box3d<double>;
}

