namespace ggo
{
  template <typename data_t>
  class disc3d : public raytracable_shape3d_abc<data_t>
  {
  public:
  
                              disc3d(const ggo::ray3d<data_t> & ray, data_t radius) : _ray(ray), _radius(radius) {}
    
    const ggo::ray3d<data_t>  ray() const { return _ray; }
    data_t                    radius() const { return _radius; }
    
    bool                      intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const override;
    bool                      is_convex() const override { return true; }
    std::ostream &            operator<<(std::ostream & os) const override;

  private:
  
    ggo::ray3d<data_t> _ray;
    data_t             _radius;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  using disc3d_double = disc3d<double>;
  using disc3d_float  = disc3d<float>;
}