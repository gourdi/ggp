//////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename data_t>
  class plane3d : public raytracable_shape3d_abc<data_t>
  {
  public:

                    plane3d(const ggo::vec3<data_t> & normal, data_t dist_to_origin) : _normal(normal.get_normalized()), _dist_to_origin(dist_to_origin) {}

    bool            intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const override;
    bool            is_convex() const override { return true; }
    std::ostream &  operator<<(std::ostream & os) const override;

  private:

    const ggo::vec3<data_t> _normal;
    const data_t            _dist_to_origin;
  };
}

//////////////////////////////////////////////////////////////
// Compile-time defined plane. Faster than regular plane, 
// but must hold integer data.
namespace ggo
{
  template <typename data_t, int normal_x, int normal_y, int normal_z, int dist_to_origin>
  class const_plane3d : public raytracable_shape3d_abc<data_t>
  {
  public:

                    const_plane3d() { static_assert(normal_x * normal_x + normal_y * normal_y + normal_z * normal_z == 1, "normal is not normalized"); }

    bool            intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const override;
    bool            is_convex() const override { return true; }
    std::ostream &  operator<<(std::ostream & os) const override;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  using plane3d_double  = plane3d<double>;
  using plane3d_float   = plane3d<float>;
}

