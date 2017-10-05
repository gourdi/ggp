//////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename T, bool double_sided = false>
  class triangle3d : public raytracable_shape3d_abc<T>
  {
  public:

                          triangle3d() {}
                          triangle3d(const ggo::pos3<T> & v1, const ggo::pos3<T> & v2, const ggo::pos3<T> & v3) : _v1(v1), _v2(v2), _v3(v3) {}
                          triangle3d(T v1_x, T v1_y, T v1_z, T v2_x, T v2_y, T v2_z, T v3_x, T v3_y, T v3_z) : _v1(v1_x, v1_y, v1_z), _v2(v2_x, v2_y, v2_z), _v3(v3_x, v3_y, v3_z) {}

    const ggo::pos3<T> &	v1() const { return _v1; }
    const ggo::pos3<T> &	v2() const { return _v2; }
    const ggo::pos3<T> &	v3() const { return _v3; }

    ggo::pos3<T> &		    v1() { return _v1; }
    ggo::pos3<T> &		    v2() { return _v2; }
    ggo::pos3<T> &		    v3() { return _v3; }

    bool                  intersect_ray(const ggo::ray3d<T> & ray, T & dist, ggo::ray3d<T> & normal) const override;
    bool                  is_convex() const override { return true; }
  
    void                  get_bounding_sphere(ggo::sphere3d<T> & bounding_sphere) const;
  
    ggo::vec3<T>          get_normal() const;
    ggo::pos3<T>          get_center() const;

    T					            area() const;

  private:

    ggo::pos3<T>	_v1;
    ggo::pos3<T>	_v2;
    ggo::pos3<T>	_v3;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  typedef triangle3d<float>  triangle3d_float ;
  typedef triangle3d<double> triangle3d_double;
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename data_t, bool double_sided>
  std::ostream & operator<<(std::ostream & os, const triangle3d<data_t, double_sided> & t);
}




