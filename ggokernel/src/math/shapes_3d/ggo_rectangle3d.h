namespace ggo
{
  template <int normal_x, int normal_y, int normal_z, typename data_t>
  class rectangle3d : public raytracable_shape3d_abc<data_t>
  {
  public:

    static_assert(std::is_floating_point<data_t>::value == true, "expecting floating point type");
    static_assert(normal_x * normal_x + normal_y * normal_y + normal_z * normal_z == 1, "invalid normal coordinates");

    rectangle3d(const pos3<data_t> & center, data_t size1, data_t size2) : _center(center), _size1(size1), _size2(size2) {}

    bool intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const override;
    bool is_convex() const override { return true; }

  private:

    bool intersect_ray_x(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const;
    bool intersect_ray_y(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const;
    bool intersect_ray_z(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const;

    const pos3<data_t> _center;
    const data_t _size1;
    const data_t _size2;
  };
}

