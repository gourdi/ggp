#pragma once

#include <kernel/ggo_kernel.h>
#include <kernel/math/signal_processing/ggo_scale1d.h>
#include <kernel/math/interpolation/ggo_interpolation2d.h>
#include <kernel/math/interpolation/ggo_integration2d.h>

// Nearest neightbor.
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename in_t, typename out_t>
  void scale_2d_nearest_neighbor(in_t && in, int width_in, int height_in, out_t && out, int width_out, int height_out)
  {
    for (int y = 0; y < height_out; ++y)
    {
      auto y_in = nearest_neighbor_index(y, height_in, height_out);

      for (int x = 0; x < width_out; ++x)
      {
        auto x_in = nearest_neighbor_index(x, width_in, width_out);
        out(x, y, in(x_in, y_in));
      }
    }
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  void scale_2d_nearest_neighbor(const data_t * ptr_in, int width_in, int height_in, data_t * ptr_out, int width_out, int height_out)
  {
    auto in  = [&](int x, int y) { return ptr_in[y * width_in + x]; };
    auto out = [&](int x, int y, const data_t& v) { ptr_out[y * width_out + x] = v; };

    scale_2d_nearest_neighbor(in, width_in, height_in, out, width_out, height_out);
  }
}

// Bilinear and bicubic interpolation.
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename in_t, typename out_t, typename scalar_t = float>
  void scale_2d_linear_interpolation(in_t && in, int width_in, int height_in, out_t && out, int width_out, int height_out)
  {
    scalar_t ratio_x = interpolation_ratio<scalar_t>(width_in, width_out);
    scalar_t ratio_y = interpolation_ratio<scalar_t>(height_in, height_out);

    for (int y = 0; y < height_out; ++y)
    {
      scalar_t y_in = ratio_y * y;

      for (int x = 0; x < width_out; ++x)
      {
        scalar_t x_in = ratio_x * x;

        out(x, y, linear_interpolation2d(in, x_in, y_in));
      }
    }
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  void scale_2d_linear_interpolation(const data_t * ptr_in, int width_in, int height_in, data_t * ptr_out, int width_out, int height_out)
  {
    auto in  = [&](int x, int y) { return ptr_in[y * width_in + x]; };
    auto out = [&](int x, int y, const data_t& v) { ptr_out[y * width_out + x] = v; };

    scale_2d_linear_interpolation(in, width_in, height_in, out, width_out, height_out);
  }

  //////////////////////////////////////////////////////////////
  template <typename in_t, typename out_t, typename scalar_t = float>
  void scale_2d_cubic_interpolation(in_t&& in, int width_in, int height_in, out_t&& out, int width_out, int height_out)
  {
    scalar_t ratio_x = interpolation_ratio<scalar_t>(width_in, width_out);
    scalar_t ratio_y = interpolation_ratio<scalar_t>(height_in, height_out);

    for (int y = 0; y < height_out; ++y)
    {
      scalar_t y_in = ratio_y * y;

      for (int x = 0; x < width_out; ++x)
      {
        scalar_t x_in = ratio_x * x;

        out(x, y, cubic_interpolation2d(in, x_in, y_in));
      }
    }
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  void scale_2d_cubic_interpolation(const data_t * ptr_in, int width_in, int height_in, data_t * ptr_out, int width_out, int height_out)
  {
    auto in  = [&](int x, int y) { return ptr_in[std::clamp(y, 0, height_in - 1) * width_in + std::clamp(x, 0, width_in - 1)]; };
    auto out = [&](int x, int y, const data_t& v) { ptr_out[y * width_out + x] = v; };

    scale_2d_cubic_interpolation(in, width_in, height_in, out, width_out, height_out);
  }
}

// Bilinear integration.
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename in_t, typename out_t, typename scalar_t = float>
  void scale_2d_linear_integration(in_t && in, int width_in, int height_in, out_t && out, int width_out, int height_out)
  {
    for (int y = 0; y < height_out; ++y)
    {
      for (int x = 0; x < width_out; ++x)
      {
        const auto [x_inf, x_sup] = get_integration_boudaries<scalar_t>(width_in,  width_out,  x);
        const auto [y_inf, y_sup] = get_integration_boudaries<scalar_t>(height_in, height_out, y);

        out(x, y, linear_integration2d(in, x_inf, x_sup, y_inf, y_sup) / ((x_sup - x_inf) * (y_sup - y_inf)));
      }
    }
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  void scale_2d_linear_integration(const data_t * ptr_in, int width_in, int height_in, data_t * ptr_out, int width_out, int height_out)
  {
    auto in  = [&](int x, int y) { return ptr_in[y * width_in + x]; };
    auto out = [&](int x, int y, const data_t & v) { ptr_out[y * width_out + x] = v; };

    scale_2d_linear_integration(in, width_in, height_in, out, width_out, height_out);
  }
}
