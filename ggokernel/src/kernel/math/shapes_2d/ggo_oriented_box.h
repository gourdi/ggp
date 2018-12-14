//////////////////////////////////////////////////////////////////
//
// WARNING : NEVER DIRECTLY INCLUDE THIS FILE!
// INCLUDE 'ggo_math.h' INSTEAD.
//
//////////////////////////////////////////////////////////////////

namespace ggo
{
  template <typename data_type>
  class oriented_box final : public paintable_shape2d_abc<data_type>, public affine_shape2d_abc<data_type>
  {
  public:

    using data_t = data_type;
    using affine_shape2d_abc<data_t>::rotate;
    using affine_shape2d_abc<data_t>::move;
    using samplable_shape2d_abc<data_t>::is_point_inside;

                                    oriented_box(ggo::pos2<data_t> pos, ggo::pos2<data_t> dir, data_t size1, data_t size2) : _pos(pos), _dir(normalize(dir)), _size1(size1), _size2(size2) {}

    const ggo::pos2<data_t> &       pos() const { return _pos; }
    const ggo::vec2<data_t> &       dir() const { return _dir; }
    ggo::vec2<data_t>               dir2() const { return ggo::pos2<data_t>(-_dir.y(), _dir.x()); }
    data_t                          size1() const { return _size1; }
    data_t                          size2() const { return _size2; }

    ggo::pos2<data_t>               operator[](int i) const;

    std::vector<ggo::pos2<data_t>>  get_points() const;

    // Interfaces.
    void                            move(const ggo::vec2<data_t> & m) override { _pos += m; }
    void                            rotate(data_t angle, const ggo::pos2<data_t> & center) override;
    ggo::pos2<data_t>               get_center() const override { return _pos; }
    bool                            is_point_inside(const ggo::pos2<data_t> & p) const override;
    rect_data<data_t>               get_bounding_rect() const override;
    rect_intersection               get_rect_intersection(const rect_data<data_t> & rect_data) const override;

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
    os << "(" << box.pos() << "; " << box.dir() << "; " << box.size1() << "; " << box.size2() << ")";
    return os;
  }
}

namespace ggo
{
  using oriented_box_float  = oriented_box<float>;
  using oriented_box_double = oriented_box<double>;
}

