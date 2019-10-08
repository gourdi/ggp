#ifndef __GGO_BLIT__
#define __GGO_BLIT__

#include <2d/ggo_image.h>
#include <2d/ggo_color.h>
#include <2d/paint/ggo_blend.h>

// Static images.
namespace ggo
{
  // Generic function.
  template <typename input_image_type, typename output_image_type>
  void blit(const input_image_type & input_image, output_image_type & output_image)
  {
    using input_color_t = typename input_image_type::color_t;
    using output_color_t = typename output_image_type::color_t;

    if (input_image.size() != output_image.size())
    {
      throw std::runtime_error("size mismatch");
    }

    for (int y = 0; y < input_image.height(); ++y)
    {
      for (int x = 0; x < input_image.width(); ++x)
      {
        if constexpr (has_alpha_v<input_color_t> == true)
        {
          output_image.write_pixel(x, y, alpha_blend<output_color_t>(output_image.read_pixel(x, y), input_image.read_pixel(x, y)));
        }
        else
        {
          output_image.write_pixel(x, y, convert_color_to<output_color_t>(input_image.read_pixel(x, y)));
        }
      }
    }
  }

  // Optimized version for image_base_t that have the same pixel type and memory lines order.
  template <
    pixel_type pixel_type, lines_order memory_lines_order, typename input_void_ptr_t, bool input_owns_buffer, bool output_owns_buffer
  >
    void blit(
      const image_base_t<pixel_type, memory_lines_order, input_void_ptr_t, input_owns_buffer> & input_image,
      image_base_t<pixel_type, memory_lines_order, void *, output_owns_buffer> & output_image)
  {
    using color_t = typename pixel_type_traits<pixel_type>::color_t;

    if (input_image.size() != output_image.size())
    {
      throw std::runtime_error("size mismatch");
    }

    if constexpr (has_alpha_v<color_t> == true)
    {
      for (int y = 0; y < input_image.height(); ++y)
      {
        const void * input_ptr = input_image.line_ptr(y);
        void * output_ptr = output_image.line_ptr(y);

        for (int x = 0; x < input_image.width(); ++x)
        {
          auto c1 = pixel_type_traits<pixel_type>::read(input_ptr);
          auto c2 = pixel_type_traits<pixel_type>::read(output_ptr);
          auto c3 = alpha_blend<color_t>(c2, c1);
          pixel_type_traits<pixel_type>::write(output_ptr, c3);

          input_ptr = ggo::move_ptr(input_ptr, input_image.pixel_byte_size());
          output_ptr = ggo::move_ptr(output_ptr, output_image.pixel_byte_size());
        }
      }
    }
    else
    {
      const int line_byte_size = input_image.width() * input_image.pixel_byte_size();

      for (int y = 0; y < input_image.height(); ++y)
      {
        std::memcpy(output_image.line_ptr(y), input_image.line_ptr(y), line_byte_size);
      }
    }
  }

  template <pixel_type in_pixel_type, lines_order in_memory_lines_order, typename in_void_ptr_t, bool in_owns_buffer,
    pixel_type out_pixel_type, lines_order out_memory_lines_order, bool out_owns_buffer>
  void blit(const image_base_t<in_pixel_type, in_memory_lines_order, in_void_ptr_t, in_owns_buffer> & input_image,
    image_base_t<out_pixel_type, out_memory_lines_order, void *, out_owns_buffer> & output_image,
    int left, int bottom)
  {
    // Create image views.
    auto input_view = input_image.create_view(rect_int::from_left_width_bottom_height(-left, output_image.width(), -bottom, output_image.height()));
    if (!input_view)
    {
      return;
    }

    auto output_view = output_image.create_view(rect_int::from_left_width_bottom_height(left, input_image.width(), bottom, input_image.height()));
    if (!output_view)
    {
      return;
    }

    GGO_ASSERT_EQ(input_view->size(), output_view->size());

    blit(*input_view,* output_view);
  }
}

// Dynamic images.
namespace ggo
{
  struct dispatch_output
  {
    template <ggo::pixel_type output_pixel_type, ggo::lines_order output_memory_lines_order, typename input_image_t, typename output_image_t>
    static void call(const input_image_t & input_image, output_image_t & output_image, int left, int bottom)
    {
      ggo::image_view_t<output_pixel_type, output_memory_lines_order> output_view(output_image.data(), output_image.size());

      blit(input_image, output_view, left, bottom);
    }
  };

  struct dispatch_input
  {
    template <ggo::pixel_type input_pixel_type, ggo::lines_order input_memory_lines_order, typename input_image_t, bool out_owns_buffer>
    static void call(const input_image_t & input_image, image_base<void *, out_owns_buffer> & output_image, int left, int bottom)
    {
      ggo::const_image_view_t<input_pixel_type, input_memory_lines_order> input_view(input_image.data(), input_image.size());

      dispatch_image_format<dispatch_output>(output_image.pixel_type(), output_image.memory_lines_order(), input_view, output_image, left, bottom);
    }
  };

  template <typename in_void_ptr_t, bool in_owns_buffer, bool out_owns_buffer>
  void blit(
    const image_base<in_void_ptr_t, in_owns_buffer> & input_image,
    image_base<void *, out_owns_buffer> & output_image,
    int left, int bottom)
  {
    dispatch_image_format<dispatch_input>(input_image.pixel_type(), input_image.memory_lines_order(), input_image, output_image, left, bottom);
  }
}

#endif
