#ifndef __GGO_CONVOLUTION__
#define __GGO_CONVOLUTION__

#include <ggo_kernel.h>

/////////////////////////////////////////////////////////////////////
// 1D
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename input_iterator_t, typename output_iterator_t, typename left_t, typename right_t, typename kernel_t, typename kernel_size_t>
  void apply_symetric_kernel_1d(input_iterator_t input_it, output_iterator_t output_it, left_t left, right_t right,
    const int count,
    const kernel_t * kernel,
    const kernel_size_t kernel_size)
  {
    static_assert(std::is_integral<kernel_size_t>::value, "expecting integral type");

    for (int x = 0; x < static_cast<int>(kernel_size) - 1; ++x)
    {
      auto v = input_it.read() * kernel[0];
      for (int i = 1; i < static_cast<int>(kernel_size); ++i)
      {
        auto tmp1 = i > x ? left(x - i) : input_it.read(-i);
        auto tmp2 = input_it.read(i);

        v += (tmp1 + tmp2) * kernel[i];
      }

      output_it.write(v);

      output_it.move_nxt();
      input_it.move_nxt();
    }

    for (int x = static_cast<int>(kernel_size) - 1; x <= count - static_cast<int>(kernel_size); ++x)
    {
      auto v = input_it.read() * kernel[0];
      for (int i = 1; i < static_cast<int>(kernel_size); ++i)
      {
        auto tmp1 = input_it.read(-i);
        auto tmp2 = input_it.read(i);
        v += (tmp1 + tmp2) * kernel[i];
      }

      output_it.write(v);

      output_it.move_nxt();
      input_it.move_nxt();
    }

    for (int x = count - static_cast<int>(kernel_size) + 1; x < count; ++x)
    {
      auto v = input_it.read() * kernel[0];
      for (int i = 1; i < static_cast<int>(kernel_size); ++i)
      {
        auto tmp1 = input_it.read(-i);
        auto tmp2 = x + i >= count ? right(x + i) : input_it.read(i);
        v += (tmp1 + tmp2) * kernel[i];
      }

      output_it.write(v);

      output_it.move_nxt();
      input_it.move_nxt();
    }
  }
}
 
/////////////////////////////////////////////////////////////////////
// 2D
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  // Apply 2D symetric kernel horizontally.
  template <typename input_line_iterator_t, typename output_line_iterator_t,
    typename left_t, typename right_t,
    typename kernel_t, typename kernel_size_t>
  void apply_symetric_kernel_2d_horz(input_line_iterator_t input_line_iterator, output_line_iterator_t output_line_iterator,
    left_t left, right_t right, int width, int height,
    const kernel_t * kernel, const kernel_size_t kernel_size)
  {
    for (int y = 0; y < height; ++y)
    {
      auto input_it  = input_line_iterator(y);
      auto output_it = output_line_iterator(y);

      auto left_1d   = [&](int x) { GGO_ASSERT(x < 0);  return left(x, y); };
      auto right_1d  = [&](int x) { GGO_ASSERT(x >= width); return right(x, y); };

      apply_symetric_kernel_1d(input_it, output_it, left_1d, right_1d, width, kernel, kernel_size);
    }
  }

  /////////////////////////////////////////////////////////////////////
  // Apply 2D symetric kernel vertically.
  template <typename input_column_iterator_t, typename output_column_iterator_t,
    typename bottom_t, typename top_t,
    typename kernel_t, typename kernel_size_t>
  void apply_symetric_kernel_2d_vert(input_column_iterator_t input_column_iterator, output_column_iterator_t output_column_iterator,
    bottom_t bottom, top_t top, int width, int height,
    const kernel_t * kernel, const kernel_size_t kernel_size)
  {
    for (int x = 0; x < width; ++x)
    {
      auto input_it  = input_column_iterator(x);
      auto output_it = output_column_iterator(x);

      auto left_1d  = [&](int y) { GGO_ASSERT(y < 0); return bottom(x, y); };
      auto right_1d = [&](int y) { GGO_ASSERT(y >= height); return top(x, y); };

      apply_symetric_kernel_1d(input_it, output_it, left_1d, right_1d, height, kernel, kernel_size);
    }
  }
}

#endif