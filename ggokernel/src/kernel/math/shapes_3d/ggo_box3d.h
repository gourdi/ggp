//////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename data_t>
  class box3d : public raytracable_shape3d_abc<data_t>
  {
  public:

                      box3d(data_t x_min, data_t x_max, data_t y_min, data_t y_max, data_t z_min, data_t z_max);
                      box3d(const box3d_data<data_t> & data);
                      box3d(const ggo::pos3<data_t> & p);
  
    data_t			      x_min() const { return _data._x_min; }
    data_t			      x_max() const { return _data._x_max; }
    data_t			      y_min() const { return _data._y_min; }
    data_t			      y_max() const { return _data._y_max; }
    data_t			      z_min() const { return _data._z_min; }
    data_t			      z_max() const { return _data._z_max; }
  
    data_t &	        x_min() { return _data._x_min; }
    data_t &	        x_max() { return _data._x_max; }
    data_t &	        y_min() { return _data._y_min; }
    data_t &	        y_max() { return _data._y_max; }
    data_t &	        z_min() { return _data._z_min; }
    data_t &	        z_max() { return _data._z_max; }

    const auto &      data() const { return _data; }
    auto &            data() { return _data; }
  
    bool              is_point_inside(const ggo::pos3<data_t> & point) const;
  
    bool              intersect_segment(const ggo::pos3<data_t> & pos, const ggo::vec3<data_t> & dir, data_t length) const override;
    bool              intersect_ray(const ggo::ray3d<data_t> & ray) const; // Fast intersection test.
    bool              intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const override;
    bool              is_convex() const override { return true; }

    std::optional<box3d_data<data_t>>  get_bounding_box(const ggo::basis3d<data_t> & basis) const override;
  
    void              get_bounding_sphere(ggo::sphere3d<data_t> & bounding_sphere) const;

    ggo::pos3<data_t> get_center() const { return _data.get_center(); }
    data_t			      get_size_x() const { return _data.get_size_x(); }
    data_t			      get_size_y() const { return _data.get_size_y(); }
    data_t			      get_size_z() const { return _data.get_size_z(); }

  private:

    box3d_data<data_t>  _data;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename data_t>
  bool test_intersection(const box3d<data_t> & b1, const box3d<data_t> & b2);

  template <typename data_t>
  box3d<data_t> get_union(const box3d<data_t> & b1, const box3d<data_t> & b2);

  template <typename data_t>
  box3d<data_t> get_union(const box3d<data_t> & box, const pos3<data_t> & p);

  template <typename data_t>
  std::ostream & operator<<(std::ostream & os, const box3d<data_t> & box);
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  using box3d_f = box3d<float>;
  using box3d_d = box3d<double>;
}

