namespace ggo
{
  template <typename T>
  class plane3d : public raytracable_shape3d_abc<T>
  {
  public:

                plane3d(const ggo::set3<T> & normal, T dist_to_origin) : _normal(normal.get_normalized()), _dist_to_origin(dist_to_origin) {}

    bool        intersect_ray(const ggo::ray3d<T> & ray, float & dist, ggo::ray3d<T> & normal) const override;
    bool        is_convex() const override { return true; }
    std::string desc() const override;
      
  private:

    const ggo::set3<T> _normal;
    const T            _dist_to_origin;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  using plane3d_double  = plane3d<double>;
  using plane3d_float   = plane3d<float>;
}

