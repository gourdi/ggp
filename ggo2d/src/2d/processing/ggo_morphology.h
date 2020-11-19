#pragma once

#if 0

#include <kernel/math/signal_processing/ggo_morphology.h>
#include <2d/ggo_pixel_type.h>
#include <2d/ggo_image.h>

// Definition.
namespace ggo
{
  // Dilatation.
  template <typename input_image_t, typename output_image_t>
  void dilatation_rectangle(const input_image_t & input, output_image_t & output, int kernel_width, int kernel_height);

  template <typename input_image_t, typename output_image_t>
  void dilatation_disc(const input_image_t & input, output_image_t & output, float radius);

  // Erosion.
  template <typename input_image_t, typename output_image_t>
  void erosion_rectangle(const input_image_t & input, output_image_t & output, int kernel_width, int kernel_height);

  template <typename input_image_t, typename output_image_t>
  void erosion_disc(const input_image_t & input, output_image_t & output, float radius);
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
  template <typename input_image_t, typename output_image_t>
  void dilatation_rectangle(const input_image_t & input, output_image_t & output, int kernel_width, int kernel_height)
  {
    static_assert(std::is_same_v<input_image_t::color_t, output_image_t::color_t>);

    using color_t = typename input_image_t::color_t;

    if (input.size() != output.size())
    {
      throw std::runtime_error("dimension mismatch");
    }

    auto in = [&](int x, int y) { return input.read_pixel(x, y); };
    auto out = [&](int x, int y, const color_t & c) { output.write_pixel(x, y, c); };
    auto pred = [](const color_t & c1, const color_t & c2) { return morpho_t<color_t>::dilatation(c1, c2); };

    morpho_rectangle(in, out, input.width(), input.height(), kernel_width, kernel_height, pred);
  }

  template <typename input_image_t, typename output_image_t>
  void dilatation_disc(const input_image_t & input, output_image_t & output, float radius)
  {
    static_assert(std::is_same_v<typename input_image_t::color_t, typename output_image_t::color_t>);

    using color_t = typename input_image_t::color_t;

    if (input.size() != output.size())
    {
      throw std::runtime_error("dimension mismatch");
    }

    auto in = [&](int x, int y) { return input.read_pixel(x, y); };
    auto out = [&](int x, int y, const color_t & c) { output.write_pixel(x, y, c); };
    auto pred = [](const color_t & c1, const color_t & c2) { return morpho_t<color_t>::dilatation(c1, c2); };

    morpho_disc(in, out, input.width(), input.height(), radius, pred);
  }

  // Erosion.
  template <typename input_image_t, typename output_image_t>
  void erosion_rectangle(const input_image_t & input, output_image_t & output, int kernel_width, int kernel_height)
  {
    static_assert(std::is_same_v<typename input_image_t::color_t, typename output_image_t::color_t>);

    using color_t = typename input_image_t::color_t;

    if (input.size() != output.size())
    {
      throw std::runtime_error("dimension mismatch");
    }

    auto in = [&](int x, int y) { return input.read_pixel(x, y); };
    auto out = [&](int x, int y, const color_t & c) { output.write_pixel(x, y, c); };
    auto pred = [](const color_t & c1, const color_t & c2) { return morpho_t<color_t>::erosion(c1, c2); };

    morpho_rectangle(in, out, input.width(), input.height(), kernel_width, kernel_height, pred);
  }

  template <typename input_image_t, typename output_image_t>
  void erosion_disc(const input_image_t & input, output_image_t & output, float radius)
  {
    static_assert(std::is_same_v<typename input_image_t::color_t, output_image_t::color_t>);

    using color_t = typename input_image_t::color_t;

    if (input.size() != output.size())
    {
      throw std::runtime_error("dimension mismatch");
    }

    auto in = [&](int x, int y) { return input.read_pixel(x, y); };
    auto out = [&](int x, int y, const color_t & c) { output.write_pixel(x, y, c); };
    auto pred = [](const color_t & c1, const color_t & c2) { return morpho_t<color_t>::erosion(c1, c2); };

    morpho_disc(in, out, input.width(), input.height(), radius, pred);
  }
}

// Dynamic images. For now only a subset of pixel types is supported.
namespace ggo
{
  template <pixel_type pixel_type>
  struct morphology_dispatcher
  {
    template <typename input_void_ptr_t>
    static void dilatation_rectangle(const image_base<input_void_ptr_t> & input, image & output, int kernel_width, int kernel_height)
    {
      ggo::const_image_t<pixel_type> input_view(input.data(), input.size(), input.line_byte_step());
      ggo::image_t<pixel_type> output_view(output.data(), output.size(), output.line_byte_step());

      ggo::dilatation_rectangle(input_view, output_view, kernel_width, kernel_height);
    }

