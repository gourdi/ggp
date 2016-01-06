namespace ggo
{
  template <typename T>
  class plane3d : public raytracable_shape3d_abc<T>
  {
  public:

                plane3d() : _a(1), _b(0), _c(0), _d(0) {}
                plane3d(T a, T b, T c, T d) : _a(a), _b(b), _c(c), _d(d) {}
  
    const T &	  a() const { return _a; }
    const T &	  b() const { return _b; }
    const T &	  c() const { return _c; }
    const T &	  d() const { return _d; }
  
    T &			    a() { return _a; }
    T &			    b() { return _b; }
    T &			    c() { return _c; }
    T &			    d() { return _d; }
        
    bool        intersect_ray(const ggo::ray3d<T> & ray, float & dist, ggo::ray3d<T> & normal) const override;
    bool        is_convex() const override { return true; }
    std::string desc() const override;
      
  private:

    T	_a;
    T	_b;
    T	_c;
    T	_d;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{
#if __cplusplus >= 201103L
  using plane3d_double  = plane3d<double>;
  using plane3d_float   = plane3d<float>;
#else
  typedef plane3d<float>  plane3d_float;
  typedef plane3d<double> plane3d_double;
#endif
}

