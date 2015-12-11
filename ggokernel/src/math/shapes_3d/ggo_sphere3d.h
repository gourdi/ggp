//////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename T>
  class sphere3d : public raytracable_shape3d_abc<T>
  {
  public:

                          sphere3d() : _center(0, 0, 0), _radius(1) {};
                          sphere3d(const ggo::set3<T> & center, T radius) : _center(center), _radius(radius) {};
    
    bool                  intersect_ray(const ggo::ray3d<T> & ray, T & dist, ggo::ray3d<T> & normal) const override;
    bool                  is_convex() const override { return true; }

    ggo::set3<T> &        center() { return _center; }
    const ggo::set3<T> &  center() const { return _center; }
      
    T &                   radius() { return _radius; }
    const T &             radius() const { return _radius; }
            
    bool                  intersect_ray(const ggo::ray3d<T> & ray) const;
    
    std::string           desc() const override;
  
    static  sphere3d<T>   merge(const sphere3d<T> & sphere1, const sphere3d<T> & sphere2);

  private:

    ggo::set3<T> _center;
    T            _radius;
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
  template <typename T>
  T distance(const ggo::sphere3d<T> & sphere1, const ggo::sphere3d<T> & sphere2)
  {
    return std::max<T>(0, ggo::distance(sphere1.center(), sphere2.center()) - sphere1.radius() - sphere2.radius());
  }
}