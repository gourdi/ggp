#ifndef __GGO_SCALE2D__
#define __GGO_SCALE2D__

#include <kernel/math/signal_proc/ggo_scale1d.h>
#include <kernel/memory/ggo_ptr_offset.h>
#include <kernel/memory/ggo_buffer2d.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <scaling_algo horz_algo, scaling_algo vert_algo, typename data_t, typename real_t, typename input, typename output>
  void scale_2d(input in, int width_in, int height_in, output out, int width_out, int height_out)
  {
    if constexpr(horz_algo == scaling_algo::nearest_neighbor && vert_algo == scaling_algo::nearest_neighbor)
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
    else
    {
      ggo::buffer2d_t<data_t> temp(width_out, height_in);

      // First pass to scale horizontally.
      for (int y = 0; y < height_in; ++y)
      {
        auto horz_in = [&](int x)
        {
          if constexpr(horz_algo == scaling_algo::cubic_integration)
          {
            x = ggo::clamp<int>(x, 0, static_cast<int>(width_in - 1));
          }
          return in(x, y); 
        };

        auto horz_out = [&](int x, data_t v)
        {
          temp(x, y) = v;
        };

        scale_1d<horz_algo, data_t, real_t>(horz_in, width_in, horz_out, width_out);
      }

      // Second pass to scale vertically.
      for (int x = 0; x < width_out; ++x)
      {
        auto vert_in = [&](int y)
        {
          if constexpr(vert_algo == scaling_algo::cubic_integration)
          {
            y = ggo::clamp<int>(y, 0, static_cast<int>(height_in - 1));
          }
          return temp(x, y);
        };

        auto vert_out = [&](int y, data_t v) 
        {
          out(x, y, v);
        };

        scale_1d<vert_algo, data_t, real_t>(vert_in, height_in, vert_out, height_out);
      }
    }
  }

  //////////////////////////////////////////////////////////////
  template <scaling_algo horz_algo, scaling_algo vert_algo, typename data_t, typename size_t>
  void	scale_2d(const data_t * input, size_t width_in, size_t height_in, size_t line_byte_step_in,
    data_t * output, size_t width_out, size_t height_out, size_t line_byte_step_out)
  {
    static_assert(std::is_integral<size_t>::value == true);

    auto in = [&](int x, int y)
    {
      return ggo::ptr_offset(input, y * static_cast<int>(line_byte_step_in))[x];
    };

    auto out = [&](int x, int y, data_t v)
    {
      ggo::ptr_offset(output, y * static_cast<int>(line_byte_step_out))[x] = v;
    };

    scale_2d<horz_algo, vert_algo, data_t, data_t>(
      in, static_cast<int>(width_in), static_cast<int>(height_in),
      out, static_cast<int>(width_out), static_cast<int>(height_out));
  }
}

#endif
