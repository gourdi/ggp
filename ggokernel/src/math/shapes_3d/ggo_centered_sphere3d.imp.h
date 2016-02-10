#include <ggo_shape_sampling.h>
#include <ggo_best_candidate_sequence.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  // Solve the equation (x-center_x)^2+(y-center_y)^2+(z-center_z)^2=r^2
  // injecting the ray into the equation.
  template <typename T>
  bool centered_sphere3d<T>::intersect_ray(const ggo::ray3d<T> & ray, T & dist, ggo::ray3d<T> & normal) const
  {
    // Build the quadratic and solve it.
    const ggo::set3<T> & dir = ray.dir();
    const ggo::set3<T> & diff = ray.pos();
    
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
  ggo::set3<T> centered_sphere3d<T>::sample_point(const ggo::set3<T> & target_pos, T random_variable1, T random_variable2) const
  {
    ggo::set3<T> dir(target_pos);
    dir.normalize();

    return _radius * ggo::hemisphere_sampling(dir, random_variable1, random_variable2);
  }


   //////////////////////////////////////////////////////////////
  template <typename T>
  std::vector<ggo::ray3d<T>> centered_sphere3d<T>::sample_rays(int samples_count) const
  {
    std::vector<ggo::ray3d_float> rays;

    for (int i = 0; i < samples_count; ++i)
    {
      ggo::vector3d_float sphere_sample = ggo::sphere_sampling(ggo::best_candidate_table[i].x(), ggo::best_candidate_table[i].y());
      ggo::vector3d_float hemisphere_sample = ggo::hemisphere_sampling(sphere_sample, ggo::best_candidate_table[i].x(), ggo::best_candidate_table[i].y());

      rays.emplace_back(_radius * sphere_sample, hemisphere_sample, false);
    }

    return rays;
  }
  
  //////////////////////////////////////////////////////////////
  template <typename T>
  std::string centered_sphere3d<T>::desc() const
  {
    std::ostringstream oss;
    oss << "sphere3d (" << _radius << ")";
    return oss.str();
  }
}
