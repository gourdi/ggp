#include <ggo_shape_sampling.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  // Solve the equation (x-center_x)^2+(y-center_y)^2+(z-center_z)^2=r^2
  // injecting the ray into the equation.
  template <typename T>
  bool centered_sphere3d<T>::intersect_ray(const ggo::ray3d<T> & ray, T & dist, ggo::ray3d<T> & normal) const
  {
    // Build the quadratic and solve it.
    const ggo::vec3<T> & dir = ray.dir();
    const ggo::pos3<T> & diff = ray.pos();
    
    // Hint 1: deg2 is dot(dir, dir) which is 1
    // Hint 2: actually, deg1 should be 2*dot(dir,diff), but removing 
    // the multiplication simplifies the computations
    T deg1 = ggo::dot(dir, diff); 
    T deg0 = ggo::dot(diff, diff) - _radius * _radius;
    
    // Solve it.
    T d = deg1 * deg1 - deg0;
    if (d < 0)
    {
      return false;
    }
    d = std::sqrt(d);

    // Check the lower solution.
    dist = -deg1 - d;
    if (dist >= 0)
    {
      normal.pos() = ray.pos() + dist * dir;
      normal.set_dir(normal.pos());
      return true;
    }

    // Check the greatest solution (the ray's origin is then inside the spehre, which is a valid case).
    // In that case, the normal points towards the center of the sphere.
    dist = -deg1 + d;
    if (dist >= 0)
    {
      normal.pos() = ray.pos() + dist * dir;
      normal.set_dir(-normal.pos());
      return true;
    }

    return false;
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  ggo::pos3<T> centered_sphere3d<T>::sample_point(const ggo::pos3<T> & target_pos, T random_variable1, T random_variable2) const
  {
    ggo::vec3<T> dir(target_pos);
    dir.normalize();

    return _radius * ggo::hemisphere_sampling(dir, random_variable1, random_variable2);
  }


   //////////////////////////////////////////////////////////////
  template <typename T>
  ggo::ray3d<T> centered_sphere3d<T>::sample_ray(T random_variable1, T random_variable2) const
  {
    ggo::pos3<T> sphere_sample = ggo::sphere_sampling(random_variable1, random_variable2);
    ggo::pos3<T> hemisphere_sample = ggo::hemisphere_sampling(sphere_sample, random_variable1, random_variable2);

    return ggo::ray3d<T>(_radius * sphere_sample, hemisphere_sample, false);
  }
  
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  std::ostream &  operator<<(std::ostream & os, const centered_sphere3d<data_t> & s)
  {
    os << "(radius=" << s.radius() << ")";
    return os;
  }
}
