#include <ggo_quadratic.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  cylinder3d<data_t>::cylinder3d(const ggo::set3<data_t> & orig, const ggo::set3<data_t> & dir, data_t radius)
  :
  _orig(orig),
  _dir(dir.get_normalized()),
  _radius(radius)
  {
    GGO_ASSERT_GE(radius, data_t(0));
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  bool cylinder3d<data_t>::intersect_line(const ggo::ray3d<data_t> & ray, data_t & dist_inf, data_t & dist_sup) const
  {
    // Unrolling logical flow.
    //  hypot(Pproj - P) = r*r
    //  hypot(cyl_orig_x + dot(P - cyl_orig, cyl_dir) * dir_cyl_x - P_x) = r*r // Consider the x coordinate only.
    //  hypot(cyl_orig_x + dot(ray_orig + t * ray_dir - cyl_orig, cyl_dir) * dir_cyl_x - ray_orig_x - t * ray_dir_x) = r*r
    //  hypot(cyl_orig_x + dot(diff + t * ray_dir, cyl_dir) * dir_cyl_x - ray_orig_x - t * ray_dir_x) = r*r
    //  hypot(dot(diff + t * ray_dir, cyl_dir) * dir_cyl_x - diff_x - t * ray_dir_x) = r*r
    //  hypot(dot(diff, cyl_dir) * dir_cyl_x + t * dot(ray_dir, cyl_dir) * dir_cyl_x - diff_x - t * ray_dir_x) = r*r
    //  hypot(t * dot(ray_dir, cyl_dir) * dir_cyl_x - t * ray_dir_x + dot(diff, cyl_dir) * dir_cyl_x - diff_x) = r*r
    //  hypot(t * [dot(ray_dir, cyl_dir) * dir_cyl_x - ray_dir_x] + [dot(diff, cyl_dir) * dir_cyl_x - diff_x]) = r*r

    ggo::set3<data_t> diff(ray.pos() - _orig);

    // Original non-optimized code.
    //  data_t deg2 =
    //    ggo::square(ggo::dot(ray.dir(), _dir) * _dir.x() - ray.dir().x()) +
    //    ggo::square(ggo::dot(ray.dir(), _dir) * _dir.y() - ray.dir().y()) +
    //    ggo::square(ggo::dot(ray.dir(), _dir) * _dir.z() - ray.dir().z());
    //  
    //  data_t deg1 =
    //    2 * (ggo::dot(ray.dir(), _dir) * _dir.x() - ray.dir().x()) * (ggo::dot(diff, _dir) * _dir.x() - diff.x()) +
    //    2 * (ggo::dot(ray.dir(), _dir) * _dir.y() - ray.dir().y()) * (ggo::dot(diff, _dir) * _dir.y() - diff.y()) +
    //    2 * (ggo::dot(ray.dir(), _dir) * _dir.z() - ray.dir().z()) * (ggo::dot(diff, _dir) * _dir.z() - diff.z());
    //  
    //  data_t deg0 =
    //    ggo::square(ggo::dot(diff, _dir) * _dir.x() - diff.x()) +
    //    ggo::square(ggo::dot(diff, _dir) * _dir.y() - diff.y()) +
    //    ggo::square(ggo::dot(diff, _dir) * _dir.z() - diff.z()) -
    //    ggo::square(_radius);

    // Optimized code.
    ggo::set3<data_t> a = ggo::dot(ray.dir(), _dir) * _dir - ray.dir();
    ggo::set3<data_t> b = ggo::dot(diff, _dir) * _dir - diff;

    data_t deg2 = a.get_hypot();
    data_t deg1 = 2 * ggo::dot(a, b);
    data_t deg0 = b.get_hypot() - ggo::square(_radius);

    data_t sol1, sol2;
    if (ggo::quadratic_solve(deg2, deg1, deg0, sol1, sol2) == false)
    {
      return false;
    }

    if (sol1 < sol2)
    {
      dist_inf = sol1;
      dist_sup = sol2;
    }
    else
    {
      dist_inf = sol2;
      dist_sup = sol1;
    }

    return true;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  bool cylinder3d<data_t>::intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const
  {
    data_t dist_inf = 0;
    data_t dist_sup = 0;
    if (intersect_line(ray, dist_inf, dist_sup) == false)
    {
      return false;
    }

    if (dist_sup < 0) // The cylinder is behind the ray.
    {
      return false;
    }
    
    if (dist_inf < 0) // The ray's origin is inside of the cylinder.
    {
      dist = dist_sup;
      normal.pos() = ray.pos() + dist * ray.dir();

      ggo::set3<data_t> diff = normal.pos() - _orig;
      ggo::set3<data_t> proj = _orig + ggo::dot(diff, _dir) * _dir;
      normal.set_dir(proj - normal.pos());
    }
    else // The ray's origin is outside of the cylinder.
    {
      dist = dist_inf;
      normal.pos() = ray.pos() + dist * ray.dir();

      ggo::set3<data_t> diff = normal.pos() - _orig;
      ggo::set3<data_t> proj = _orig + ggo::dot(diff, _dir) * _dir;
      normal.set_dir(normal.pos() - proj);
    }

    return true;
  }
  
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  std::ostream & cylinder3d<data_t>::operator<<(std::ostream & os) const
  {
    os << "cylinder (" << _orig << ", " << _dir << ", " << _radius << ")";
    return os;
  }
}