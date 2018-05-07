//////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename data_t, bool double_sided = false>
  class face3d : public raytracable_shape3d_abc<data_t>
  {
  public:

                                      face3d(const vertex<data_t> & v1, const vertex<data_t> & v2, const vertex<data_t> & v3);

    const vertex<data_t> &            v1() const { return _v1; }
    const vertex<data_t> &            v2() const { return _v2; }
    const vertex<data_t> &            v3() const { return _v3; }

    bool                              intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const override;
    bool                              intersect_segment(const ggo::pos3<data_t> & pos, const ggo::vec3<data_t> & dir, data_t length) const override;
    bool                              is_convex() const override { return true; }
    std::optional<box3d_data<data_t>> get_bounding_box(const ggo::basis3d<data_t> & basis) const override;

  private:

    bool                              solve_intersection(const ggo::pos3<data_t> & pos, const ggo::vec3<data_t> & dir, data_t & s0, data_t & s1, data_t & s2) const;

  private:

    const vertex<data_t> _v1;
    const vertex<data_t> _v2;
    const vertex<data_t> _v3;
    data_t _m00;
    data_t _m10;
    data_t _m20;
    data_t _m01;
    data_t _m11;
    data_t _m21;
    data_t _m10m21subm11m20;
    data_t _m20m01subm00m21;
    data_t _m00m11subm10m01;
  };
}

namespace ggo
{
  template<typename data_t, bool double_sided>
  std::ostream & operator<<(std::ostream & os, const face3d<data_t, double_sided> & face);
}
