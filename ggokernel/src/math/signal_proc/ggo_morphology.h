#ifndef __GGO_MORPHOLOGY__
#define __GGO_MORPHOLOGY__

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename getter, typename setter, typename predicate>
  void morpho_rectangle(const getter & in, setter out, int width, int height, int kernel_width, int kernel_height, predicate pred)
  {
    for (int y = 0; y < height; ++y)
    {
      for (int x = 0; x < width; ++x)
      {
        auto ref = in(x, y);
        out(x, y, ref);

        for (int kernel_y = -kernel_height; kernel_y <= kernel_height; ++kernel_y)
        {
          if ((y + kernel_y < 0) || (y + kernel_y >= height)) { continue; }

          for (int kernel_x = -kernel_width; kernel_x <= kernel_width; ++kernel_x)
          {
            if ((x + kernel_x < 0) || (x + kernel_x >= width)) { continue; }

            auto cur = in(x + kernel_x, y + kernel_y);

            if (pred(cur, ref))
            {
              out(x, y, cur);
            }
          }
        }
      }
    }
  }

  //////////////////////////////////////////////////////////////
  template <typename getter, typename setter, typename predicate>
  void morpho_disc_kernel_t(const getter & in, setter out, int width, int height, float radius, predicate pred)
  {
    float radius_squared = radius * radius;
    int kernel_size = static_cast<int>(radius + 1);

    for (int y = 0; y < height; ++y)
    {
      for (int x = 0; x < width; ++x)
      {
        auto ref = in(x, y);
        out(x, y, ref);

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
              auto cur = in(input_x, input_y);

              if (pred(cur, ref))
              {
                out(x, y, cur);
              }
            }
          }
        }
      }
    }
  }
}

#endif
