#pragma once

#include <2d/ggo_image.h>
#include <2d/ggo_color.h>

// Fill solid.
namespace ggo
{
  template <typename image_t>
  void fill_solid(image_t& image, const typename image_t::color_t& c)
  {
    const int w = image.width();
    const int h = image.height();

    for (int y = 0; y < h; ++y)
    {
      for (int x = 0; x < w; ++x)
      {
        image.write_pixel(x, y, c);
      }
    }
  }

  template <pixel_type image_pixel_type, typename memory_layout_t>
  void fill_solid(image_t<image_pixel_type, memory_layout_t> & image, const typename pixel_type_traits<image_pixel_type>::color_t & c)
  {
    image.for_each_pixel([&](void * ptr, int x, int t)
    {
      pixel_type_traits<image.pixel_type()>::write(ptr, c);
    });
  }




  //template <typename image_t>
  //void fill_solid(image_t & image, const typename image_t::color_t & c, const ggo::rect_int & clipping)
  //{
  //  auto view = make_image_view(image, clipping);
  //  if (view)
  //  {
  //    fill_solid(*view, c);
  //  }
  //}
}

// Fill black.
namespace ggo
{
  template <typename image_t>
  void fill_black(image_t& image)
  {
    using color_t = typename image_t::color_t;

    constexpr auto b = black<color_t>();

    const int w = image.width();
    const int h = image.height();

    for (int y = 0; y < h; ++y)
    {
      for (int x = 0; x < w; ++x)
      {
        image.write_pixel(x, y, b);
      }
    }
  }

  template <pixel_type image_pixel_type, typename memory_layout_t>
  void fill_black(image_t<image_pixel_type, memory_layout_t> & image)
  {
    using color_t = typename pixel_type_traits<image_pixel_type>::color_t;

    if constexpr (has_alpha_v<color_t> == true)
    {
      constexpr auto b = black<color_t>();

      fill_solid(image, b);
    }
    else
    {
      std::memset(image.data(), 0, image.buffer_byte_size());
    }
  }

  //template <typename image_t>
  //void fill_black(image_t & image, const ggo::rect_int & clipping)
  //{
  //  auto view = make_image_view(image, clipping);
  //  if (view)
  //  {
  //    fill_black(*view);
  //  }
  //}

  //struct fill_black_functor
  //{
  //  template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
  //  static void call(image & img)
  //  {
  //    ggo::image_t<pixel_type, memory_lines_order> view(img.data(), img.size(), img.line_byte_step());

  //    fill_black(view);
  //  }
  //};

  //inline void fill_black(image & img)
  //{
  //  ggo::dispatch_image_format<fill_black_functor>(img.pixel_type(), img.memory_lines_order(), img);
  //}
}

