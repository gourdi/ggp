#ifndef __GGO_SHAPE_SAMPLING__
#define __GGO_SHAPE_SAMPLING__

#include <kernel/ggo_vec2.h>
#include <kernel/ggo_vec3.h>
#include <kernel/math/ggo_distance.h>
#include <kernel/math/sampling/low_discrepancy_sequences/ggo_poisson_sampling.h>

namespace ggo
{
  template <typename data_t>
  ggo::pos2<data_t> disc_sampling(data_t random_variable1, data_t random_variable2)
  {
    static_assert(std::is_floating_point<data_t>::value, "expecting floating point type");

    GGO_ASSERT_BTW(random_variable1, 0, 1.001);
    GGO_ASSERT_BTW(random_variable2, 0, 1.001);
    
    data_t r   = std::sqrt(random_variable1);
    data_t phi = 2 * pi<data_t>() * random_variable2;
   
    return ggo::pos2<data_t>(std::cos(phi) * r, std::sin(phi) * r);
  }
  
  template <typename data_t>
  ggo::pos2<data_t> disc_uniform_sampling()
  {
    return disc_sampling(ggo::rand<data_t>(0, 1), ggo::rand<data_t>(0, 1));
  }

  template <typename data_t>
  ggo::pos2<data_t> disc_uniform_sampling(const ggo::pos2<data_t> & center, data_t radius)
  {
    return center + radius * disc_sampling(ggo::rand<data_t>(), ggo::rand<data_t>());
  }
  
  template <typename data_t>
  ggo::pos3<data_t> sphere_sampling(data_t random_variable1, data_t random_variable2)
  {
    static_assert(std::is_floating_point<data_t>::value, "expecting floating point type");

    GGO_ASSERT_BTW(random_variable1, 0, 1.001);
    GGO_ASSERT_BTW(random_variable2, 0, 1.001);
    
    data_t theta = random_variable1 * 2 * pi<data_t>();
    data_t phi   = std::acos(2 * random_variable2 - 1);
    
    return ggo::pos3<data_t>(std::cos(theta) * std::sin(phi), std::sin(theta) * std::sin(phi), std::cos(phi));
  }
  
  template <typename data_t>
  ggo::pos3<data_t> sphere_uniform_sampling()
  {
    return sphere_sampling(ggo::rand<data_t>(0, 1), ggo::rand<data_t>(0, 1));
  }

  // The main direction is the Z one (ie. [0, 0, 1]).
  template <typename data_t>
  ggo::pos3<data_t> hemisphere_sampling(data_t random_variable1, data_t random_variable2, data_t theta_max = pi<data_t>() / 2)
  {
    GGO_ASSERT_BTW(random_variable1, 0, 1.001);
    GGO_ASSERT_BTW(random_variable2, 0, 1.001);
    
    data_t z_min = std::cos(theta_max);
    data_t z     = ggo::map<data_t, data_t>(random_variable1, 0, 1, z_min, 1);
    data_t phi   = random_variable2 * 2 * pi<data_t>();
    data_t r     = std::sqrt(1 - z * z);

    return ggo::pos3<data_t>(std::cos(phi) * r, std::sin(phi) * r, z);
  }
  
  template <typename data_t>
  ggo::pos3<data_t> hemisphere_sampling(const ggo::vec3<data_t> & main_dir, data_t random_variable1, data_t random_variable2, data_t theta_max = pi<data_t>() / 2)
  {
    auto sample = hemisphere_sampling(random_variable1, random_variable2, theta_max);
    
    auto b = build_basis(main_dir);
    
    sample = b.first * sample.x() + b.second * sample.y() + main_dir * sample.z();
    GGO_ASSERT_GE(ggo::dot(sample, main_dir), 0);
    GGO_ASSERT(is_normalized(sample, data_t(0.001)));

    return sample;
  }
  
  template <typename data_t>
  ggo::pos3<data_t> hemisphere_uniform_sampling(data_t theta_max = pi<data_t>() / 2)
  {
    return hemisphere_sampling(ggo::rand<data_t>(0, 1), ggo::rand<data_t>(0, 1), theta_max);
  }
  
  template <typename data_t>
  std::vector<ggo::pos3<data_t>> hemisphere_poisson_sampling(data_t theta_max, data_t radius, int tries_count)
  {
    return poisson_sampling<ggo::pos3<data_t>, data_t>(
      [theta_max](){ return ggo::hemisphere_uniform_sampling<data_t>(theta_max); },
      [](const ggo::pos3<data_t> & p1, const ggo::pos3<data_t> & p2){ return ggo::distance(p1, p2); },
      radius,
      tries_count);
  }
}

#endif
