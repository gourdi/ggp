namespace ggo
{
  template <typename T>
  class parallelogram3d : public raytracable_shape3d_abc<T>
  {
  public:
  
                    parallelogram3d(const ggo::pos3<T> & pos, const ggo::vec3<T> & v1, const ggo::vec3<T> & v2);
        
    bool            intersect_ray(const ggo::ray3d<T> & ray, T & dist, ggo::ray3d<T> & normal) const override;
    bool            is_convex() const override { return true; }
        
    std::ostream &  operator<<(std::ostream & os) const override;

  private:
  
    ggo::pos3<T>  _pos;
    ggo::vec3<T>  _v1;
    ggo::vec3<T>  _v2;
    ggo::vec3<T>  _normal;
  };
}

namespace ggo
{
  using parallelogram3d_float   = parallelogram3d<float>;
  using parallelogram3d_double  = parallelogram3d<double>;
}

