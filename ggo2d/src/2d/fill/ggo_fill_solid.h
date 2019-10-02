#ifndef __GGO_FILL_SOLID__
#define __GGO_FILL_SOLID__

#include <2d/ggo_image.h>
#include <2d/ggo_color.h>

namespace ggo
{
  template <typename image_t>
  void fill_solid(image_t & image, const typename image_t::color_t & c, const ggo::rect_int & clipping)
  {
    const int w = image.width();
    const int h = image.height();

    for (int y = clipping.bottom(); y <= clipping.top(); ++y)
    {
      for (int x = clipping.left(); x <= clipping.right(); ++x)
      {
        image.write_pixel(x, y, c);
      }
    }
  }

  template <typename image_t>
  void fill_solid(image_t & image, const typename image_t::color_t & c)
  {
    fill_solid(image, c, ggo::rect_int::from_size(image.size()));
  }

  template <ggo::pixel_type pixel_type>
  void fill_solid(void * buffer, int width, int height, int line_byte_step,
    const typename pixel_type_traits<pixel_type>::color_t & c,
    const ggo::rect_int & clipping)
  {
    ggo::image_view_t<pixel_type, ggo::lines_order::up> image(buffer, { width, height }, line_byte_step);

    fill_solid(image, c, clipping);
  }

  template <ggo::pixel_type pixel_type>
  void fill_solid(void * buffer, int width, int height, int line_byte_step,
    const typename pixel_type_traits<pixel_type>::color_t & c)
  {
    fill_solid<pixel_type>(buffer, width, height, line_byte_step, c, ggo::rect_int::from_width_height(width, height));
  }
}

namespace ggo
{
  template <typename image_t>
  void fill_black(image_t & image, const ggo::rect_int & clipping)
  {
    const int w = image.width();
    const int h = image.height();

    for (int y = clipping.bottom(); y <= clipping.top(); ++y)
    {
      for (int x = clipping.left(); x <= clipping.right(); ++x)
      {
        image.write_pixel(x, y, black<typename image_t::color_t>());
      }
    }
  }

  template <typename image_t>
  void fill_black(image_t & image)
  {
    fill_black(image, ggo::rect_int::from_size(image.size()));
  }
}

#endif
