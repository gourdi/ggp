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

                                      oriented_box(ggo::pos2<data_t> pos, ggo::pos2<data_t> dir_x, data_t size_x, data_t size_y) : _pos(pos), _dir_x(normalize(dir_x)), _size_x(size_x), _size_y(size_y) {}

    const ggo::pos2<data_t> &         pos() const { return _pos; }
    const ggo::vec2<data_t> &         dir_x() const { return _dir_x; }
    ggo::vec2<data_t>                 dir_y() const { return ggo::pos2<data_t>(-_dir_x.y(), _dir_x.x()); }
    data_t                            size_x() const { return _size_x; }
    data_t                            size_y() const { return _size_y; }

    ggo::pos2<data_t>                 operator[](int i) const;

    std::array<ggo::pos2<data_t>, 4>  get_points() const;

    // Interfaces.
    void                              move(const ggo::vec2<data_t> & m) override { _pos += m; }
    void                              rotate(data_t angle, const ggo::pos2<data_t> & center) override;
    ggo::pos2<data_t>                 get_center() const override { return _pos; }
    bool                              is_point_inside(const ggo::pos2<data_t> & p) const override;
    rect_data<data_t>                 get_bounding_rect() const override;
    rect_intersection                 get_rect_intersection(const rect_data<data_t> & rect_data) const override;

  private:

    ggo::pos2<data_t> _pos;
    ggo::pos2<data_t> _dir_x;
    data_t            _size_x;
    data_t            _size_y;
  };
}

namespace ggo
{
  template <typename data_t>
  ggo::range<data_t> project(const ggo::oriented_box<float> & box, ggo::vec2_f dir)
  {
    GGO_ASSERT(ggo::is_normalized(dir));

    data_t dot_center = ggo::dot(box.pos(), dir);
    data_t dot_dir_x = ggo::dot(box.dir_x(), dir);
    data_t dot_dir_y = ggo::dot(box.dir_y(), dir);

    return {
      dot_center - std::abs(box.size_x() * dot_dir_x) - std::abs(box.size_y() * dot_dir_y),
      dot_center + std::abs(box.size_x() * dot_dir_x) + std::abs(box.size_y() * dot_dir_y) };
  };

  // Helper to ease template parameter deduction.
  inline ggo::range_f project(const ggo::oriented_box<float> & box, ggo::vec2_f dir) { return project<float>(box, dir); }

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
  using oriented_box_f = oriented_box<float>;
  using oriented_box_d = oriented_box<double>;
}

