//////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename data_t>
  class sphere3d : public raytracable_shape3d_abc<data_t>
  {
  public:

                              sphere3d() : _center(0, 0, 0), _radius(1) {};
                              sphere3d(const ggo::set3<data_t> & center, data_t radius) : _center(center), _radius(radius) {};

    bool                      intersect_ray(const ggo::ray3d<data_t> & ray) const;
    bool                      intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist_inf, data_t & dist_sup) const;
    
    bool                      intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const override;
    bool                      is_convex() const override { return true; }

    ggo::set3<data_t> &       center() { return _center; }
    const ggo::set3<data_t> & center() const { return _center; }
      
    data_t &                  radius() { return _radius; }
    const data_t &            radius() const { return _radius; }

    std::string               desc() const override;
  
    static  sphere3d<data_t>  merge(const sphere3d<data_t> & sphere1, const sphere3d<data_t> & sphere2);

  private:

    ggo::set3<data_t> _center;
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
}