#ifndef __GGO_SCANNER2D__
#define __GGO_SCANNER2D__

#include <kernel/ggo_rect_int.h>

namespace ggo
{
  struct scanner2d_lines_up
  {
    template <typename func_t>
    static void for_each_pixel(const ggo::rect_int & clipping, func_t && func)
    {
      const auto b = clipping.bottom();
      const auto t = clipping.top();
      const auto l = clipping.left();
      const auto r = clipping.right();

      for (int y = b; y <= t; ++y)
      {
        for (int x = l; x <= r; ++x)
        {
          func(x, y);
        }
      }
    }
  };

  struct scanner2d_lines_down
  {
    template <typename func_t>
    static void for_each_pixel(const ggo::rect_int & clipping, func_t && func)
    {
      const auto b = clipping.bottom();
      const auto t = clipping.top();
      const auto l = clipping.left();
      const auto r = clipping.right();

      for (int y = t; y >= b; --y)
      {
        for (int x = l; x <= r; ++x)
        {
          func(x, y);
        }
      }
    }
  };
}

#endif
