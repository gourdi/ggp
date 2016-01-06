namespace ggo
{
  template <typename T>
  class disc3d : public raytracable_shape3d_abc<T>
  {
  public:
  
                        disc3d(const ggo::ray3d<T> & ray, T radius) : _ray(ray), _radius(radius) {}
    
    const ggo::ray3d<T> ray() const { return _ray; }
    T                   radius() const { return _radius; }
    
    bool                intersect_ray(const ggo::ray3d<T> & ray, float & dist, ggo::ray3d<T> & normal) const override;
    bool                is_convex() const override { return true; }
    std::string         desc() const override;
  
  private:
  
    ggo::ray3d<T> _ray;
    T             _radius;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  using disc3d_double = disc3d<double>;
  using disc3d_float  = disc3d<float>;
}