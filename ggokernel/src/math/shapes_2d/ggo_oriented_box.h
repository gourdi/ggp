//////////////////////////////////////////////////////////////////
//
// WARNING : NEVER DIRECTLY INCLUDE THIS FILE!
// INCLUDE 'ggo_math.h' INSTEAD.
//
//////////////////////////////////////////////////////////////////

namespace ggo
{
  template <typename data_t>
  class oriented_box : public affine_shape2d_abc<data_t>, public samplable_shape2d_abc<data_t>
  {
  public:

    using affine_shape2d_abc<data_t>::rotate;
    using affine_shape2d_abc<data_t>::move;
    using samplable_shape2d_abc<data_t>::is_point_inside;

                                    oriented_box(ggo::pos2<data_t> pos, ggo::pos2<data_t> dir, data_t size1, data_t size2) : _pos(pos), _dir(dir.get_normalized()), _size1(size1), _size2(size2) {}

    const ggo::pos2<data_t> &       dir() const { return _dir; }
    ggo::pos2<data_t>               dir2() const { return ggo::pos2<data_t>(-_dir.template get<1>(), _dir.template get<0>()); }

    data_t                          size1() const { return _size1; }
    data_t                          size2() const { return _size2; }

    ggo::pos2<data_t>               operator[](int i) const;

    std::vector<ggo::pos2<data_t>>  get_points() const;

    // Interfaces.
    void                            move(data_t dx, data_t dy) override { _pos.move(dx, dy); }
    void                            rotate(data_t angle, const ggo::pos2<data_t> & center) override;
    ggo::pos2<data_t>               get_center() const override { return _pos; }
    bool                            is_point_inside(data_t x, data_t y) const override;

  private:

    ggo::pos2<data_t> _pos;
    ggo::pos2<data_t> _dir;
    data_t            _size1;
    data_t            _size2;
  };
}

/////////////////////////////////////////////////////////////////////
// I/O operators.
namespace ggo
{
  template <typename data_t>
  std::ostream & operator<<(std::ostream & os, const ggo::oriented_box<data_t> & box)
  {
    auto points = box.get_points();
    os << "(" << points[0] << "; " << points[1] << "; " << points[2] << "; " << points[3] << ")";
    return os;
  }
}

namespace ggo
{
  using oriented_box_float  = oriented_box<float>;
  using oriented_box_double = oriented_box<double>;
}

