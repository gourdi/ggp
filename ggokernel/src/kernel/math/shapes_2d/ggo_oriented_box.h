//////////////////////////////////////////////////////////////////
//
// WARNING : NEVER DIRECTLY INCLUDE THIS FILE!
// INCLUDE 'ggo_math.h' INSTEAD.
//
//////////////////////////////////////////////////////////////////

namespace ggo
{
  enum class oriented_box_vertex_id
  {
    left_bottom, left_top, right_bottom, right_top
  };

  enum class oriented_box_edge_id
  {
    left, right, bottom, top
  };

  template <typename data_type>
  class oriented_box final : public paintable_shape2d_abc<data_type>, public affine_shape2d_abc<data_type>
  {
  public:

    struct vertex
    {
      ggo::pos2<data_t> _pos;
      oriented_box_vertex_id _id;
    };

    struct edge
    {
      std::array<vertex, 2> _vertices;
      ggo::vec2<data_t> _normal;
      oriented_box_edge_id _id;
    };

    using data_t = data_type;
    using affine_shape2d_abc<data_t>::rotate;
    using affine_shape2d_abc<data_t>::move;
    using samplable_shape2d_abc<data_t>::is_point_inside;

                                      oriented_box(ggo::pos2<data_t> pos, data_t angle, data_t half_size_x, data_t half_size_y) : _pos(pos), _angle(angle), _half_size_x(half_size_x), _half_size_y(half_size_y) {}

    const ggo::pos2<data_t> &         pos() const { return _pos; }
    data_t                            angle() const { return _angle; }
    data_t                            half_size_x() const { return _half_size_x; }
    data_t                            half_size_y() const { return _half_size_y; }

    ggo::vec2<data_t>                 dir_x() const { return { std::cos(_angle), std::sin(_angle) }; }
    ggo::vec2<data_t>                 dir_y() const { return { -std::sin(_angle), std::cos(_angle) }; }

    ggo::pos2<data_t>                 operator[](int i) const;
    std::array<ggo::pos2<data_t>, 4>  get_points() const;
    std::array<typename vertex, 4>    get_vertices() const;
    std::array<typename edge, 4>      get_edges() const;

    // Interfaces.
    void                              move(const ggo::vec2<data_t> & m) override { _pos += m; }
    void                              rotate(data_t angle, const ggo::pos2<data_t> & center) override;
    ggo::pos2<data_t>                 get_center() const override { return _pos; }
    bool                              is_point_inside(const ggo::pos2<data_t> & p) const override;
    rect_data<data_t>                 get_bounding_rect() const override;
    rect_intersection                 get_rect_intersection(const rect_data<data_t> & rect_data) const override;

  private:

    ggo::pos2<data_t> _pos;
    data_t            _angle;
    data_t            _half_size_x;
    data_t            _half_size_y;
  };
}

/////////////////////////////////////////////////////////////////////
// Project.
namespace ggo
{
  template <typename data_t>
  struct oriented_box_projection
  {
    data_t _center;
    data_t _inf;
    data_t _sup;
  };

  template <typename data_t>
  ggo::oriented_box_projection<data_t> project(const ggo::oriented_box<data_t> & box, ggo::vec2_f dir)
  {
    GGO_ASSERT(ggo::is_normalized(dir));

    const data_t cos = std::cos(box.angle());
    const data_t sin = std::sin(box.angle());

    const ggo::vec2<data_t> x{ cos, sin };
    const ggo::vec2<data_t> y{ -sin, cos };

    data_t dot_center = ggo::dot(box.pos(), dir);
    data_t dot_dir_x = ggo::dot(x, dir);
    data_t dot_dir_y = ggo::dot(y, dir);

    return {
      dot_center,
      dot_center - std::abs(box.half_size_x() * dot_dir_x) - std::abs(box.half_size_y() * dot_dir_y),
      dot_center + std::abs(box.half_size_x() * dot_dir_x) + std::abs(box.half_size_y() * dot_dir_y) };
  };

  // Helper to ease template parameter deduction.
  inline ggo::oriented_box_projection<float> project(const ggo::oriented_box<float> & box, ggo::vec2_f dir) { return project<float>(box, dir); }
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

  inline
  std::ostream & operator<<(std::ostream & os, ggo::oriented_box_edge_id id)
  {
    switch (id)
    {
    case ggo::oriented_box_edge_id::left:
      os << "left";
      break;
    case ggo::oriented_box_edge_id::right:
      os << "right";
      break;
    case ggo::oriented_box_edge_id::bottom:
      os << "bottom";
      break;
    case ggo::oriented_box_edge_id::top:
      os << "top";
      break;
    }

    return os;
  }

  inline
  std::ostream & operator<<(std::ostream & os, ggo::oriented_box_vertex_id id)
  {
    switch (id)
    {
    case ggo::oriented_box_vertex_id::left_bottom:
      os << "left_bottom";
      break;
    case ggo::oriented_box_vertex_id::left_top:
      os << "left_top";
      break;
    case ggo::oriented_box_vertex_id::right_bottom:
      os << "right_bottom";
      break;
    case ggo::oriented_box_vertex_id::right_top:
      os << "right_top";
      break;
    }

    return os;
  }
}

namespace ggo
{
  using oriented_box_f = oriented_box<float>;
  using oriented_box_d = oriented_box<double>;
}

