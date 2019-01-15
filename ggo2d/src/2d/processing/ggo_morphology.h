#ifndef __GGO_2D_MORPHOLOGY__
#define __GGO_2D_MORPHOLOGY__

#include <2d/ggo_image_format.h>
#include <kernel/math/signal_proc/ggo_morphology.h>

// Definition.
namespace ggo
{
  // Dilatation.
  template <image_format format>
  void dilatation_rectangle(const void * input, void * output, int width, int height, int line_byte_step, int kernel_width, int kernel_height);

  template <image_format format>
  void dilatation_disc(const void * input, void * output, int width, int height, int line_byte_step, float radius);

  // Erosion.
  template <image_format format>
  void erosion_rectangle(const void * input, void * output, int width, int height, int line_byte_step, int kernel_width, int kernel_height);

  template <image_format format>
  void erosion_disc(const void * input, void * output, int width, int height, int line_byte_step, float radius);
}

// Implementation.
namespace ggo
{
  template <typename color_t>
  struct morpho_t
  {
  };

  template <>
  struct morpho_t<uint8_t>
  {
    static uint8_t dilatation(uint8_t c1, uint8_t c2) { return std::max(c1, c2); }
    static uint8_t erosion(uint8_t c1, uint8_t c2) { return std::min(c1, c2); }
  };

  template <>
  struct morpho_t<rgb_8u>
  {
    static rgb_8u dilatation(rgb_8u c1, rgb_8u c2) { return rgb_8u(std::max(c1.r(), c2.r()), std::max(c1.g(), c2.g()), std::max(c1.b(), c2.b())); }
    static rgb_8u erosion(rgb_8u c1, rgb_8u c2) { return rgb_8u(std::min(c1.r(), c2.r()), std::min(c1.g(), c2.g()), std::min(c1.b(), c2.b())); }
  };

  // Dilatation.
  template <image_format format>
  void dilatation_rectangle(const void * input, void * output, int width, int height, int line_byte_step, int kernel_width, int kernel_height)
  {
    if (input == output)
    {
      throw inplace_exception();
    }

    using format_info = image_format_info<format>;
    using color_t = format_info::color_t;

    auto in = [&](int x, int y) { return read_pixel<format>(input, x, y, height, line_byte_step); };
    auto out = [&](int x, int y, const color_t & c) { write_pixel<format>(output, x, y, height, line_byte_step, c); };
    auto pred = [](const color_t & c1, const color_t & c2) { return morpho_t<color_t>::dilatation(c1, c2); };

    morpho_rectangle(in, out, width, height, kernel_width, kernel_height, pred);
  }

  template <image_format format>
  void dilatation_disc(const void * input, void * output, int width, int height, int line_byte_step, float radius)
  {
    if (input == output)
    {
      throw std::runtime_error("inplace unsupported");
    }

    using format_traits = image_format_traits<format>;
    using color_t = format_traits::color_t;

    auto in = [&](int x, int y) { return read_pixel<format>(input, x, y, height, line_byte_step); };
    auto out = [&](int x, int y, const color_t & c) { write_pixel<format>(output, x, y, height, line_byte_step, c); };
    auto pred = [](const color_t & c1, const color_t & c2) { return morpho_t<color_t>::dilatation(c1, c2); };

    morpho_disc(in, out, width, height, radius, pred);
  }

  // Erosion.
  template <image_format format>
  void erosion_rectangle(const void * input, void * output, int width, int height, int line_byte_step, int kernel_width, int kernel_height)
  {
    if (input == output)
    {
      throw std::runtime_error("inplace unsupported");
    }

    using format_traits = image_format_traits<format>;
    using color_t = format_traits::color_t;

    auto in = [&](int x, int y) { return read_pixel<format>(input, x, y, height, line_byte_step); };
    auto out = [&](int x, int y, const color_t & c) { write_pixel<format>(output, x, y, height, line_byte_step, c); };
    auto pred = [](const color_t & c1, const color_t & c2) { return morpho_t<color_t>::erosion(c1, c2); };

    morpho_rectangle(in, out, width, height, kernel_width, kernel_height, pred);
  }

  template <image_format format>
  void erosion_disc(const void * input, void * output, int width, int height, int line_byte_step, float radius)
  {
    if (input == output)
    {
      throw inplace_exception();
    }

    using format_info = image_format_info<format>;
    using color_t = format_info::color_t;

    auto in = [&](int x, int y) { return read_pixel<format>(input, x, y, height, line_byte_step); };
    auto out = [&](int x, int y, const color_t & c) { write_pixel<format>(output, x, y, height, line_byte_step, c); };
    auto pred = [](const color_t & c1, const color_t & c2) { return morpho_t<color_t>::erosion(c1, c2); };

    morpho_disc(in, out, width, height, radius, pred);
  }
}

#endif
