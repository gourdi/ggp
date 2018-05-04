#include <kernel/math/sampling/shape_sampling/ggo_shape_sampling.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  // Solve the equation (x-center_x)^2+(y-center_y)^2+(z-center_z)^2=r^2
  // injecting the ray into the equation.
  template <typename data_t>
  bool centered_sphere3d<data_t>::intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const
  {
    // Build the quadratic and solve it.
    
    // Hint 1: deg2 is dot(dir, dir) which is 1
    // Hint 2: actually, deg1 should be 2*dot(dir,diff), but removing 
    // the multiplication simplifies the computations
    data_t deg1 = ggo::dot(ray.dir(), ray.pos());
    data_t deg0 = ggo::dot(ray.pos(), ray.pos()) - _radius * _radius;
    
    // Solve it.
    data_t d = deg1 * deg1 - deg0;
    if (d < 0)
    {
      return false;
    }
    d = std::sqrt(d);

    // Check the lower solution.
    dist = -deg1 - d;
    if (dist >= 0)
    {
      normal.pos() = ray.pos() + dist * ray.dir();
      normal.set_dir(normal.pos());
      return true;
    }

    // Check the greatest solution (the ray's origin is then inside the sphere, which is a valid case).
    // In that case, the normal points towards the center of the sphere.
    dist = -deg1 + d;
    if (dist >= 0)
    {
      normal.pos() = ray.pos() + dist * ray.dir();
      normal.set_dir(-normal.pos());
      return true;
    }

    return false;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  bool centered_sphere3d<data_t>::intersect_segment(const ggo::pos3<data_t> & pos, const ggo::vec3<data_t> & dir, data_t length) const
  {
    // Hint 1: deg2 is dot(dir, dir) which is 1
    // Hint 2: actually, deg1 should be 2*dot(dir,diff), but removing 
    // the multiplication simplifies the computations
    data_t deg1 = ggo::dot(dir, pos);
    data_t deg0 = ggo::dot(pos, pos) - _radius * _radius;

    // Solve it.
    data_t d = deg1 * deg1 - deg0;
    if (d < 0)
    {
      return false;
    }
    d = std::sqrt(d);

    data_t dist_inf = -deg1 - d;
    if (dist_inf > length)
    {
      return false;
    }

    data_t dist_sup = -deg1 + d;
    if (dist_sup < 0)
    {
      return false;
    }

    return true;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  ggo::pos3<data_t> centered_sphere3d<data_t>::sample_point(const ggo::pos3<data_t> & target_pos, data_t random_variable1, data_t random_variable2) const
  {
    ggo::vec3<data_t> dir(target_pos);
    dir.normalize();

    return _radius * ggo::hemisphere_sampling(dir, random_variable1, random_variable2);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  ggo::ray3d<data_t> centered_sphere3d<data_t>::sample_ray(data_t random_variable1, data_t random_variable2) const
  {
    ggo::pos3<data_t> sphere_sample = ggo::sphere_sampling(random_variable1, random_variable2);
    ggo::pos3<data_t> hemisphere_sample = ggo::hemisphere_sampling(sphere_sample, random_variable1, random_variable2);

    return ggo::ray3d<data_t>(_radius * sphere_sample, hemisphere_sample, false);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  std::optional<box3d_data<data_t>> centered_sphere3d<data_t>::get_bounding_box(const ggo::basis3d<data_t> & basis) const
  {
    const ggo::vec3<data_t> x(data_t(1), data_t(0), data_t(0));
    const ggo::vec3<data_t> y(data_t(0), data_t(1), data_t(0));
    const ggo::vec3<data_t> z(data_t(0), data_t(0), data_t(1));

    return box3d_data<data_t>::from({
      basis.pos() - _radius * x, basis.pos() + _radius * x,
      basis.pos() - _radius * y, basis.pos() + _radius * y,
      basis.pos() - _radius * z, basis.pos() + _radius * z });
  }
  
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  std::ostream &  operator<<(std::ostream & os, const centered_sphere3d<data_t> & s)
  {
    os << "(radius=" << s.radius() << ")";
    return os;
  }
}
