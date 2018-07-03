#ifndef __GGO_NUMERICAL_INTEGRATION__
#define __GGO_NUMERICAL_INTEGRATION__

#include <type_traits>

namespace ggo
{
  template <typename data_t, typename scalar_t, typename velocity_t>
  data_t explicit_euler(const data_t & x_n, scalar_t dt, velocity_t velocity)
  {
    static_assert(std::is_floating_point<scalar_t>::value == true);

    return x_n + dt * velocity(x_n);
  }

  template <typename data_t, typename scalar_t, typename velocity_t>
  data_t runge_kutta(const data_t & x_n, scalar_t dt, velocity_t velocity)
  {
    static_assert(std::is_floating_point<scalar_t>::value == true);

    const auto k1 = velocity(x_n) * dt;
    const auto k2 = velocity(x_n + k1 / scalar_t(2)) * dt;
    const auto k3 = velocity(x_n + k2 / scalar_t(2)) * dt;
    const auto k4 = velocity(x_n + k3) * dt;

    return x_n + (k1 + scalar_t(2) * k2 + scalar_t(2) * k3 + k4) / scalar_t(6);
  }
}

#endif
