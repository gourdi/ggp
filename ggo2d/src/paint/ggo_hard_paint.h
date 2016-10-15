#ifndef __GGO_HARD_PAINT__
#define __GGO_HARD_PAINT__

#include <ggo_2d.h>

// Hard line.
namespace ggo
{
  template <typename real_t, typename paint_pixel_func>
  void paint_line(int width, int height, real_t x_from, real_t y_from, real_t x_to, real_t y_to, paint_pixel_func paint_pixel);
}

// Hard line implementation.
namespace ggo
{
  template <typename real_t, typename paint_pixel_func>
  void paint_line(int width, int height, real_t x_from, real_t y_from, real_t x_to, real_t y_to, paint_pixel_func paint_pixel)
  {
    // Clamp line coordinates.

    // Make sure 'from' is on the left of 'to'.
    if (x_from > x_to)
    {
      std::swap(x_from, x_to);
      std::swap(y_from, y_to);
    }

    // Left clamp.
    real_t t_left = -x_from / (x_to - x_from);
    if (t_left > 1) { return; }
    if (t_left > 0)
    {
      x_from = x_from + t_left * (x_to - x_from);
      y_from = y_from + t_left * (y_to - y_from);
    }

    // Right clamp.
    real_t t_right = (width - x_from) / (x_to - x_from);
    if (t_right < 0) { return; }
    if (t_right < 1)
    {
      x_to = x_from + t_right * (x_to - x_from);
      y_to = y_from + t_right * (y_to - y_from);
    }

    // Make sure 'from' is below 'to'.
    if (y_from > y_to)
    {
      std::swap(x_from, x_to);
      std::swap(y_from, y_to);
    }

    // Bottom clamp.
    real_t t_bottom = -y_from / (y_to - y_from);
    if (t_bottom > 1) { return; }
    if (t_bottom > 0)
    {
      x_from = x_from + t_bottom * (x_to - x_from);
      y_from = y_from + t_bottom * (y_to - y_from);
    }

    // Top clamp.
    real_t t_top = (height - y_from) / (y_to - y_from);
    if (t_top < 0) { return; }
    if (t_top < 1)
    {
      x_to = x_from + t_top * (x_to - x_from);
      y_to = y_from + t_top * (y_to - y_from);
    }

    int x_from_i = ggo::clamp(ggo::to<int>(x_from), 0, width - 1);
    int y_from_i = ggo::clamp(ggo::to<int>(y_from), 0, height - 1);
    int x_to_i = ggo::clamp(ggo::to<int>(x_to), 0, width - 1);
    int y_to_i = ggo::clamp(ggo::to<int>(y_to), 0, height - 1);

    // Since the first point isn't painted, well, paint it.
    paint_pixel(x_from_i, y_from_i);

    // Paint the line.
    int dx = x_from_i - x_to_i;
    int dy = y_from_i - y_to_i;
    if (abs(y_from_i - y_to_i) > abs(x_from_i - x_to_i))
    {
      if (y_from_i > y_to_i)
      {
        std::swap(x_from_i, x_to_i);
        std::swap(y_from_i, y_to_i);
      }

      for (int y = y_from_i; y != y_to_i; ++y)
      {
        int x = static_cast<int>(x_from + (y - y_from_i) * dx / dy);
        paint_pixel(x, y);
      }
    }
    else
    {
      if (x_from_i > x_to_i)
      {
        std::swap(x_from_i, x_to_i);
        std::swap(y_from_i, y_to_i);
      }

      for (int x = x_from_i; x != x_to_i; ++x)
      {
        int y = static_cast<int>(y_from + (x - x_from_i) * dy / dx);
        paint_pixel(x, y);
      }
    }
  }
}

#endif
