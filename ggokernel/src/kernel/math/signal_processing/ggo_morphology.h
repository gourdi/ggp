#ifndef __GGO_MORPHOLOGY__
#define __GGO_MORPHOLOGY__

#include <kernel/memory/ggo_ptr_arithmetics.h>

namespace ggo
{
  template <typename getter, typename setter, typename predicate>
  void morpho_rectangle(const getter & in, setter out, int width, int height, int kernel_width, int kernel_height, predicate pred);
 
  template <typename getter, typename setter, typename predicate>
  void morpho_disc(const getter & in, setter out, int width, int height, float radius, predicate pred);
}

namespace ggo
{
  template <typename data_t>
  void dilatation_rectangle(const data_t * input, data_t * output,
    int width, int height, int input_line_byte_step, int output_line_byte_step,
    int kernel_width, int kernel_height)
  {
    if (input == output)
    {
      throw std::runtime_error("in-place dilatation not supported");
    }

    auto in = [&](int x, int y){ return ggo::move_ptr(input, y * input_line_byte_step)[x]; };
    auto out = [&](int x, int y, const data_t & v) { ggo::move_ptr(output, y * output_line_byte_step)[x] = v; };
    auto pred = [](const data_t & cur, const data_t & ref) { return std::max(cur, ref); };

    morpho_rectangle(in, out, width, height, kernel_width, kernel_height, pred);
  }
  
  template <typename data_t>
  void dilatation_disc(const data_t * input, data_t * output,
    int width, int height, int input_line_byte_step, int output_line_byte_step,
    float radius)
  {
    if (input == output)
    {
      throw std::runtime_error("in-place dilatation not supported");
    }

    auto in = [&](int x, int y) { return ggo::move_ptr(input, y * input_line_byte_step)[x]; };
    auto out = [&](int x, int y, const data_t & v) { ggo::move_ptr(output, y * output_line_byte_step)[x] = v; };
    auto pred = [](const data_t & cur, const data_t & ref) { return std::max(cur, ref); };

    morpho_disc(in, out, width, height, radius, pred);
  }

  template <typename data_t>
  void erosion_rectangle(const data_t * input, data_t * output, 
    int width, int height, int input_line_byte_step, int output_line_byte_step,
    int kernel_width, int kernel_height)
  {
    if (input == output)
    {
      throw std::runtime_error("in-place erosion not supported");
    }

    auto in = [&](int x, int y) { return ggo::move_ptr(input, y * input_line_byte_step)[x]; };
    auto out = [&](int x, int y, const data_t & v) { ggo::move_ptr(output, y * output_line_byte_step)[x] = v; };
    auto pred = [](const data_t & cur, const data_t & ref) { return std::min(cur, ref); };

    morpho_rectangle(in, out, width, height, kernel_width, kernel_height, pred);
  }

  template <typename data_t>
  void erosion_disc(const data_t * input, data_t * output,
    int width, int height, int input_line_byte_step, int output_line_byte_step,
    float radius)
  {
    if (input == output)
    {
      throw std::runtime_error("in-place erosion not supported");
    }

    auto in = [&](int x, int y) { return ggo::ptr_offset(input, y * input_line_byte_step)[x]; };
    auto out = [&](int x, int y, const data_t & v) { ggo::ptr_offset(output, y * output_line_byte_step)[x] = v; };
    auto pred = [](const data_t & cur, const data_t & ref) { return std::min(cur, ref); };

    morpho_disc(in, out, width, height, radius, pred);
  }
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename getter, typename setter, typename predicate>
  void morpho_rectangle(const getter & in, setter out, int width, int height, int kernel_width, int kernel_height, predicate pred)
  {
    for (int y = 0; y < height; ++y)
    {
      int y_inf = std::max(0, y - kernel_height);
      int y_sup = std::min(height - 1, y + kernel_height);

      for (int x = 0; x < width; ++x)
      {
        int x_inf = std::max(0, x - kernel_width);
        int x_sup = std::min(width - 1, x + kernel_width);

        auto v = in(x, y);

        for (int kernel_y = y_inf; kernel_y <= y_sup; ++kernel_y)
        {
          for (int kernel_x = x_inf; kernel_x <= x_sup; ++kernel_x)
          {
            auto cur = in(kernel_x, kernel_y);

            v = pred(cur, v);
          }
        }
        
        out(x, y, v);
      }
    }
  }

  //////////////////////////////////////////////////////////////
  template <typename getter, typename setter, typename predicate>
  void morpho_disc(const getter & in, setter out, int width, int height, float radius, predicate pred)
  {
    float radius_squared = radius * radius;
    int kernel_size = static_cast<int>(radius + 1);

    for (int y = 0; y < height; ++y)
    {
      for (int x = 0; x < width; ++x)
      {
        auto v = in(x, y);

        for (int kernel_y = -kernel_size; kernel_y <= kernel_size; ++kernel_y)
        {
          for (int kernel_x = -kernel_size; kernel_x <= kernel_size; ++kernel_x)
          {
            int input_x = x + kernel_x;
            int input_y = y + kernel_y;

            if (input_x < 0 || input_x >= width ||
                input_y < 0 || input_y >= height)
            {
              continue;
            }

            float hypot = static_cast<float>(kernel_x * kernel_x + kernel_y * kernel_y);

            if (hypot <= radius_squared)
            {
              auto cur = in(input_x, input_y);

              v = pred(cur, v);
            }
          }
        }
        
        out(x, y, v);
      }
    }
  }
}

#endif