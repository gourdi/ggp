namespace ggo
{
  template <typename data_t>
  class plane3d : public raytracable_shape3d_abc<data_t>
  {
  public:

                    plane3d(const ggo::set3<data_t> & normal, data_t dist_to_origin) : _normal(normal.get_normalized()), _dist_to_origin(dist_to_origin) {}

    bool            intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const override;
    bool            is_convex() const override { return true; }
    std::ostream &  operator<<(std::ostream & os) const override;

  private:

    const ggo::set3<data_t> _normal;
    const data_t            _dist_to_origin;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  using plane3d_double  = plane3d<double>;
  using plane3d_float   = plane3d<float>;
}

