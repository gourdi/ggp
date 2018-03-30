#ifndef __GGO_PBF_MORPHOLOGY__
#define __GGO_PBF_MORPHOLOGY__

#include <ggo_pixel_buffer_format.h>
#include <ggo_morphology.h>

// Definition.
namespace ggo
{
  // Dilatation.
  template <typename color_t>
  void dilatation_rectangle(const void * input, void * output, int width, int height, int line_byte_step, int kernel_width, int kernel_height);

  template <typename color_t>
  void dilatation_disc(const void * input, void * output, int width, int height, int line_byte_step, float radius);

  // Erosion.
  template <typename color_t>
  void erosion_rectangle(const void * input, void * output, int width, int height, int line_byte_step, int kernel_width, int kernel_height);

  template <typename color_t>
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
  struct morpho_t<color_8u>
  {
    static color_8u dilatation(color_8u c1, color_8u c2) { return color_8u(std::max(c1.r(), c2.r()), std::max(c1.g(), c2.g()), std::max(c1.b(), c2.b())); }
    static color_8u erosion(color_8u c1, color_8u c2) { return color_8u(std::min(c1.r(), c2.r()), std::min(c1.g(), c2.g()), std::min(c1.b(), c2.b())); }
  };

  // Dilatation.
  template <pixel_buffer_format pbf>
  void dilatation_rectangle(const void * input, void * output, int width, int height, int line_byte_step, int kernel_width, int kernel_height)
  {
    if (input == output)
    {
      throw inplace_exception();
    }

    using pbf_info = pixel_buffer_format_info<pbf>;
    using color_t = pbf_info::color_t;

    auto in = [&](int x, int y) { return read_pixel<pbf>(input, x, y, height, line_byte_step); };
    auto out = [&](int x, int y, const color_t & c) { write_pixel<pbf>(output, x, y, height, line_byte_step, c); };
    auto pred = [](const color_t & c1, const color_t & c2) { return morpho_t<color_t>::dilatation(c1, c2); };

    morpho_rectangle(in, out, width, height, kernel_width, kernel_height, pred);
  }

  template <pixel_buffer_format pbf>
  void dilatation_disc(const void * input, void * output, int width, int height, int line_byte_step, float radius)
  {
    if (input == output)
    {
      throw inplace_exception();
    }

    using pbf_info = pixel_buffer_format_info<pbf>;
    using color_t = pbf_info::color_t;

    auto in = [&](int x, int y) { return read_pixel<pbf>(input, x, y, height, line_byte_step); };
    auto out = [&](int x, int y, const color_t & c) { write_pixel<pbf>(output, x, y, height, line_byte_step, c); };
    auto pred = [](const color_t & c1, const color_t & c2) { return morpho_t<color_t>::dilatation(c1, c2); };

    morpho_disc(in, out, width, height, radius, pred);
  }

  // Erosion.
  template <pixel_buffer_format pbf>
  void erosion_rectangle(const void * input, void * output, int width, int height, int line_byte_step, int kernel_width, int kernel_height)
  {
    if (input == output)
    {
      throw inplace_exception();
    }

    using pbf_info = pixel_buffer_format_info<pbf>;
    using color_t = pbf_info::color_t;

    auto in = [&](int x, int y) { return read_pixel<pbf>(input, x, y, height, line_byte_step); };
    auto out = [&](int x, int y, const color_t & c) { write_pixel<pbf>(output, x, y, height, line_byte_step, c); };
    auto pred = [](const color_t & c1, const color_t & c2) { return morpho_t<color_t>::erosion(c1, c2); };

    morpho_rectangle(in, out, width, height, kernel_width, kernel_height, pred);
  }

  template <pixel_buffer_format pbf>
  void erosion_disc(const void * input, void * output, int width, int height, int line_byte_step, float radius)
  {
    if (input == output)
    {
      throw inplace_exception();
    }

    using pbf_info = pixel_buffer_format_info<pbf>;
    using color_t = pbf_info::color_t;

    auto in = [&](int x, int y) { return read_pixel<pbf>(input, x, y, height, line_byte_step); };
    auto out = [&](int x, int y, const color_t & c) { write_pixel<pbf>(output, x, y, height, line_byte_step, c); };
    auto pred = [](const color_t & c1, const color_t & c2) { return morpho_t<color_t>::erosion(c1, c2); };

    morpho_disc(in, out, width, height, radius, pred);
  }
}

#endif
