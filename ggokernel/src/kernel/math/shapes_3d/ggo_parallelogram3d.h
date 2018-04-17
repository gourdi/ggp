namespace ggo
{
  template <typename data_t>
  class parallelogram3d : public raytracable_shape3d_abc<data_t>
  {
  public:
  
                              parallelogram3d(const ggo::pos3<data_t> & pos, const ggo::vec3<data_t> & v1, const ggo::vec3<data_t> & v2);

    const ggo::pos3<data_t> & pos() const { return _pos; }
    const ggo::vec3<data_t> & v1() const { return _v1; }
    const ggo::vec3<data_t> & v2() const { return _v2; }

        
    bool                      intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const override;
    bool                      is_convex() const override { return true; }
    std::optional<box3d_data<data_t>>  get_bounding_box(const ggo::basis3d<data_t> & basis) const override;

  private:
  
    ggo::pos3<data_t>  _pos;
    ggo::vec3<data_t>  _v1;
    ggo::vec3<data_t>  _v2;
    ggo::vec3<data_t>  _normal;
  };
}

namespace ggo
{
  using parallelogram3d_float   = parallelogram3d<float>;
  using parallelogram3d_double  = parallelogram3d<double>;
}

namespace ggo
{
  template <typename data_t>
  std::ostream & operator<<(std::ostream & os, const parallelogram3d<data_t> & parallelogram);
}

