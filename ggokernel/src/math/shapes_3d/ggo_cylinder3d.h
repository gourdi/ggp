namespace ggo
{
  template <typename data_t>
  class cylinder3d : public raytracable_shape3d_abc<data_t>, public influence_shape3d_abc<data_t>
  {
  public:

                        cylinder3d(const ggo::pos3<data_t> & pos, const ggo::vec3<data_t> & dir, data_t radius);

    bool                intersect_line(const ggo::line3d<data_t> & line, data_t & dist_inf, data_t & dist_sup) const;

    std::vector<data_t> intersect_ray(const ggo::ray3d<data_t> & ray) const override;
    data_t              hypot_to_center(const ggo::pos3<data_t> & p) const override { return ggo::hypot(ggo::line3d<data_t>(_pos, _dir, false), p); }
    data_t              get_influence_hypot() const override { return _radius * _radius; }

    bool                intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const override;
    bool                is_convex() const override { return true; }
    std::ostream &      operator<<(std::ostream & os) const override;

  private:

    ggo::pos3<data_t> _pos;
    ggo::vec3<data_t> _dir;
    data_t            _radius;
  };
}
