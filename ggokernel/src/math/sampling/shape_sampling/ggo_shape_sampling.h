#ifndef __GGO_SHAPE_SAMPLING__
#define __GGO_SHAPE_SAMPLING__

#include <ggo_distance3d.h>
#include <ggo_poisson_sampling.h>

namespace ggo
{
  template <typename T>
  ggo::set2<T> disc_sampling(T random_variable1, T random_variable2)
  {
    GGO_ASSERT_BTW(random_variable1, 0, 1.001);
    GGO_ASSERT_BTW(random_variable2, 0, 1.001);
    
    T r   = std::sqrt(random_variable1);
    T phi = 2 * PI<T>() * random_variable2;
   
    return ggo::set2<T>(std::cos(phi) * r, std::sin(phi) * r);
  }
  
  template <typename T>
  ggo::set2<T> disc_uniform_sampling()
  {
    return disc_sampling(ggo::rand_real<T>(0, 1), ggo::rand_real<T>(0, 1));
  }
  
  template <typename T>
  ggo::set3<T> sphere_sampling(T random_variable1, T random_variable2)
  {
    GGO_ASSERT_BTW(random_variable1, 0, 1.001);
    GGO_ASSERT_BTW(random_variable2, 0, 1.001);
    
    T theta = random_variable1 * 2 * PI<T>();
    T phi   = std::acos(2 * random_variable2 - 1);
    
    return ggo::set3<T>(std::cos(theta) * std::sin(phi), std::sin(theta) * std::sin(phi), std::cos(phi));
  }
  
  template <typename T>
  ggo::set3<T> sphere_uniform_sampling()
  {
    return sphere_sampling(ggo::rand_real<T>(0, 1), ggo::rand_real<T>(0, 1));
  }

  // The main direction is the Z one (ie. [0, 0, 1]).
  template <typename T>
  ggo::set3<T> hemisphere_sampling(T random_variable1, T random_variable2, T theta_max = PI<T>() / 2)
  {
    GGO_ASSERT_BTW(random_variable1, 0, 1.001);
    GGO_ASSERT_BTW(random_variable2, 0, 1.001);
    
    T z_min = std::cos(theta_max);
    T z     = ggo::map<T>(random_variable1, 0, 1, z_min, 1);
    T phi   = random_variable2 * 2 * PI<T>();
    T r     = std::sqrt(1 - z * z);

    return ggo::set3<T>(std::cos(phi) * r, std::sin(phi) * r, z);
  }
  
  template <typename T>
  ggo::set3<T> hemisphere_sampling(const ggo::set3<T> & main_dir, T random_variable1, T random_variable2, T theta_max = PI<T>() / 2)
  {
    auto sample = hemisphere_sampling(random_variable1, random_variable2, theta_max);
    
    ggo::set3<T> base1, base2;
    main_dir.build_basis(base1, base2);
    
    sample = base1 * sample.x() + base2 * sample.y() + main_dir * sample.z();
    GGO_ASSERT_GE(ggo::dot(sample, main_dir), 0);
    GGO_ASSERT(sample.is_normalized(T(0.001)));

    return sample;
  }
  
  template <typename T>
  ggo::set3<T> hemisphere_uniform_sampling(T theta_max = PI<T>() / 2)
  {
    return hemisphere_sampling(ggo::rand_real<T>(0, 1), ggo::rand_real<T>(0, 1), theta_max);
  }
  
  template <typename T>
  std::vector<ggo::set3<T>> hemisphere_poisson_sampling(T theta_max, T radius, int tries_count)
  {
    return poisson_sampling<ggo::set3<T>, T>(
      [theta_max](){ return ggo::hemisphere_uniform_sampling<T>(theta_max); },
      [](const ggo::set3<T> & p1, const ggo::set3<T> & p2){ return ggo::distance(p1, p2); },
      radius,
      tries_count);
  }
}

#endif
