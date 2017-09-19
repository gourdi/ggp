#ifndef __GGO_MORPHOLOGY__
#define __GGO_MORPHOLOGY__

namespace ggo
{
  template <typename getter, typename setter, typename predicate>
  void morpho_rectangle(const getter & in, setter out, int width, int height, int kernel_width, int kernel_height, predicate pred);
 
  template <typename getter, typename setter, typename predicate>
  void morpho_disc(const getter & in, setter out, int width, int height, float radius, predicate pred);
}

namespace ggo
{
  template <typename data_t, int stride_in = 1, int stride_out = 1>
  void dilatation_rectangle(const data_t * input, data_t * output, int width, int height, int kernel_width, int kernel_height)
  {
    if (input == output)
    {
      throw inplace_exception();
    }

    auto in = [&](int x, int y){ return get2d_mirror<data_t, stride_in>(input, x, y, width, height); };
    auto out = [&](int x, int y, const data_t & v){ set2d<data_t, stride_out>(output, x, y, width, height, v); };
    auto pred = [](const data_t & cur, const data_t & ref) { return cur > ref; };
    
    morpho_rectangle(in, out, width, height, kernel_width, kernel_height, pred);
  }
  
  template <typename data_t, int stride_in = 1, int stride_out = 1>
  void dilatation_disc(const data_t * input, data_t * output, int width, int height, float radius)
  {
    if (input == output)
    {
      throw inplace_exception();
    }

    auto in = [&](int x, int y){ return get2d_mirror<data_t, stride_in>(input, x, y, width, height); };
    auto out = [&](int x, int y, const data_t & v){ set2d<data_t, stride_out>(output, x, y, width, height, v); };
    auto pred = [](const data_t & cur, const data_t & ref) { return cur > ref; };
    
    morpho_disc(in, out, width, height, radius, pred);
  }

  template <typename data_t, int stride_in = 1, int stride_out = 1>
  void erosion_rectangle(const data_t * input, data_t * output, int width, int height, int kernel_width, int kernel_height)
  {
    if (input == output)
    {
      throw inplace_exception();
    }

    auto in = [&](int x, int y) { return get2d_mirror<data_t, stride_in>(input, x, y, width, height); };
    auto out = [&](int x, int y, const data_t & v) { set2d<data_t, stride_out>(output, x, y, width, height, v); };
    auto pred = [](const data_t & cur, const data_t & ref) { return cur < ref; };

    morpho_rectangle(in, out, width, height, kernel_width, kernel_height, pred);
  }

  template <typename data_t, int stride_in = 1, int stride_out = 1>
  void erosion_disc(const data_t * input, data_t * output, int width, int height, float radius)
  {
    if (input == output)
    {
      throw inplace_exception();
    }

    auto in = [&](int x, int y) { return get2d_mirror<data_t, stride_in>(input, x, y, width, height); };
    auto out = [&](int x, int y, const data_t & v) { set2d<data_t, stride_out>(output, x, y, width, height, v); };
    auto pred = [](const data_t & cur, const data_t & ref) { return cur < ref; };

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
      for (int x = 0; x < width; ++x)
      {
        auto v = in(x, y);

        for (int kernel_y = -kernel_height; kernel_y <= kernel_height; ++kernel_y)
        {
          if ((y + kernel_y < 0) || (y + kernel_y >= height)) { continue; }

          for (int kernel_x = -kernel_width; kernel_x <= kernel_width; ++kernel_x)
          {
            if ((x + kernel_x < 0) || (x + kernel_x >= width)) { continue; }

            auto cur = in(x + kernel_x, y + kernel_y);

            if (pred(cur, v))
            {
              v = cur;
            }
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
            float hypot = static_cast<float>(kernel_x * kernel_x + kernel_y * kernel_y);
            int input_x = x + kernel_x;
            int input_y = y + kernel_y;

            if (input_x >= 0 && input_x < width &&
                input_y >= 0 && input_y < height &&
                hypot <= radius_squared)
            {
              auto cur = in(input_x, input_y);

              if (pred(cur, v))
              {
                v = cur;
              }
            }
          }
        }
        
        out(x, y, v);
      }
    }
  }
}

#endif
