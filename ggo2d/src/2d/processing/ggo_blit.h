#pragma once

#include <kernel/math/signal_processing/ggo_blit.h>
#include <2d/ggo_color.h>
#include <2d/blend/ggo_alpha_blend.h>

#if 0
namespace ggo
{
  // Generic function.
  template <typename in_t, typename out_t>
  void blit(dst_t && dst, ggo::size dst_size, src_t && src, ggo::size src_size, ggo::pos2i offset)
  {
    for (int y_dst, int y_src; ; ++y_dst, ++y_src)
    {
      for (int x_dst, int x_src; ; ++x_dst, ++x_src)
      {
        dst(x_dst, y_dst, src(x_src, y_src));
      }
    }
  }

  // Optimized version for image_base_t that have the same pixel type and memory lines order.
  template <pixel_type pixel_type, lines_order memory_lines_order, typename input_void_ptr_t>
  void blit(
    const image_base_t<pixel_type, memory_lines_order, input_void_ptr_t> & input_image,
    image_base_t<pixel_type, memory_lines_order, void *> & output_image)
  {
    using color_t = typename pixel_type_traits<pixel_type>::color_t;

    if (input_image.size() != output_image.size())
    {
      throw std::runtime_error("size mismatch");
    }

    const int h = input_image.height();

    if constexpr (has_alpha_v<color_t> == true)
    {
      const int w = input_image.width();

      for (int y = 0; y < h; ++y)
      {
        const void * input_ptr = input_image.line_ptr(y);
        void * output_ptr = output_image.line_ptr(y);

        for (int x = 0; x < w; ++x)
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

      const void * src_ptr = input_image.data();
      void * dst_ptr = output_image.data();

      for (int y = 0; y < h; ++y)
      {
        std::memcpy(dst_ptr, src_ptr, line_byte_size);

        src_ptr = move_ptr(src_ptr, input_image.line_byte_step());
        dst_ptr = move_ptr(dst_ptr, output_image.line_byte_step());
      }
    }
  }

  template <pixel_type in_pixel_type, lines_order in_memory_lines_order, typename in_void_ptr_t,
    pixel_type out_pixel_type, lines_order out_memory_lines_order>
  void blit(const image_base_t<in_pixel_type, in_memory_lines_order, in_void_ptr_t> & input_image,
    image_base_t<out_pixel_type, out_memory_lines_order, void *> & output_image,
    int left, int bottom)
  {
    // Create image views.
    auto input_view = make_image_view(input_image, rect_int::from_left_width_bottom_height(-left, output_image.width(), -bottom, output_image.height()));
    if (!input_view)
    {
      return;
    }

    auto output_view = make_image_view(output_image, rect_int::from_left_width_bottom_height(left, input_image.width(), bottom, input_image.height()));
    if (!output_view)
    {
      return;
    }
    
    GGO_ASSERT_EQ(input_view->size(), output_view->size());

    blit(*input_view,* output_view);
  }
}
#endif

// Static images, with no dispatch.
namespace ggo
{
  template <ggo::pixel_type src_pixel_type, typename dst_image_t, typename src_image_t>
  struct dispatch_src_dst
  {
    template <ggo::pixel_type dst_pixel_type>
    static void call(dst_image_t & dst_image, const src_image_t & src_image, ggo::pos2_i offset)
    {
      using dst_color_t = typename pixel_type_traits<dst_pixel_type>::color_t;
      using src_color_t = typename pixel_type_traits<src_pixel_type>::color_t;

      if constexpr (has_alpha_v<src_color_t> == true)
      {

      }
      else
      {
        auto dst = [&](int x, int y, src_color_t c)
        {
          auto c2 = ggo::convert_color_to<dst_color_t>(c);
          write_pixel<dst_pixel_type>(dst_image, x, y, c2);
        };
        auto src = [&](int x, int y){ return read_pixel<src_pixel_type>(src_image, x, y); };

        blit(dst, dst_image.size(), src, src_image.size(), offset);
      }
    }
  };

  template <typename dst_image_t, typename src_image_t>
  struct dispatch_src
  {
    template <ggo::pixel_type src_pixel_type>
    static void call(dst_image_t & dst_image, const src_image_t & src_image, ggo::pos2_i offset)
    {
      dispatch_pixel_type<dispatch_src_dst<src_pixel_type, dst_image_t, src_image_t>>(dst_image.pixel_type(), dst_image, src_image, offset);
    }
  };




  template <typename dst_image_t, typename src_image_t>
  void blit(dst_image_t & dst_image, const src_image_t & src_image, ggo::pos2_i offset)
  {
    if constexpr (requires { src_image.read_pixel(int, int); })
    {
      using dst_color_t = typename dst_image_t::color_t;
      using src_color_t = typename src_image_t::color_t;

      if constexpr (has_alpha_v<src_color_t> == true)
      {
        auto dst = [&](int x, int y, src_color_t c)
        {
          auto c2 = dst_image.read_pixel(x, y);
          auto c3 = alpha_blend(c2, c);
          dst_image.write_pixel(x, y, c3);
        };
        auto src = [&](int x, int y) { return src_image.read_pixel(x, y); };

        blit(dst, dst_image.size(), src, src_image.size(), offset);
      }
      else
      {
        auto dst = [&](int x, int y, src_color_t c)
        {
          auto c2 = ggo::convert_color_to<dst_color_t>(c);
          dst_image.write_pixel(x, y, c2);
        };
        auto src = [&](int x, int y) { return src_image.read_pixel(x, y); };

        blit(dst, dst_image.size(), src, src_image.size(), offset);
      }
    }
    else
    {
      dispatch_pixel_type<dispatch_src<dst_image_t, src_image_t>>(src_image.pixel_type(), dst_image, src_image, offset);
    }
  }
}
//
//// Dynamic images.
//namespace ggo
//{
//
//
//  template <typename dst_image_t, typename src_image_t>
//  void blit(dst_image_t & dst_image, const src_image_t & src_image, ggo::pos2_i offset)
//  {
//    dispatch_pixel_type<dispatch_src<dst_image_t, src_image_t>>(src_image.pixel_type(), dst_image, src_image, offset);
//  }
//}
//
