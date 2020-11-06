#pragma once

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
}
