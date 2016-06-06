//////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename data_t>
  class segment3d
  {
  public:

                              segment3d() {};
                              segment3d(const segment3d<data_t> & s): _v1(s._v1), _v2(s._v2) {};
                              segment3d(const ggo::set3<data_t> & v1, const ggo::set3<data_t> & v2): _v1(v1), _v2(v2) {};

    const ggo::set3<data_t> &	v1() const { return _v1; }
    const ggo::set3<data_t> &	v2() const { return _v2; }

    ggo::set3<data_t> &		    v1() { return _v1; }
    ggo::set3<data_t> &		    v2() { return _v2; }

  private:

    ggo::set3<data_t>	_v1;
    ggo::set3<data_t>	_v2;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  using segment3d_float   = segment3d<float>;
  using segment3d_double  = segment3d<double>;
}
