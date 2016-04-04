#ifndef __GGO_CONVOLUTION__
#define __GGO_CONVOLUTION__

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  // Apply 1D symetric filter.
  template <typename filter_type, typename getter, typename setter>
  void apply_symetric_filter_1d(getter in,
                                setter out,
                                int width,
                                const filter_type * filter,
                                int filter_size)
  {
    for (int x = 0; x < width; ++x)
    {
      auto v = in(x, width) * filter[0];
      for (int i = 1; i < filter_size; ++i)
      {
        int i1 = x + i;
        int i2 = x - i;
        auto tmp1 = in(i1, width);
        auto tmp2 = in(i2, width);
        v += (tmp1 + tmp2) * filter[i];
      }

      out(x, width, v);
    }
  }
}

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  // Apply 2D symetric filter horizontally.
  template <typename filter_type, typename getter, typename setter>
  void apply_symetric_filter_2d_horz(getter in,
                                     setter out,
                                     int width,
                                     int height,
                                     const filter_type * filter,
                                     int filter_size)
  {
    for (int y = 0; y < height; ++y)
    {
      for (int x = 0; x < width; ++x)
      {
        auto v = in(x, y, width, height) * filter[0];
        for (int i = 1; i < filter_size; ++i)
        {
          int x1 = x + i;
          int x2 = x - i;
          auto tmp1 = in(x1, y, width, height);
          auto tmp2 = in(x2, y, width, height);
          v += (tmp1 + tmp2) * filter[i];
        }

        out(x, y, width, height, v);
      }
    }
  }

  /////////////////////////////////////////////////////////////////////
  // Apply 2D symetric filter vertically.
  template <typename filter_type, typename getter, typename setter>
  void apply_symetric_filter_2d_vert(getter in,
                                     setter out,
                                     int width,
                                     int height,
                                     const filter_type * filter,
                                     int filter_size)
  {
    for (int x = 0; x < width; ++x)
    {
      for (int y = 0; y < height; ++y)
      {
        auto v = in(x, y, width, height) * filter[0];
        for (int i = 1; i < filter_size; ++i)
        {
          int y1 = y + i;
          int y2 = y - i;
          auto tmp1 = in(x, y1, width, height);
          auto tmp2 = in(x, y2, width, height);
          v += (tmp1 + tmp2) * filter[i];
        }

        out(x, y, width, height, v);
      }
    }
  }
}

#endif
