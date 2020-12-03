#pragma once

#include <kernel/math/signal_processing/ggo_blit.h>
#include <kernel/scan/ggo_scan.h>
#include <2d/ggo_color.h>
#include <2d/blend/ggo_alpha_blend.h>

// Static images, with no dispatch.
namespace ggo
{
  template <typename dst_image_t, typename src_image_t>
  void blit(dst_image_t & dst_image, const src_image_t & src_image, ggo::pos2_i offset)
  {
    using dst_color_t = typename dst_image_t::color_t;
    using src_color_t = typename src_image_t::color_t;

    auto scan = pixels_scan_for(dst_image);

    if constexpr (has_alpha_v<src_color_t> == true)
    {
      auto dst = [&](int x, int y, src_color_t c)
      {
        auto c2 = dst_image.read_pixel(x, y);
        auto c3 = alpha_blend(c2, c);
        dst_image.write_pixel(x, y, c3);
      };
      auto src = [&](int x, int y) { return src_image.read_pixel(x, y); };

      blit(dst, dst_image.size(), src, src_image.size(), offset, scan);
    }
    else
    {
      auto dst = [&](int x, int y, src_color_t c)
      {
        auto c2 = ggo::convert_color_to<dst_color_t>(c);
        dst_image.write_pixel(x, y, c2);
      };
      auto src = [&](int x, int y) { return src_image.read_pixel(x, y); };

      blit(dst, dst_image.size(), src, src_image.size(), offset, scan);
    }
  }
}

#if 0
// Dynamic images.
namespace ggo
{
  template <ggo::pixel_type src_pixel_type, typename dst_image_t, typename src_image_t, typename scan_t>
  struct dispatch_src_dst
  {
    template <ggo::pixel_type dst_pixel_type>
    static void call(dst_image_t & dst_image, const src_image_t & src_image, ggo::pos2_i offset, const scan_t & scan)
    {
      using dst_color_t = typename pixel_type_traits<dst_pixel_type>::color_t;
      using src_color_t = typename pixel_type_traits<src_pixel_type>::color_t;

      if constexpr (has_alpha_v<src_color_t> == true)
      {
        auto dst = [&](int x, int y, src_color_t c)
        {
          auto c2 = read_pixel<dst_pixel_type>(dst_image, x, y);
          auto c3 = alpha_blend(c2, c);
          write_pixel<dst_pixel_type>(dst_image, x, y, c3);
        };
        auto src = [&](int x, int y) { return read_pixel<src_pixel_type>(src_image, x, y); };

        blit(dst, dst_image.size(), src, src_image.size(), offset, scan_rows_up());
      }
      else
      {
        auto dst = [&](int x, int y, src_color_t c)
        {
          auto c2 = ggo::convert_color_to<dst_color_t>(c);
          write_pixel<dst_pixel_type>(dst_image, x, y, c2);
        };
        auto src = [&](int x, int y){ return read_pixel<src_pixel_type>(src_image, x, y); };

        blit(dst, dst_image.size(), src, src_image.size(), offset, scan_rows_up());
      }
    }
  };

  template <typename dst_image_t, typename src_image_t, typename scan_t>
  struct dispatch_src
  {
    template <ggo::pixel_type src_pixel_type>
    static void call(dst_image_t & dst_image, const src_image_t & src_image, ggo::pos2_i offset, const scan_t & scan)
    {
      dispatch_pixel_type<dispatch_src_dst<src_pixel_type, dst_image_t, src_image_t, scan_t>>(dst_image.pixel_type(), dst_image, src_image, offset, scan);
    }
  };

  template <typename dst_image_t, typename src_image_t, typename scan_t>
  void blit(dst_image_t & dst_image, const src_image_t & src_image, ggo::pos2_i offset, const scan_t & scan)
  {
    dispatch_pixel_type<dispatch_src<dst_image_t, src_image_t, scan_t>>(src_image.pixel_type(), dst_image, src_image, offset, scan);
  }

  template <typename dst_image_t, typename src_image_t>
  void blit(dst_image_t & dst_image, const src_image_t & src_image, ggo::pos2_i offset)
  {
    blit(dst_image, src_image, offset, scan_rows_up());
  }
}
#endif
