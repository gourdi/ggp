namespace ggo
{
  template <typename data_t>
  class influence_plane3d : public ggo::influence_shape3d_abc<data_t>
  {
  public:

    influence_plane3d(const ggo::vector3d<data_t> & normal, data_t dist_to_origin, data_t width);

    std::vector<data_t> intersect_ray(const ggo::ray3d<data_t> & ray) const override;
    data_t              hypot_to_center(const ggo::point3d<data_t> & p) const override;
    data_t              get_influence_hypot() const override { return _width * _width; }

  private:

    const ggo::vector3d<data_t> _normal;
    const data_t                _dist_to_origin;
    const data_t                _width;
  };
}

