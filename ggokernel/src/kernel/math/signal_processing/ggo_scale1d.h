#pragma once

#include <kernel/ggo_kernel.h>
#include <kernel/math/interpolation/ggo_interpolation1d.h>
#include <kernel/math/interpolation/ggo_integration1d.h>

// Nearest neighbor.
namespace ggo
{
  //////////////////////////////////////////////////////////////
  inline int nearest_neighbor_index(int i, int size_in, int size_out)
  {
    return i * size_in / size_out + (size_in / size_out) / 2;
  }

  //////////////////////////////////////////////////////////////
  template <typename in_t, typename out_t>
  void scale_1d_nearest_neighbor(in_t && in, int size_in, out_t && out, int size_out)
  {
    for (int i = 0; i < size_out; ++i)
    {
      int i_in = nearest_neighbor_index(i, size_in, size_out);
      out(i, in(i_in));
    }
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  void scale_1d_nearest_neighbor(const data_t * ptr_in, int size_in, data_t * ptr_out, int size_out)
  {
    auto in = [&](int i) { return ptr_in[i]; };
    auto out = [&](int i, const data_t& v) { ptr_out[i] = v; };

    return scale_1d_nearest_neighbor(in, size_in, out, size_out);
  }
}

// Linear and cubic interpolation.
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename scalar_t>
  constexpr scalar_t interpolation_ratio(int size_in, int size_out)
  {
    static_assert(std::is_floating_point_v<scalar_t>);

    return static_cast<scalar_t>(size_in - 1) / static_cast<scalar_t>(size_out - 1);
  }

  static_assert(interpolation_ratio<float>(4, 4) == 1.f);
  static_assert(interpolation_ratio<float>(5, 4) == 4.f / 3.f);
  static_assert(interpolation_ratio<float>(4, 5) == 3.f / 4.f);

  //////////////////////////////////////////////////////////////
  template <typename scalar_t, typename in_t, typename out_t>
  void scale_1d_linear_interpolation(in_t && in, int size_in, out_t && out, int size_out)
  {
    scalar_t ratio = interpolation_ratio<scalar_t>(size_in, size_out);

    for (int i = 0; i < size_out; ++i)
    {
      scalar_t x = ratio * i;
      out(i, linear_interpolation1d(in, x));
    }
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  void scale_1d_linear_interpolation(const data_t * ptr_in, int size_in, data_t * ptr_out, int size_out)
  {
    auto in  = [&](int i) { return ptr_in[i]; };
    auto out = [&](int i, const data_t& v) { ptr_out[i] = v; };

    return scale_1d_linear_interpolation<float>(in, size_in, out, size_out);
  }

  //////////////////////////////////////////////////////////////
  template <typename scalar_t, typename in_t, typename out_t>
  void scale_1d_cubic_interpolation(in_t && in, int size_in, out_t && out, int size_out)
  {
    scalar_t ratio = interpolation_ratio<scalar_t>(size_in, size_out);

    for (int i = 0; i < size_out; ++i)
    {
      scalar_t x = ratio * i;
      out(i, cubic_interpolation1d(in, x));
    }
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  void scale_1d_cubic_interpolation(const data_t* ptr_in, int size_in, data_t* ptr_out, int size_out)
  {
    auto in  = [&](int i) { return ptr_in[ggo::clamp(i, 0, size_in - 1)]; };
    auto out = [&](int i, const data_t& v) { ptr_out[i] = v; };

    return scale_1d_cubic_interpolation<float>(in, size_in, out, size_out);
  }
}

// Linear integration.
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename scalar_t>
  constexpr std::pair<scalar_t, scalar_t> get_integration_boudaries(int size_in, int size_out, int sample_index)
  {
    static_assert(std::is_floating_point_v<scalar_t>);

    scalar_t delta = scalar_t(size_in - 1) / scalar_t(size_out);
    return std::make_pair(sample_index * delta, (sample_index + 1) * delta);
  }

  static_assert(get_integration_boudaries<float>(4, 10, 0) == std::pair<float, float>(0.f, 3.f / 10.0f));
  static_assert(get_integration_boudaries<float>(4, 10, 9) == std::pair<float, float>(27.f / 10.0f, 3.0f));

  //////////////////////////////////////////////////////////////
  template <typename scalar_t, typename data_t, typename in_t, typename out_t>
  void scale_1d_linear_integration(in_t && in, int size_in, out_t&& out, int size_out)
  {
    for (int i = 0; i < size_out; ++i)
    {
      const auto [from, to] = get_integration_boudaries<scalar_t>(size_in, size_out, i);

      out(i, linear_integration1d<data_t>(in, from, to) / (to - from));
    }
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  void scale_1d_linear_integration(const data_t * ptr_in, int size_in, data_t * ptr_out, int size_out)
  {
    auto in  = [&](int i) { return ptr_in[i]; };
    auto out = [&](int i, const data_t& v) { ptr_out[i] = v; };

    return scale_1d_linear_integration<float, data_t>(in, size_in, out, size_out);
  }

  //////////////////////////////////////////////////////////////
  template <typename scalar_t, typename data_t, typename in_t, typename out_t>
  void scale_1d_cubic_integration(in_t&& in, int size_in, out_t&& out, int size_out)
  {
    for (int i = 0; i < size_out; ++i)
    {
      const auto [from, to] = get_integration_boudaries<scalar_t>(size_in, size_out, i);

      out(i, cubic_integration1d<data_t>(in, from, to) / (to - from));
    }
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  void scale_1d_cubic_integration(const data_t* ptr_in, int size_in, data_t* ptr_out, int size_out)
  {
    auto in  = [&](int i) { return ptr_in[ggo::clamp(i, 0, size_in - 1)]; };
    auto out = [&](int i, const data_t& v) { ptr_out[i] = v; };

    return scale_1d_cubic_integration<float, data_t>(in, size_in, out, size_out);
  }
}

