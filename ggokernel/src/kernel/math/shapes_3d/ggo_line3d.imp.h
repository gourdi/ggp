#include <kernel/math/linear_algebra/ggo_linear_algebra2d.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  line3d<data_t>::line3d(const ggo::ray3d<data_t> & ray)
  :
  _pos(ray.pos()),
  _dir(ray.dir())
  {

  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  line3d<data_t>::line3d(const ggo::pos3<data_t> & pos, const ggo::vec3<data_t> & dir, bool normalize)
  :
  _pos(pos),
  _dir(normalize ? ggo::normalize(dir) : dir)
  {
  }
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  bool find_closest_lines_points(const ggo::line3d<data_t> & line1, const ggo::line3d<data_t> & line2, data_t & dist1, data_t & dist2)
  {
    GGO_ASSERT(is_normalized(line1.dir()) == true);
    GGO_ASSERT(is_normalized(line2.dir()) == true);

    const ggo::vec3<data_t> diff(line2.pos() - line1.pos());

    const float dot = ggo::dot(line1.dir(), line2.dir());
    const float m[2][2] = { { 1, -dot }, { dot, -1 } };
    const float c[2] = { ggo::dot(line1.dir(), diff), ggo::dot(line2.dir(), diff) };
    float s[2] = { 0.f, 0.f };

    if (ggo::linsolve2d(m, c, s) == false)
    {
      return false;
    }

    dist1 = s[0];
    dist2 = s[1];

    return true;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  bool find_closest_lines_points(const ggo::line3d<data_t> & line1, const ggo::line3d<data_t> & line2, ggo::pos3<data_t> & p1, ggo::pos3<data_t> & p2)
  {
    data_t dist1, dist2;

    if (find_closest_lines_points(line1, line2, dist1, dist2) == false)
    {
      return false;
    }

    p1 = line1.pos() + dist1 * line1.dir();
    p2 = line2.pos() + dist2 * line2.dir();

    return true;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  data_t hypot(const ggo::line3d<data_t> & line, const ggo::pos3<data_t> & p)
  {
    ggo::vec3<data_t> diff = p - line.pos();
    ggo::pos3<data_t> proj = line.pos() + ggo::dot(diff, line.dir()) * line.dir();

    return ggo::hypot(proj, p);
  }
}

