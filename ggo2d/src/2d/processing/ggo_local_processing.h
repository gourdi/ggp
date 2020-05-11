#ifndef __GGO_2D_LOCAL_PROCESSING__
#define __GGO_2D_LOCAL_PROCESSING__

#include <kernel/math/signal_processing/ggo_local_processing.h>
#include <2d/ggo_image.h>

// Generic functions.
namespace ggo
{
  template <typename input_image_t, typename output_image_t, typename left_t, typename right_t, typename processing_t>
  void apply_horizontal_processing(const input_image_t & in, output_image_t & out,
    left_t && handle_left_border, right_t && handle_right_border,
    int processing_left_size, int processing_right_size, processing_t && processing)
  {
    if (in.size() != out.size())
    {
      throw std::runtime_error("size mismatch");
    }

    const int w = in.width();
    const int h = in.height();

    for (int y = 0; y < h; ++y)
    {
      int x = 0;

      {
        auto left_input = [&](int delta)
        {
          auto x_delta = x + delta;

          return x_delta < 0 ? handle_left_border(x_delta, y) : in.read_pixel(x_delta, y);
        };

        int x_end = processing_left_size;
        for (; x < x_end; ++x)
        {
          out.write_pixel(x, y, processing(left_input));
        }
      }

      {
        auto center_input = [&](int delta)
        {
          return in.read_pixel(x + delta, y);
        };

        int x_end = w - processing_right_size;
        for (; x < x_end; ++x)
        {
          out.write_pixel(x, y, processing(center_input));
        }
      }

      {
        auto right_input = [&](int delta)
        {
          auto x_delta = x + delta;

          return x_delta >= w ? handle_right_border(x_delta, y) : in.read_pixel(x_delta, y);
        };

        for (; x < w; ++x)
        {
          out.write_pixel(x, y, processing(right_input));
        }
      }
    }
  }

  template <border_mode border_mode, typename input_image_t, typename output_image_t, typename processing_t>
  void apply_horizontal_processing(const input_image_t & in, output_image_t & out,
    int processing_left_size, int processing_right_size, processing_t && processing)
  {
    const int w = in.width();

    auto handle_border = [&](int x, int y)
    {
      if constexpr (border_mode == ggo::border_mode::zero)
      {
        return typename output_image_t::color_t(0);
      }
      else
      {
        return in.read_pixel(index<border_mode>(x, w), y);
      }
    };

    apply_horizontal_processing(in, out, handle_border, handle_border,
      processing_left_size, processing_right_size, processing);
  }

  template <typename input_image_t, typename output_image_t, typename bottom_t, typename top_t, typename processing_t>
  void apply_vertical_processing(const input_image_t & in, output_image_t & out,
    bottom_t && handle_bottom_border, top_t && handle_top_border,
    int processing_bottom_size, int processing_top_size, processing_t && processing)
  {
    if (in.size() != out.size())
    {
      throw std::runtime_error("size mismatch");
    }

    const int w = in.width();
    const int h = in.height();

    for (int x = 0; x < w; ++x)
    {
      int y = 0;

      {
        auto bottom_input = [&](int delta)
        {
          auto y_delta = y + delta;

          return y_delta < 0 ? handle_bottom_border(x, y_delta) : in.read_pixel(x, y_delta);
        };

        int y_end = processing_bottom_size;
        for (; y < y_end; ++y)
        {
          out.write_pixel(x, y, processing(bottom_input));
        }
      }

      {
        auto center_input = [&](int delta)
        {
          return in.read_pixel(x, y + delta);
        };

        int y_end = h - processing_top_size;
        for (; y < y_end; ++y)
        {
          out.write_pixel(x, y, processing(center_input));
        }
      }

      {
        auto top_input = [&](int delta)
        {
          auto y_delta = y + delta;

          return y_delta >= h ? handle_top_border(x, y_delta) : in.read_pixel(x, y_delta);
        };

        for (; y < h; ++y)
        {
          out.write_pixel(x, y, processing(top_input));
        }
      }
    }
  }

  template <border_mode border_mode, typename input_image_t, typename output_image_t, typename processing_t>
  void apply_vertical_processing(const input_image_t & in, output_image_t & out,
    int processing_bottom_size, int processing_top_size, processing_t && processing)
  {
    const int h = in.height();

    auto handle_border = [&](int x, int y)
    {
      if constexpr (border_mode == ggo::border_mode::zero)
      {
        return typename output_image_t::color_t(0);
      }
      else
      {
        return in.read_pixel(x, index<border_mode>(y, h));
      }
    };

    apply_vertical_processing(in, out, handle_border, handle_border,
      processing_bottom_size, processing_top_size, processing);
  }
}

#endif
