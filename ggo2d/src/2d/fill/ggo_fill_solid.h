#ifndef __GGO_FILL_SOLID__
#define __GGO_FILL_SOLID__

#include <2d/ggo_image.h>
#include <2d/ggo_color.h>

// Fill solid.
namespace ggo
{
  template <typename image_t>
  void fill_solid(image_t & image, const typename image_t::color_t & c)
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

  template <typename image_t>
  void fill_solid(image_t & image, const typename image_t::color_t & c, const ggo::rect_int & clipping)
  {
    auto view = make_image_view(image, clipping);
    if (view)
    {
      fill_solid(*view, c);
    }
  }
}

// Fill black.
namespace ggo
{
  template <pixel_type image_pixel_type, lines_order image_memory_lines_order>
  void fill_black(image_t<image_pixel_type, image_memory_lines_order> & image)
  {
    using color_t = typename pixel_type_traits<image_pixel_type>::color_t;

    if constexpr (has_alpha_v<color_t> == true)
    {
      constexpr auto b = black<color_t>();

      fill_solid(image, b);
    }
    else
    {
      const int l = image.width() * image.pixel_byte_size();
      const int h = image.height();

      void * ptr = image.data();

      for (int y = 0; y < h; ++y)
      {
        std::memset(ptr, 0, l);

        ptr = move_ptr(ptr, image.line_byte_step());
      }
    }
  }

  template <typename image_t>
  void fill_black(image_t & image)
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

  template <typename image_t>
  void fill_black(image_t & image, const ggo::rect_int & clipping)
  {
    auto view = make_image_view(image, clipping);
    if (view)
    {
      fill_black(*view);
    }
  }

  struct fill_black_functor
  {
    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
    static void call(image & img)
    {
      ggo::image_t<pixel_type, memory_lines_order> view(img.data(), img.size(), img.line_byte_step());

      fill_black(view);
    }
  };

  inline void fill_black(image & img)
  {
    ggo::dispatch_image_format<fill_black_functor>(img.pixel_type(), img.memory_lines_order(), img);
  }
}

#endif
