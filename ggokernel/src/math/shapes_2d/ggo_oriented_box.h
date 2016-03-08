//////////////////////////////////////////////////////////////////
//
// WARNING : NEVER DIRECTLY INCLUDE THIS FILE!
// INCLUDE 'ggo_math.h' INSTEAD.
//
//////////////////////////////////////////////////////////////////

namespace ggo
{
  template <typename T>
  class oriented_box : public affine_shape2d_abc<T>
  {
  public:

    using affine_shape2d_abc<T>::rotate;
    using affine_shape2d_abc<T>::move;

                              oriented_box(ggo::set2<T> pos, ggo::set2<T> dir, T size1, T size2) : _pos(pos), _dir(dir.get_normalized()), _size1(size1), _size2(size2) {}

    const ggo::set2<T> &      dir() const { return _dir; }
    ggo::set2<T>              dir2() const { return ggo::set2<T>(-_dir.y(), _dir.x()); }

    T                         size1() const { return _size1; }
    T                         size2() const { return _size2; }

    ggo::set2<T>              operator[](int i) const;

    std::vector<ggo::set2<T>> get_points() const;

    // Interfaces.
    void                      move(T dx, T dy) override { _pos.x() += dx; _pos.y() += dy; }
    void                      rotate(T angle, const ggo::set2<T> & center) override;
    ggo::set2<T>              get_center() const override { return _pos; }

  private:

    ggo::set2<T> _pos;
    ggo::set2<T> _dir;
    T            _size1;
    T            _size2;
  };
}

/////////////////////////////////////////////////////////////////////
// I/O operators.
namespace ggo
{
  template <typename T>
  std::ostream & operator<<(std::ostream & os, const ggo::oriented_box<T> & box)
  {
    auto points = box.get_draw_points();
    os << "(" << points[0] << "; " << points[1] << "; " << points[2] << "; " << points[3] << ")";
    return os;
  }
}

namespace ggo
{
  using oriented_box_float  = oriented_box<float>;
  using oriented_box_double = oriented_box<double>;
}