    template <typename input_void_ptr_t>
    static void dilatation_disc(const image_base<input_void_ptr_t> & input, image & output, float radius)
    {
      ggo::const_image_t<pixel_type> input_view(input.data(), input.size(), input.line_byte_step());
      ggo::image_t<pixel_type> output_view(output.data(), output.size(), output.line_byte_step());

      ggo::dilatation_disc(input_view, output_view, radius);
    }

    template <typename input_void_ptr_t>
      static void erosion_rectangle(const image_base<input_void_ptr_t> & input, image & output, int kernel_width, int kernel_height)
    {
      ggo::const_image_t<pixel_type> input_view(input.data(), input.size(), input.line_byte_step());
      ggo::image_t<pixel_type> output_view(output.data(), output.size(), output.line_byte_step());

      ggo::erosion_rectangle(input_view, output_view, kernel_width, kernel_height);
    }

    template <typename input_void_ptr_t>
    static void erosion_disc(const image_base<input_void_ptr_t> & input, image & output, float radius)
    {
      ggo::const_image_t<pixel_type> input_view(input.data(), input.size(), input.line_byte_step());
      ggo::image_t<pixel_type> output_view(output.data(), output.size(), output.line_byte_step());

      ggo::erosion_disc(input_view, output_view, radius);
    }
  };

  // Dilatation.
  template <typename input_void_ptr_t>
  void dilatation_rectangle(const image_base<input_void_ptr_t> & input, image & output, int kernel_width, int kernel_height)
  {
    if (input.pixel_type() == ggo::pixel_type::rgb_8u && output.pixel_type() == ggo::pixel_type::rgb_8u)
    {
      morphology_dispatcher<ggo::pixel_type::rgb_8u>::dilatation_rectangle(input, output, kernel_width, kernel_height);
    }
    else if (input.pixel_type() == ggo::pixel_type::bgr_8u && output.pixel_type() == ggo::pixel_type::bgr_8u)
    {
      morphology_dispatcher<ggo::pixel_type::bgr_8u>::dilatation_rectangle(input, output, kernel_width, kernel_height);
    }
    else
    {
      throw std::runtime_error("pixel type mismatch, or unsupported");
    }
  }

  template <typename input_void_ptr_t>
  void dilatation_disc(const image_base<input_void_ptr_t> & input, image & output, float radius)
  {
    if (input.pixel_type() == ggo::pixel_type::rgb_8u && output.pixel_type() == ggo::pixel_type::rgb_8u)
    {
      morphology_dispatcher<ggo::pixel_type::rgb_8u>::dilatation_disc(input, output, radius);
    }
    else if (input.pixel_type() == ggo::pixel_type::bgr_8u && output.pixel_type() == ggo::pixel_type::bgr_8u)
    {
      morphology_dispatcher<ggo::pixel_type::bgr_8u>::dilatation_disc(input, output, radius);
    }
    else
    {
      throw std::runtime_error("pixel type mismatch, or unsupported");
    }
  }

  // Erosion.
  template <typename input_void_ptr_t>
  void erosion_rectangle(const image_base<input_void_ptr_t> & input, image & output, int kernel_width, int kernel_height)
  {
    if (input.pixel_type() == ggo::pixel_type::rgb_8u && output.pixel_type() == ggo::pixel_type::rgb_8u)
    {
      morphology_dispatcher<ggo::pixel_type::rgb_8u>::erosion_rectangle(input, output, kernel_width, kernel_height);
    }
    else if (input.pixel_type() == ggo::pixel_type::bgr_8u && output.pixel_type() == ggo::pixel_type::bgr_8u)
    {
      morphology_dispatcher<ggo::pixel_type::bgr_8u>::erosion_rectangle(input, output, kernel_width, kernel_height);
    }
    else
    {
      throw std::runtime_error("pixel type mismatch, or unsupported");
    }
  }

  template <typename input_void_ptr_t>
  void erosion_disc(const image_base<input_void_ptr_t> & input, image & output, float radius)
  {
    if (input.pixel_type() == ggo::pixel_type::rgb_8u && output.pixel_type() == ggo::pixel_type::rgb_8u)
    {
      morphology_dispatcher<ggo::pixel_type::rgb_8u>::erosion_disc(input, output, radius);
    }
    else if (input.pixel_type() == ggo::pixel_type::bgr_8u && output.pixel_type() == ggo::pixel_type::bgr_8u)
    {
      morphology_dispatcher<ggo::pixel_type::bgr_8u>::erosion_disc(input, output, radius);
    }
    else
    {
      throw std::runtime_error("pixel type mismatch, or unsupported");
    }
  }
}

#endif
