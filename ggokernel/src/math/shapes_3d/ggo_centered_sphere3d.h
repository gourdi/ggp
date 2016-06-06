//////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename T>
  class centered_sphere3d : public raytracable_shape3d_abc<T>
  {
  public:

                    centered_sphere3d(T radius) : _radius(radius) {};
       
    bool            intersect_ray(const ggo::ray3d<T> & ray, T & dist, ggo::ray3d<T> & normal) const override;
    bool            is_convex() const override { return true; }

    ggo::set3<T>    sample_point(const ggo::set3<T> & target_pos, T random_variable1, T random_variable2) const override;
    ggo::ray3d<T>   sample_ray(T random_variable1, T random_variable2) const override;
       
    T &             radius() { return _radius; }
    const T &       radius() const { return _radius; }
    
    std::ostream &  operator<<(std::ostream & os) const override;

  private:

    T _radius;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  using centered_sphere3d_double = centered_sphere3d<double>;
  using centered_sphere3d_float  = centered_sphere3d<float>;
}
