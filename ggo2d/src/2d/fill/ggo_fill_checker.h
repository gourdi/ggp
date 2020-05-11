#ifndef __GGO_FILL_CHECKER__
#define __GGO_FILL_CHECKER__

#include <2d/ggo_image.h>

namespace ggo
{
  template <typename image_t>
  void fill_checker(image_t & image, 
    const typename image_t::color_t & c1,
    const typename image_t::color_t & c2,
    int tile_size)
  {
    const int w = image.width();
    const int h = image.height();

    for (int y = 0; y < h; ++y)
    {
      for (int x = 0; x < w; ++x)
      {
        int index_x = x / tile_size;
        int index_y = y / tile_size;

        image.write_pixel(x, y, ((index_x + index_y) % 2) ? c1 : c2);
      }
    }
  }

  template <ggo::pixel_type pixel_type>
  void fill_checker(void * buffer, int width, int height, int line_byte_step,
    const typename pixel_type_traits<pixel_type>::color_t & c1,
    const typename pixel_type_traits<pixel_type>::color_t & c2,
    int tile_size)
  {
    ggo::image_t<pixel_type, ggo::lines_order::up> image(buffer, { width, height }, line_byte_step);

    fill_checker(image, c1, c2, tile_size);
  }
}

#endif
