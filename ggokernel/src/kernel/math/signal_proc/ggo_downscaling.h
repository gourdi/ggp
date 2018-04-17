#ifndef __GGO_DOWNSCALING__
#define __GGO_DOWNSCALING__

#include <kernel/ggo_kernel.h>

namespace ggo
{
  template <typename input, typename output>
  void downscale2x2_t(input in, output out, int width, int height);

  template <typename input, typename output>
  void downscale4x4_t(input in, output out, int width, int height);
}

namespace ggo
{
  template <int stride_in, int stride_out, typename data_t>
  void downscale2x2(const data_t * in, data_t * out, int width, int height)
  {
    auto in_func = [&](int x, int y) { return in[stride_in * (y * width + x)]; };
    auto out_func = [&](int x, int y, data_t v) { out[stride_out * (y * width + x)] = v; };

    downscale2x2_t(in_func, out_func, width, height);
  }

  template <typename data_t>
  void downscale2x2(const data_t * in, data_t * out, int width, int height)
  {
    downscale2x2<1, 1, data_t>(in, out, width, height);
  }

  template <int stride_in, int stride_out, typename data_t>
  void downscale4x4(const data_t * in, data_t * out, int width, int height)
  {
    auto in_func = [&](int x, int y) { return in[stride_in * (y * width + x)]; };
    auto out_func = [&](int x, int y, data_t v) { out[stride_out * (y * width + x)] = v; };

    downscale4x4_t(in_func, out_func, width, height);
  }

  template <typename data_t>
  void downscale4x4(const data_t * in, data_t * out, int width, int height)
  {
    downscale4x4<1, 1, data_t>(in, out, width, height);
  }
}

namespace ggo
{
  template <typename input, typename output>
  void downscale2x2_t(input in, output out, int width, int height)
  {
    if ((width % 2) != 0 || (height % 2) != 0)
    {
      std::runtime_error("invalid input size");
    }

    for (int y = 0; y < height; y += 2)
    {
      for (int x = 0; x < width; x += 2)
      {
        out(x / 2, y / 2, ggo::average(in(x, y), in(x + 1, y), in(x, y + 1), in(x + 1, y + 1)));
      }
    }
  }

  template <typename input, typename output>
  void downscale4x4_t(input in, output out, int width, int height)
  {
    if ((width % 4) != 0 || (height % 4) != 0)
    {
      std::runtime_error("invalid input size");
    }

    for (int y = 0; y < height; y += 4)
    {
      for (int x = 0; x < width; x += 4)
      {
        out(x / 4, y / 4, ggo::average(in(x + 0, y + 0), in(x + 1, y + 0), in(x + 2, y + 0), in(x + 3, y + 0),
                                       in(x + 0, y + 1), in(x + 1, y + 1), in(x + 2, y + 1), in(x + 3, y + 1),
                                       in(x + 0, y + 2), in(x + 1, y + 2), in(x + 2, y + 2), in(x + 3, y + 2),
                                       in(x + 0, y + 3), in(x + 1, y + 3), in(x + 2, y + 3), in(x + 3, y + 3)));
      }
    }
  }
}

#endif
