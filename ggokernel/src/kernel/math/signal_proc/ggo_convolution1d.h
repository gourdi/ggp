#ifndef __GGO_CONVOLUTION_1D__
#define __GGO_CONVOLUTION_1D__

#include <kernel/ggo_kernel.h>
#include <kernel/ggo_borders.h>
#include <stdexcept>

namespace ggo
{
  template <border_mode border, typename read_t, typename write_t, typename kernel_t>
  void symmetric_convolution(read_t read, write_t write, int size, int left, int right, const kernel_t * kernel, int kernel_size)
  {
    GGO_ASSERT(left >= 0 && right < size);

    if constexpr (border == border_mode::in_memory)
    {
      for (int i = left; i <= right; ++i)
      {
        auto v = kernel[0] * read(i);
        for (int j = 1; j < kernel_size; ++j)
        {
          v += kernel[j] * (read(i - j) + read(i + j));
        }
        write(i, v);
      }
    }
    else
    {
      // The kernel is larger than data.
      if (size < 2 * kernel_size - 1)
      {
        for (int i = left; i <= right; ++i)
        {
          auto v = kernel[0] * read(i);
          for (int j = 1; j < kernel_size; ++j)
          {
            v += kernel[j] * (read(inf_index<border>(i - j, size)) + read(sup_index<border>(i + j, size)));
          }
          write(i, v);
        }
      }
      else
      {
        {
          // Left part, need to handle left border.
          int i = left;
          for (; i < kernel_size - 1; ++i)
          {
            auto v = kernel[0] * read(i);
            for (int j = 1; j < kernel_size; ++j)
            {
              v += kernel[j] * (read(inf_index<border>(i - j, size)) + read(i + j));
            }
            write(i, v);
          }

          // Central part, no need to handle borders.
          int center_sup = std::min(right, size - kernel_size);
          for (; i <= center_sup; ++i)
          {
            auto v = kernel[0] * read(i);
            for (int j = 1; j < kernel_size; ++j)
            {
              v += kernel[j] * (read(i - j) + read(i + j));
            }
            write(i, v);
          }

          // Right part, need to handle right border.
          for (; i <= right; ++i)
          {
            auto v = kernel[0] * read(i);
            for (int j = 1; j < kernel_size; ++j)
            {
              v += kernel[j] * (read(i - j) + read(ggo::sup_index<border>(i + j, size)));
            }
            write(i, v);
          }
        }
      }
    }
  }

  template <border_mode border, typename data_t, typename kernel_t>
  void symmetric_convolution(const data_t * in, data_t * out, int size, int left, int right, const kernel_t * kernel, int kernel_size)
  {
    if (in == out)
    {
      throw std::runtime_error("in-place convolution not supported");
    }

    auto read = [&](int i) { return in[i]; };
    auto write = [&](int i, data_t v) { out[i] = v; };

    symmetric_convolution<border>(read, write, size, left, right, kernel, kernel_size);
  }

  template <border_mode border, typename data_t, typename kernel_t>
  void symmetric_convolution(const data_t * in, data_t * out, int size, const kernel_t * kernel, int kernel_size)
  {
    symmetric_convolution<border>(in, out, size, 0, size - 1, kernel, kernel_size);
  }
}

#endif