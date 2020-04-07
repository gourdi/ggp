#pragma once

#include <kernel/ggo_assert.h>
#include <kernel/ggo_kernel.h>
#include <kernel/math/ggo_discretization.h>
#include <kernel/math/ggo_polynom.h>
#include <kernel/math/interpolation/ggo_interpolation_polynom.h>
#include <algorithm>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename in_t, typename make_polynom_t, typename scalar_t>
  auto integration1d(in_t && in, make_polynom_t && make_polynom, scalar_t from, scalar_t to)
  {
    using data_t = std::decay_t<std::invoke_result_t<in_t, int>>;

    data_t val(0);

    int from_i = lower_sample_index(from);
    int to_i = upper_sample_index(to);

    // Integrate the middle parts.
    for (int i = from_i; i < to_i; ++i)
    {
      val += integrate(make_polynom(i), 0, 1);
    }

    // Remove first block.
    val -= integrate(make_polynom(from_i), scalar_t(0), from - scalar_t(from_i));

    // Remove last block.
    val -= integrate(make_polynom(to_i - 1), to - scalar_t(to_i - 1), scalar_t(1));

    return val;
  }
}

// Linear integration.
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename in_t, typename scalar_t>
  auto linear_integration1d(in_t && in, scalar_t from, scalar_t to)
  {
    auto make_polynom = [&](int lower_sample_index)
    {
      return make_interpolation_linear(in(lower_sample_index), in(lower_sample_index + 1));
    };

    return integration1d(in, make_polynom, from, to);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, typename scalar_t>
  data_t linear_integration1d(const data_t * in, scalar_t from, scalar_t to)
  {
    return linear_integration1d([&](int i) { return in[i]; }, from, to);
  }
}

// Cubic integration.
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename in_t, typename scalar_t>
  auto cubic_integration1d(in_t && in, scalar_t from, scalar_t to)
  {
    auto make_polynom = [&](int lower_sample_index)
    {
      return make_interpolation_cubic(in(lower_sample_index - 1), in(lower_sample_index), in(lower_sample_index + 1), in(lower_sample_index + 2));
    };

    return integration1d(in, make_polynom, from, to);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, typename scalar_t>
  data_t cubic_integration1d(const data_t* in, int size, scalar_t from, scalar_t to)
  {
    return cubic_integration1d([&](int i) { return in[ggo::clamp(i, 0, size - 1)]; }, from, to);
  }
}



