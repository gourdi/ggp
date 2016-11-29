#ifndef __GGO_SHAPE_SAMPLING__
#define __GGO_SHAPE_SAMPLING__

#include <ggo_distance.h>
#include <ggo_poisson_sampling.h>

namespace ggo
{
  template <typename T>
  ggo::pos2<T> disc_sampling(T random_variable1, T random_variable2)
  {
    static_assert(std::is_floating_point<T>::value, "expecting floating point type");

    GGO_ASSERT_BTW(random_variable1, 0, 1.001);
    GGO_ASSERT_BTW(random_variable2, 0, 1.001);
    
    T r   = std::sqrt(random_variable1);
    T phi = 2 * pi<T>() * random_variable2;
   
    return ggo::pos2<T>(std::cos(phi) * r, std::sin(phi) * r);
  }
  
  template <typename T>
  ggo::pos2<T> disc_uniform_sampling()
  {
    return disc_sampling(ggo::rand<T>(0, 1), ggo::rand<T>(0, 1));
  }

  template <typename T>
  ggo::pos2<T> disc_uniform_sampling(const ggo::pos2<T> & center, T radius)
  {
    return center + radius * disc_sampling(ggo::rand<T>(), ggo::rand<T>());
  }
  
  template <typename T>
  ggo::pos3<T> sphere_sampling(T random_variable1, T random_variable2)
  {
    static_assert(std::is_floating_point<T>::value, "expecting floating point type");

    GGO_ASSERT_BTW(random_variable1, 0, 1.001);
    GGO_ASSERT_BTW(random_variable2, 0, 1.001);
    
    T theta = random_variable1 * 2 * pi<T>();
    T phi   = std::acos(2 * random_variable2 - 1);
    
    return ggo::pos3<T>(std::cos(theta) * std::sin(phi), std::sin(theta) * std::sin(phi), std::cos(phi));
  }
  
  template <typename T>
  ggo::pos3<T> sphere_uniform_sampling()
  {
    return sphere_sampling(ggo::rand<T>(0, 1), ggo::rand<T>(0, 1));
  }

  // The main direction is the Z one (ie. [0, 0, 1]).
  template <typename T>
  ggo::pos3<T> hemisphere_sampling(T random_variable1, T random_variable2, T theta_max = pi<T>() / 2)
  {
    GGO_ASSERT_BTW(random_variable1, 0, 1.001);
    GGO_ASSERT_BTW(random_variable2, 0, 1.001);
    
    T z_min = std::cos(theta_max);
    T z     = ggo::map<T, T>(random_variable1, 0, 1, z_min, 1);
    T phi   = random_variable2 * 2 * pi<T>();
    T r     = std::sqrt(1 - z * z);

    return ggo::pos3<T>(std::cos(phi) * r, std::sin(phi) * r, z);
  }
  
  template <typename T>
  ggo::pos3<T> hemisphere_sampling(const ggo::vec3<T> & main_dir, T random_variable1, T random_variable2, T theta_max = pi<T>() / 2)
  {
    auto sample = hemisphere_sampling(random_variable1, random_variable2, theta_max);
    
    auto b = build_basis(main_dir);
    
    sample = b.first * sample.x() + b.second * sample.y() + main_dir * sample.z();
    GGO_ASSERT_GE(ggo::dot(sample, main_dir), 0);
    GGO_ASSERT(sample.is_normalized(T(0.001)));

    return sample;
  }
  
  template <typename T>
  ggo::pos3<T> hemisphere_uniform_sampling(T theta_max = pi<T>() / 2)
  {
    return hemisphere_sampling(ggo::rand<T>(0, 1), ggo::rand<T>(0, 1), theta_max);
  }
  
  template <typename T>
  std::vector<ggo::pos3<T>> hemisphere_poisson_sampling(T theta_max, T radius, int tries_count)
  {
    return poisson_sampling<ggo::pos3<T>, T>(
      [theta_max](){ return ggo::hemisphere_uniform_sampling<T>(theta_max); },
      [](const ggo::pos3<T> & p1, const ggo::pos3<T> & p2){ return ggo::distance(p1, p2); },
      radius,
      tries_count);
  }
}

#endif
