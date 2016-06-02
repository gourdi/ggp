namespace ggo
{
  template <typename data_t>
  class metaball : public raytracable_shape3d_abc<data_t>
  {
  public:

                metaball(float threshold);

    void        add_influence_sphere(const ggo::sphere3d<data_t> & sphere, data_t potential);

    bool	      intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const override;
    bool	      is_convex() const override { return false; }
    std::string desc() const override { return "metaball"; }

  public:

    struct influence_sphere
    {
      ggo::sphere3d<data_t> _sphere;
      data_t                _potential;
      data_t                _inv_squared_radius;

      data_t                evaluate(const ggo::set3<data_t> & pos) const;
    };

    struct intersection_info
    {
      bool 						          _entry;
      const influence_sphere *  _influence_sphere;
      float 						        _dist;
    };

            std::vector<intersection_info>  get_intersections(const ggo::ray3d<data_t> & ray) const;

    static  bool                            update_active_list(const float dist,
                                                               const std::vector<typename ggo::metaball<data_t>::intersection_info> & intersections,
                                                               std::vector<const intersection_info*> & active_list,
                                                               typename std::vector<intersection_info>::const_iterator & intersection_it);

    static  data_t                          compute_field_potential(const ggo::set3<data_t> & pos,
                                                                    const std::vector<const intersection_info*> & active_list);

    std::vector<influence_sphere> _influence_spheres;
    float							 		        _threshold;
  };
}