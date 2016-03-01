//////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename T>
  class segment3d
  {
  public:

                          segment3d() {};
                          segment3d(const segment3d<T> & s): _v1(s._v1), _v2(s._v2) {};
                          segment3d(const ggo::set3<T> & v1, const ggo::set3<T> & v2): _v1(v1), _v2(v2) {};

    const ggo::set3<T> &	v1() const { return _v1; }
    const ggo::set3<T> &	v2() const { return _v2; }

    ggo::set3<T> &		    v1() { return _v1; }
    ggo::set3<T> &		    v2() { return _v2; }

  private:

    ggo::set3<T>	_v1;
    ggo::set3<T>	_v2;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{
#if __cplusplus >= 201103L
  using segment3d_float   = segment3d<float>;
  using segment3d_double  = segment3d<double>;
#else
  typedef segment3d<float>  segment3d_float;
  typedef segment3d<double> segment3d_double;
#endif
}
