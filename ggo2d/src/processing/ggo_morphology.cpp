#include "ggo_morphology.h"
#include <functional>

namespace
{
  //////////////////////////////////////////////////////////////
  template <typename T, typename Compare>
  void morpho_rectangle_kernel_t(const T * input, T * output, int width, int height, int kernel_width, int kernel_height, Compare comp)
  {
    for (int y = 0; y < height; ++y)
    {
      for (int x = 0; x < width; ++x)
      {
        output[y * width + x] = input[y * width + x];

        for (int kernel_y = -kernel_height; kernel_y <= kernel_height; ++kernel_y)
        {
          if ((y + kernel_y < 0) || (y + kernel_y >= height)) { continue; }

          for (int kernel_x = -kernel_width; kernel_x <= kernel_width; ++kernel_x)
          {
            if ((x + kernel_x < 0) || (x + kernel_x >= width)) { continue; }

            T cur = input[(y + kernel_y) * width + x + kernel_x];

            if (comp(cur, output[y * width + x]))
            {
              output[y * width + x] = cur;
            }
          }
        }
      }
    }
  }

  //////////////////////////////////////////////////////////////
  template <typename T, typename Compare>
  void morpho_disc_kernel_t(const T * input, T * output, int width, int height, float radius, Compare comp)
  {
    float radius_squared = radius * radius;
    int kernel_size = static_cast<int>(radius + 1);

    for (int y = 0; y < height; ++y)
    {
      for (int x = 0; x < width; ++x)
      {
        output[y * width + x] = input[y * width + x];

        for (int kernel_y = -kernel_size; kernel_y <= kernel_size; ++kernel_y)
        {
          for (int kernel_x = -kernel_size; kernel_x <= kernel_size; ++kernel_x)
          {
            float hypot = static_cast<float>(kernel_y * kernel_y + kernel_x * kernel_x);
            int input_x = x + kernel_x;
            int input_y = y + kernel_y;

            if (input_x >= 0 && input_x < width &&
                input_y >= 0 && input_y < height &&
                hypot <= radius_squared)
            {
              T cur = input[input_y * width + input_x];

              if (comp(cur, output[y * width + x]))
              {
                output[y * width + x] = cur;
              }
            }
          }
        }
      }
    }
  }
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  void dilate_rectangle_kernel(const uint8_t * input, uint8_t * output, int width, int height, int kernel_width, int kernel_height)
  {
    morpho_rectangle_kernel_t(input, output, width, height, kernel_width, kernel_height, std::greater<int>());
  }

  //////////////////////////////////////////////////////////////
  void erode_rectanglee_kernel(const uint8_t * input, uint8_t * output, int width, int height, int kernel_width, int kernel_height)
  {
    morpho_rectangle_kernel_t(input, output, width, height, kernel_width, kernel_height, std::less<int>());
  }

  //////////////////////////////////////////////////////////////
  void dilate_circle_kernel(const uint8_t * input, uint8_t * output, int width, int height, float radius)
  {
    morpho_disc_kernel_t(input, output, width, height, radius, std::greater<int>());
  }

  //////////////////////////////////////////////////////////////
  void erode_circle_kernel(const uint8_t * input, uint8_t * output, int width, int height, float radius)
  {
    morpho_disc_kernel_t(input, output, width, height, radius, std::less<int>());
  }
}
