//////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename data_t>
  class centered_sphere3d : public raytracable_shape3d_abc<data_t>
  {
  public:

                        centered_sphere3d(data_t radius) : _radius(radius) {}

    data_t &            radius() { return _radius; }
    const data_t &      radius() const { return _radius; }

    bool                intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const override;
    bool                is_convex() const override { return true; }

    ggo::pos3<data_t>   sample_point(const ggo::pos3<data_t> & target_pos, data_t random_variable1, data_t random_variable2) const override;
    ggo::ray3d<data_t>  sample_ray(data_t random_variable1, data_t random_variable2) const override;

    std::optional<box3d_data<data_t>>  get_bounding_box(const ggo::basis3d<data_t> & basis) const override;

  private:

    data_t _radius;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  using centered_sphere3d_double = centered_sphere3d<double>;
  using centered_sphere3d_float  = centered_sphere3d<float>;
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename data_t>
  std::ostream &  operator<<(std::ostream & os, const centered_sphere3d<data_t> & s);
}



