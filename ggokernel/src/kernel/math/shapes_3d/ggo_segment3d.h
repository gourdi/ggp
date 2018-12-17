//////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename data_t>
  class segment3d
  {
  public:

                              segment3d() {};
                              segment3d(const segment3d<data_t> & s): _v1(s._v1), _v2(s._v2) {};
                              segment3d(const ggo::pos3<data_t> & v1, const ggo::pos3<data_t> & v2): _v1(v1), _v2(v2) {};

    const ggo::pos3<data_t> &	v1() const { return _v1; }
    const ggo::pos3<data_t> &	v2() const { return _v2; }

    ggo::pos3<data_t> &		    v1() { return _v1; }
    ggo::pos3<data_t> &		    v2() { return _v2; }

  private:

    ggo::pos3<data_t>	_v1;
    ggo::pos3<data_t>	_v2;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  using segment3d_f = segment3d<float>;
  using segment3d_d = segment3d<double>;
}
