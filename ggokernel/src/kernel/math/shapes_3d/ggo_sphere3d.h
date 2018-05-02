//////////////////////////////////////////////////////////////
namespace ggo
{
  template<typename data_t>
  class line3d;
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename data_t>
  class sphere3d : public raytracable_shape3d_abc<data_t>, public influence_shape3d_abc<data_t>
  {
  public:

                              sphere3d() : _center(0, 0, 0), _radius(1) {};
                              sphere3d(const ggo::pos3<data_t> & center, data_t radius) : _center(center), _radius(radius) {};

    bool                      intersect_ray_fast(const ggo::ray3d<data_t> & ray) const;
    bool                      intersect_line(const ggo::line3d<data_t> & line, data_t & dist_inf, data_t & dist_sup) const;

    std::vector<data_t>       intersect_ray(const ggo::ray3d<data_t> & ray) const override;
    data_t                    hypot_to_center(const ggo::pos3<data_t> & p) const override { return ggo::hypot(_center, p); }
    data_t                    get_influence_hypot() const override { return _radius * _radius; }
    
    bool                      intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const override;
    bool                      is_convex() const override { return true; }

    box3d_data<data_t>                get_bounding_box() const;
    std::optional<box3d_data<data_t>> get_bounding_box(const ggo::basis3d<data_t> & basis) const override;

    ggo::pos3<data_t> &       center() { return _center; }
    const ggo::pos3<data_t> & center() const { return _center; }
      
    data_t &                  radius() { return _radius; }
    const data_t &            radius() const { return _radius; }
    
    bool                      is_point_inside(data_t x, data_t y, data_t z) const;
    bool                      is_point_inside(const ggo::pos3<data_t> & p) const;

    bool                      operator==(const ggo::sphere3d<data_t> & s) const { return _center == s._center && _radius == s._radius; }
    bool                      operator!=(const ggo::sphere3d<data_t> & s) const { return !operator==(s); }
  
  private:

    ggo::pos3<data_t> _center;
    data_t            _radius;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  using sphere3d_double   = sphere3d<double>;
  using sphere3d_float    = sphere3d<float>;
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename data_t>
  data_t distance(const ggo::sphere3d<data_t> & sphere1, const ggo::sphere3d<data_t> & sphere2)
  {
    return std::max<data_t>(0, ggo::distance(sphere1.center(), sphere2.center()) - sphere1.radius() - sphere2.radius());
  }

  template <typename data_t>
  sphere3d<data_t> get_union(const sphere3d<data_t> & sphere1, const sphere3d<data_t> & sphere2);

  template <typename data_t>
  std::ostream & operator<<(std::ostream & os, const ggo::sphere3d<data_t> & sphere);
}