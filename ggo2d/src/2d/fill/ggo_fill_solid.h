#ifndef __GGO_FILL_SOLID__
#define __GGO_FILL_SOLID__

#include <2d/ggo_image.h>
#include <2d/ggo_color.h>

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
    auto view = image.create_view(clipping);
    if (view)
    {
      fill_solid(*view, c);
    }
  }
}

namespace ggo
{
  template <pixel_type image_pixel_type, lines_order image_memory_lines_orde, bool image_owns_buffer>
  void fill_black(image_base_t<image_pixel_type, image_memory_lines_orde, void *, image_owns_buffer> & image)
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
    auto view = image.create_view(clipping);
    if (view)
    {
      fill_black(*view);
    }
  }
}

#endif
