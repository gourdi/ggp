namespace ggo
{
  template <typename data_t>
  class cylinder3d : public raytracable_shape3d_abc<data_t>
  {
  public:

                cylinder3d(const ggo::set3<data_t> & orig, const ggo::set3<data_t> & dir, data_t radius);

    bool        intersect_line(const ggo::ray3d<data_t> & ray, data_t & dist_inf, data_t & dist_sup) const;

    bool        intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const override;
    bool        is_convex() const override { return true; }
    std::string desc() const override;

  private:

    ggo::set3<data_t> _orig;
    ggo::set3<data_t> _dir;
    data_t            _radius;
  };
}
