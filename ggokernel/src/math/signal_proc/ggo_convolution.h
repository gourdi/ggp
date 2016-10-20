#ifndef __GGO_CONVOLUTION__
#define __GGO_CONVOLUTION__

#include <ggo_buffer_access.h>

/////////////////////////////////////////////////////////////////////
// 1D
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  // Generic version.
  template <typename kernel_t, typename left_t, typename center_t, typename right_t, typename output_t, typename count_t>
  void apply_symetric_kernel_1d(left_t left, center_t center, right_t right,
                                output_t output,
                                const int width,
                                const kernel_t * kernel,
                                const count_t kernel_size)
  {
    static_assert(std::is_integral<count_t>::value, "expecting integral type");

    for (int x = 0; x < static_cast<int>(kernel_size) - 1; ++x)
    {
      auto v = center(x) * kernel[0];
      for (int i = 1; i < static_cast<int>(kernel_size); ++i)
      {
        auto tmp1 = i > x ? left(x - i) : center(x - i);
        auto tmp2 = center(x + i);

        v += (tmp1 + tmp2) * kernel[i];
      }

      output(x, v);
    }

    for (int x = static_cast<int>(kernel_size) - 1; x <= width - static_cast<int>(kernel_size); ++x)
    {
      auto v = center(x) * kernel[0];
      for (int i = 1; i < static_cast<int>(kernel_size); ++i)
      {
        auto tmp1 = center(x - i);
        auto tmp2 = center(x + i);
        v += (tmp1 + tmp2) * kernel[i];
      }

      output(x, v);
    }

    for (int x = width - static_cast<int>(kernel_size) + 1; x < width; ++x)
    {
      auto v = center(x) * kernel[0];
      for (int i = 1; i < static_cast<int>(kernel_size); ++i)
      {
        auto tmp1 = center(x - i);
        auto tmp2 = x + i >= width ? right(x + i) : center(x + i);
        v += (tmp1 + tmp2) * kernel[i];
      }

      output(x, v);
    }
  }

  /////////////////////////////////////////////////////////////////////
  // Buffer version.
  template <typename kernel_t, typename read_t, typename write_t, typename left_t, typename right_t, typename count_t>
  void apply_symetric_kernel_1d(const void * input_buffer, const int input_item_step, read_t read, 
    void * output_buffer, const int output_item_step, write_t write,
    left_t left, right_t right,
    const int width,
    const kernel_t * kernel,
    const count_t kernel_size)
  {
    static_assert(std::is_integral<count_t>::value, "expecting integral type");

    for (int x = 0; x < static_cast<int>(kernel_size) - 1; ++x)
    {
      auto v = read(input_buffer) * kernel[0];
      for (int i = 1; i < static_cast<int>(kernel_size); ++i)
      {
        auto tmp1 = i > x ? left(x - i) : read(static_cast<const char*>(input_buffer) - i * input_item_step);
        auto tmp2 = read(static_cast<const char*>(input_buffer) + i * input_item_step);

        v += (tmp1 + tmp2) * kernel[i];
      }

      write(output_buffer, v);

      input_buffer = ptr_offset(input_buffer, input_item_step);
      output_buffer = ptr_offset(output_buffer, output_item_step);
    }

    for (int x = static_cast<int>(kernel_size) - 1; x <= width - static_cast<int>(kernel_size); ++x)
    {
      auto v = read(input_buffer) * kernel[0];
      for (int i = 1; i < static_cast<int>(kernel_size); ++i)
      {
        auto tmp1 = read(static_cast<const char*>(input_buffer) - i * input_item_step);
        auto tmp2 = read(static_cast<const char*>(input_buffer) + i * input_item_step);
        v += (tmp1 + tmp2) * kernel[i];
      }

      write(output_buffer, v);

      input_buffer = ptr_offset(input_buffer, input_item_step);
      output_buffer = ptr_offset(output_buffer, input_item_step);
    }

    for (int x = width - static_cast<int>(kernel_size) + 1; x < width; ++x)
    {
      auto v = read(input_buffer) * kernel[0];
      for (int i = 1; i < static_cast<int>(kernel_size); ++i)
      {
        auto tmp1 = read(static_cast<const char*>(input_buffer) - i * input_item_step);
        auto tmp2 = x + i >= width ? right(x + i) : read(static_cast<const char*>(input_buffer) + i * input_item_step);
        v += (tmp1 + tmp2) * kernel[i];
      }

      write(output_buffer, v);

      input_buffer = ptr_offset(input_buffer, input_item_step);
      output_buffer = ptr_offset(output_buffer, input_item_step);
    }
  }
}

/////////////////////////////////////////////////////////////////////
// 2D, buffer API only
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  // Apply 2D symetric kernel horizontally, buffer version.
  template <y_direction y_dir, typename kernel_t, typename read_t, typename write_t, typename left_t, typename right_t, typename count_t>
  void apply_symetric_kernel_2d_horz(const void * input_buffer, const int input_item_step, const int input_line_step, read_t read,
    void * output_buffer, const int output_item_step, const int output_line_step, write_t write,
    left_t left, right_t right, int width, int height,
    const kernel_t * kernel, const count_t kernel_size)
  {
    for (int y = 0; y < height; ++y)
    {
      int mapped_y   = y_dir == y_down ? height - y - 1 : y;
      auto left_1d   = [&](int x) { GGO_ASSERT(x < 0);  return left(x, mapped_y); };
      auto right_1d  = [&](int x) { GGO_ASSERT(x >= width); return right(x, mapped_y); };

      apply_symetric_kernel_1d(input_buffer, input_item_step, read, output_buffer, output_item_step, write,
        left_1d, right_1d, width, kernel, kernel_size);

      input_buffer = ptr_offset(input_buffer, input_line_step);
      output_buffer = ptr_offset(output_buffer, output_line_step);
    }
  }

  /////////////////////////////////////////////////////////////////////
  // Apply 2D symetric kernel horizontally, buffer version.
  template <y_direction y_dir, typename kernel_t, typename read_t, typename write_t, typename bottom_t, typename top_t, typename count_t>
  void apply_symetric_kernel_2d_vert(const void * input_buffer, const int input_item_step, const int input_line_step, read_t read,
    void * output_buffer, const int output_item_step, const int output_line_step, write_t write,
    bottom_t bottom, top_t top, int width, int height,
    const kernel_t * kernel, const count_t kernel_size)
  {
    for (int x = 0; x < width; ++x)
    {
      auto left_1d  = [&](int y) { GGO_ASSERT(y < 0); return y_dir == y_down ? top(x, height - y - 1) : bottom(x, y); };
      auto right_1d = [&](int y) { GGO_ASSERT(y >= height); return y_dir == y_down ? bottom(x, height - y - 1) : top(x, y); };

      apply_symetric_kernel_1d(input_buffer, input_line_step, read, output_buffer, output_line_step, write,
        left_1d, right_1d, height, kernel, kernel_size);

      input_buffer = ptr_offset(input_buffer, input_item_step);
      output_buffer = ptr_offset(output_buffer, output_item_step);
    }
  }
}

#endif