//////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename data_t>
  class plane3d : public raytracable_shape3d_abc<data_t>
  {
  public:

                              plane3d(const ggo::vec3<data_t> & normal, data_t dist_to_origin) : _normal(normalize(normal)), _dist_to_origin(dist_to_origin) {}

    const ggo::vec3<data_t> & normal() const { return _normal; }
    data_t                    dist_to_origin() const { return _dist_to_origin; }

    bool                      intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const override;
    bool                      intersect_segment(const ggo::pos3<data_t> & pos, const ggo::vec3<data_t> & dir, data_t length) const override;
    bool                      is_convex() const override { return true; }

    std::optional<box3d_data<data_t>>  get_bounding_box(const ggo::basis3d<data_t> & basis) const override { return {}; }

  private:

    ggo::vec3<data_t> _normal;
    data_t            _dist_to_origin;
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

    bool  intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const override;
    bool  is_convex() const override { return true; }

    std::optional<box3d_data<data_t>>  get_bounding_box(const ggo::basis3d<data_t> & basis) const override { return {}; }
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  using plane3d_double  = plane3d<double>;
  using plane3d_float   = plane3d<float>;
}
//////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename data_t>
  std::ostream & operator<<(std::ostream & os, const plane3d<data_t> & plane);

  template <typename data_t, int normal_x, int normal_y, int normal_z, int dist_to_origin>
  std::ostream & operator<<(std::ostream & os, const const_plane3d<data_t, normal_x, normal_y, normal_z, dist_to_origin> & plane);
}
