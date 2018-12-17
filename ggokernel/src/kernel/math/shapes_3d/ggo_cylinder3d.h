namespace ggo
{
  template <typename data_t>
  class cylinder3d : public raytracable_shape3d_abc<data_t>
  {
  public:

                            cylinder3d(const ggo::pos3<data_t> & pos, const ggo::vec3<data_t> & dir, data_t size, data_t radius);

    const ggo::pos3<data_t> pos() const { return _pos; }
    const ggo::vec3<data_t> dir() const { return _dir; }
    data_t                  radius() const { return _radius; }

    bool                    intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const override;
    bool                    is_convex() const override { return false; }

    std::optional<box3d_data<data_t>>  get_bounding_box(const ggo::basis3d<data_t> & basis) const override;

  private:

    bool                    intersect_line(const ggo::line3d<data_t> & line, data_t & dist_inf, data_t & dist_sup) const;

  private:

    ggo::pos3<data_t> _pos;
    ggo::vec3<data_t> _dir;
    data_t            _size;
    data_t            _radius;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  using cylinder3d_f = cylinder3d<float>;
  using cylinder3d_d = cylinder3d<double>;
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename data_t>
  std::ostream & operator<<(std::ostream & os, const cylinder3d<data_t> & cylinder);
}

