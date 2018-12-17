//////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename data_t, bool double_sided = false>
  class triangle3d : public raytracable_shape3d_abc<data_t>
  {
  public:

                              triangle3d() {}
                              triangle3d(const ggo::pos3<data_t> & v1, const ggo::pos3<data_t> & v2, const ggo::pos3<data_t> & v3) : _v1(v1), _v2(v2), _v3(v3) {}

    const ggo::pos3<data_t> &	v1() const { return _v1; }
    const ggo::pos3<data_t> &	v2() const { return _v2; }
    const ggo::pos3<data_t> &	v3() const { return _v3; }

    ggo::pos3<data_t> &		    v1() { return _v1; }
    ggo::pos3<data_t> &		    v2() { return _v2; }
    ggo::pos3<data_t> &		    v3() { return _v3; }

    bool                      intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const override;
    bool                      is_convex() const override { return true; }

    std::optional<box3d_data<data_t>>  get_bounding_box(const ggo::basis3d<data_t> & basis) const override;
  
    void                      get_bounding_sphere(ggo::sphere3d<data_t> & bounding_sphere) const;
  
    ggo::vec3<data_t>         get_normal() const;
    ggo::pos3<data_t>         get_center() const;

    data_t  	                area() const;

  private:

    ggo::pos3<data_t>	_v1;
    ggo::pos3<data_t>	_v2;
    ggo::pos3<data_t>	_v3;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  using triangle3d_f = triangle3d<float>;
  using triangle3d_d = triangle3d<double>;
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename data_t, bool double_sided>
  std::ostream & operator<<(std::ostream & os, const triangle3d<data_t, double_sided> & t);
}




