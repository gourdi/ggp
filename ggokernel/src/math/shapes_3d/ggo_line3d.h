//////////////////////////////////////////////////////////////
namespace ggo
{
  template<typename data_t>
  class ray3d;
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename data_t>
  class line3d
  {
  public:

                              line3d(const ggo::ray3d<data_t> & ray);
                              line3d(const ggo::set3<data_t> & pos, const ggo::set3<data_t> & dir, bool normalize = true);

    const ggo::set3<data_t> & pos() const { return _pos; }
    const ggo::set3<data_t> & dir() const { return _dir; }

  private:
    
    ggo::set3<data_t> _pos;
    ggo::set3<data_t> _dir;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename data_t>
  bool find_closest_lines_point(const ggo::line3d<data_t> & line1, const ggo::line3d<data_t> & line2, data_t & dist1, data_t & dist2);

  template <typename data_t>
  bool find_closest_lines_point(const ggo::line3d<data_t> & line1, const ggo::line3d<data_t> & line2, ggo::set3<data_t> & p1, ggo::set3<data_t> & p2);

  template <typename data_t>
  data_t hypot(const ggo::line3d<data_t> & line, const ggo::set3<data_t> & p);
}
