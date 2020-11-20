#pragma once

#include <kernel/memory/ggo_ptr_arithmetics.h>
#include <kernel/ggo_rect_int.h>
#include <kernel/scan/ggo_scan.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename getter, typename setter, typename scan_t, typename predicate>
  void morpho_rectangle(const getter & in, setter out, int width, int height, int kernel_width, int kernel_height, scan_t scan, predicate pred)
  {
    for (auto [x, y] : scan(ggo::rect_int::from_size({ width, height })))
    {
      int y_inf = std::max(0, y - kernel_height);
      int y_sup = std::min(height - 1, y + kernel_height);

      int x_inf = std::max(0, x - kernel_width);
      int x_sup = std::min(width - 1, x + kernel_width);

      auto v = in(x, y);

      for (auto [kernel_x, kernel_y] : scan(ggo::rect_int::from_left_right_bottom_top(x_inf, x_sup, y_inf, y_sup)))
      {
        auto cur = in(kernel_x, kernel_y);

        v = pred(cur, v);
      }

      out(x, y, v);
    }
  }

  //////////////////////////////////////////////////////////////
  template <typename getter, typename setter, typename scan_t, typename predicate>
  void morpho_disc(const getter & in, setter out, int width, int height, float radius, scan_t scan, predicate pred)
  {
    float radius_squared = radius * radius;
    int kernel_size = static_cast<int>(radius + 1);

    for (auto [x, y] : scan(ggo::rect_int::from_size({ width, height })))
    {
      int y_inf = std::max(0, y - kernel_size);
      int y_sup = std::min(height - 1, y + kernel_size);

      int x_inf = std::max(0, x - kernel_size);
      int x_sup = std::min(width - 1, x + kernel_size);

      auto v = in(x, y);

      for (auto [kernel_x, kernel_y] : scan(ggo::rect_int::from_left_right_bottom_top(x_inf, x_sup, y_inf, y_sup)))
      {
        int dx = kernel_x - x;
        int dy = kernel_y - y;
        float hypot = static_cast<float>(dx * dx + dy * dy);

        if (hypot <= radius_squared)
        {
          auto cur = in(kernel_x, kernel_y);

          v = pred(cur, v);
        }
      }

      out(x, y, v);
    }
  }
}

// Helpers.
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  void dilatation_rectangle(const data_t* input, data_t* output, int width, int height, int kernel_width, int kernel_height)
  {
    if (input == output)
    {
      throw std::runtime_error("in-place dilatation not supported");
    }

    auto in = [&](int x, int y) { return input[y * width + x]; };
    auto out = [&](int x, int y, const data_t& v) { output[y * width + x] = v; };
    auto pred = [](const data_t & cur, const data_t & ref) { return std::max(cur, ref); };

    morpho_rectangle(in, out, width, height, kernel_width, kernel_height, scan_rows_up(), pred);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  void dilatation_disc(const data_t* input, data_t* output, int width, int height, float radius)
  {
    if (input == output)
    {
      throw std::runtime_error("in-place dilatation not supported");
    }

    auto in = [&](int x, int y) { return input[y * width + x]; };
    auto out = [&](int x, int y, const data_t & v) { output[y * width + x] = v; };
    auto pred = [](const data_t & cur, const data_t & ref) { return std::max(cur, ref); };

    morpho_disc(in, out, width, height, radius, scan_rows_up(), pred);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  void erosion_rectangle(const data_t* input, data_t* output, int width, int height, int kernel_width, int kernel_height)
  {
    if (input == output)
    {
      throw std::runtime_error("in-place erosion not supported");
    }

    auto in = [&](int x, int y) { return input[y * width + x]; };
    auto out = [&](int x, int y, const data_t & v) { output[y * width + x] = v; };
    auto pred = [](const data_t & cur, const data_t & ref) { return std::min(cur, ref); };

    morpho_rectangle(in, out, width, height, kernel_width, kernel_height, scan_rows_up(), pred);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  void erosion_disc(const data_t * input, data_t * output, int width, int height, float radius)
  {
    if (input == output)
    {
      throw std::runtime_error("in-place erosion not supported");
    }

    auto in = [&](int x, int y) { return input[y * width + x]; };
    auto out = [&](int x, int y, const data_t & v) { output[y * width + x] = v; };
    auto pred = [](const data_t & cur, const data_t & ref) { return std::min(cur, ref); };

    morpho_disc(in, out, width, height, radius, scan_rows_up(), pred);
  }
}

