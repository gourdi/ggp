#ifndef __GGO_SCAN2D__
#define __GGO_SCAN2D__

#include <kernel/ggo_rect_int.h>

namespace ggo
{
  struct scanner2d_lines_up
  {
    template <typename func>
    static void for_each_pixel(const ggo::rect_int & clipping, func && f)
    {
      const auto b = clipping.bottom();
      const auto t = clipping.top();
      const auto l = clipping.left();
      const auto r = clipping.right();

      for (int y = b; y <= t; ++y)
      {
        for (int x = l; x <= r; ++x)
        {
          f(x, y);
        }
      }
    }
  };

  struct scanner2d_lines_down
  {
    template <typename func>
    static void for_each_pixel(const ggo::rect_int & clipping, func && f)
    {
      const auto b = clipping.bottom();
      const auto t = clipping.top();
      const auto l = clipping.left();
      const auto r = clipping.right();

      for (int y = t; y >= b; --y)
      {
        for (int x = l; x <= r; ++x)
        {
          f(x, y);
        }
      }
    }
  };
}

#endif
