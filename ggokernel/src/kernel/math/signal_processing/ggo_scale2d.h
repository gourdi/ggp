#ifndef __GGO_SCALE2D__
#define __GGO_SCALE2D__

#include <kernel/ggo_kernel.h>
#include <kernel/math/signal_processing/ggo_scale1d.h>
#include <kernel/math/interpolation/ggo_interpolation2d.h>
#include <kernel/math/ggo_pixel_sampling.h>
#include <kernel/memory/ggo_scanner2d.h>
#include <kernel/memory/ggo_tiling.h>
#include <kernel/memory/ggo_array.h>
#include <kernel/threading/ggo_threadpool.h>

//////////////////////////////////////////////////////////////
// Sampling base scaling.
namespace ggo
{
  template <
    ggo::pixel_sampling sampling,
    ggo::interpolation2d_type interp,
    typename scanner2d_t,
    typename tiling_y,
    typename scalar_t,
    typename in_t,
    typename out_t
  >
  void resample(in_t && in, int width_in, int height_in, out_t && out, int width_out, int height_out)
  {
    static_assert(std::is_floating_point_v<scalar_t>);

    using data_t = std::result_of<in_t(int, int)>::type;

    const scalar_t horz_ratio = scalar_t(width_in) / scalar_t(width_out);
    const scalar_t vert_ratio = scalar_t(height_in) / scalar_t(height_out);

    auto in_clampped = [&](int x, int y)
    {
      if constexpr (interp == interpolation2d_type::bicublic)
      {
        x = ggo::clamp<int>(x, 0, width_in - 1);
        y = ggo::clamp<int>(y, 0, height_in - 1);
      }
      return in(x, y);
    };

    auto process = [&](ggo::rect_int tile)
    {
      scanner2d_t::for_each_pixel(tile, [&](int x, int y)
      {
        data_t v(0);
        ggo::sampler<sampling>::sample_pixel<scalar_t>(x, y, [&](scalar_t x_f, scalar_t y_f)
        {
          v += ggo::interpolation2d<interp, data_t>(in_clampped, x_f * horz_ratio, y_f * vert_ratio);
        });

        out(x, y, v / scalar_t(sampler<sampling>::samples_count));
      });
    };

    if (threadpool::global())
    {
      std::vector<std::future<void>> futures;

      auto tiles = tiling_y::make_tiles(ggo::rect_int::from_size({ width_out, height_out }), int(threadpool::global()->threads_count()));

      for (const auto & tile : tiles)
      {
        futures.emplace_back(threadpool::global()->enqueue(process, tile));
      }

      for (const auto & f : futures)
      {
        f.wait();
      }
    }
    else
    {
      process(ggo::rect_int::from_size({ width_out, height_out }));
    }
  }
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <scaling_algo algo, typename scanner2d_t, typename tiling_y, typename scalar_t, typename in_t, typename out_t>
  void scale_2d(in_t && in, int width_in, int height_in, out_t && out, int width_out, int height_out)
  {
    if constexpr(algo == scaling_algo::nearest_neighbor)
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
    else if constexpr (algo == scaling_algo::linear_integration || algo == scaling_algo::cubic_integration)
    {
      using data_t = typename std::result_of<in_t(int, int)>::type;

      ggo::array<data_t, 2> temp(width_out, height_in);

      // First pass to scale horizontally.
      for (int y = 0; y < height_in; ++y)
      {
        auto horz_in = [&](int x)
        {
          if constexpr(algo == scaling_algo::cubic_integration)
          {
            x = ggo::clamp<int>(x, 0, static_cast<int>(width_in - 1));
          }
          return in(x, y); 
        };

        auto horz_out = [&](int x, data_t v)
        {
          temp(x, y) = v;
        };

        scale_1d<algo, data_t, scalar_t>(horz_in, width_in, horz_out, width_out);
      }

      // Second pass to scale vertically.
      for (int x = 0; x < width_out; ++x)
      {
        auto vert_in = [&](int y)
        {
          if constexpr(algo == scaling_algo::cubic_integration)
          {
            y = ggo::clamp<int>(y, 0, static_cast<int>(height_in - 1));
          }
          return temp(x, y);
        };

        auto vert_out = [&](int y, data_t v) 
        {
          out(x, y, v);
        };

        scale_1d<algo, data_t, scalar_t>(vert_in, height_in, vert_out, height_out);
      }
    }
    else
    {
      switch (algo)
      {
      case scaling_algo::linear_resampling_1:
        resample<ggo::sampling_1, ggo::interpolation2d_type::bilinear, scanner2d_t, tiling_y, scalar_t>(
          in, width_in, height_in, out, width_out, height_out);
        break;
      case scaling_algo::linear_resampling_2x2:
        resample<ggo::sampling_2x2, ggo::interpolation2d_type::bilinear, scanner2d_t, tiling_y, scalar_t>(
          in, width_in, height_in, out, width_out, height_out);
        break;
      case scaling_algo::linear_resampling_4x4:
        resample<ggo::sampling_4x4, ggo::interpolation2d_type::bilinear, scanner2d_t, tiling_y, scalar_t>(
          in, width_in, height_in, out, width_out, height_out);
        break;
      case scaling_algo::linear_resampling_8x8:
        resample<ggo::sampling_8x8, ggo::interpolation2d_type::bilinear, scanner2d_t, tiling_y, scalar_t>(
          in, width_in, height_in, out, width_out, height_out);
        break;
      case scaling_algo::linear_resampling_16x16:
        resample<ggo::sampling_16x16, ggo::interpolation2d_type::bilinear, scanner2d_t, tiling_y, scalar_t>(
          in, width_in, height_in, out, width_out, height_out);
        break;
      case scaling_algo::cubic_resampling_1:
        resample<ggo::sampling_1, ggo::interpolation2d_type::bicublic, scanner2d_t, tiling_y, scalar_t>(
          in, width_in, height_in, out, width_out, height_out);
        break;
      case scaling_algo::cubic_resampling_2x2:
        resample<ggo::sampling_2x2, ggo::interpolation2d_type::bicublic, scanner2d_t, tiling_y, scalar_t>(
          in, width_in, height_in, out, width_out, height_out);
        break;
      case scaling_algo::cubic_resampling_4x4:
        resample<ggo::sampling_4x4, ggo::interpolation2d_type::bicublic, scanner2d_t, tiling_y, scalar_t>(
          in, width_in, height_in, out, width_out, height_out);
        break;
      case scaling_algo::cubic_resampling_8x8:
        resample<ggo::sampling_8x8, ggo::interpolation2d_type::bicublic, scanner2d_t, tiling_y, scalar_t>(
          in, width_in, height_in, out, width_out, height_out);
        break;
      case scaling_algo::cubic_resampling_16x16:
        resample<ggo::sampling_16x16, ggo::interpolation2d_type::bicublic, scanner2d_t, tiling_y, scalar_t>(
          in, width_in, height_in, out, width_out, height_out);
        break;
      }
    }
  }

  //////////////////////////////////////////////////////////////
  template <scaling_algo algo, typename data_t>
  void scale_2d(const data_t * input, int width_in, int height_in, data_t * output, int width_out, int height_out)
  {
    auto in = [&](int x, int y)
    {
      return input[y * width_in + x];
    };

    auto out = [&](int x, int y, data_t v)
    {
      output[y * width_out + x] = v;
    };

    scale_2d<algo, scanner2d_lines_up, vertical_tiling, data_t>(in, width_in, height_in, out, width_out, height_out);
  }
}

#endif
