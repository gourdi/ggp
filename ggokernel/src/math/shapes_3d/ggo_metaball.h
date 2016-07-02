#include <memory>

namespace ggo
{
  template <typename data_t>
  class metaball : public raytracable_shape3d_abc<data_t>
  {
  public:

                    metaball(data_t threshold);

    void            add_influence_data(std::shared_ptr<influence_shape3d_abc<data_t>> shape, data_t potential);

    bool	          intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const override;
    bool	          is_convex() const override { return false; }
    std::ostream &  operator<<(std::ostream & os) const override { os << "metaball"; return os; }

  public:

    struct influence_data
    {
      std::shared_ptr<influence_shape3d_abc<data_t>>  _shape;
      data_t                                          _potential;

      data_t  evaluate(const ggo::pos3<data_t> & pos) const;
    };

    struct intersection_info
    {
      bool 						        _entry;
      const influence_data *  _influence;
      data_t 						      _dist;
    };

            std::vector<intersection_info>  get_intersections(const ggo::ray3d<data_t> & ray) const;

    static  bool                            update_active_list(const data_t dist,
                                                               const std::vector<typename ggo::metaball<data_t>::intersection_info> & intersections,
                                                               std::vector<const intersection_info*> & active_list,
                                                               typename std::vector<intersection_info>::const_iterator & intersection_it);

    static  data_t                          compute_field_potential(const ggo::pos3<data_t> & pos,
                                                                    const std::vector<const intersection_info*> & active_list);

    std::vector<influence_data> _influences;
    data_t  					 		      _threshold;
  };
}