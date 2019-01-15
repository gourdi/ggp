#include "ggo_sonson_bitmap_artist.h"
#include <kernel/memory/ggo_array.h>
#include <2d/ggo_blit.h>

//////////////////////////////////////////////////////////////
ggo::sonson_bitmap_artist::sonson_bitmap_artist(int width, int height, int line_step, ggo::image_format format)
:
bitmap_artist_abc(width, height, line_step, format)
{

}

//////////////////////////////////////////////////////////////
void ggo::sonson_bitmap_artist::render_bitmap(void * buffer) const
{
  ggo::array2d_8u b(width(), height());

  for (int y = 0; y < b.height(); ++y)
  {
    for (int x = 0; x < b.width(); ++x)
    {
      b(x, y) = ggo::rand<uint8_t>();
    }
  }

  int size = min_size() / 100;
  ggo::array_8u neighbors((2 * size + 1) * (2 * size + 1));

  std::vector<ggo::vec2_i> pixels;
  for (int y = 0; y < height(); ++y)
  {
    for (int x = 0; x < width(); ++x)
    {
      pixels.push_back({ x, y });
    }
  }

  while (pixels.empty() == false)
  {
    size_t i = ggo::rand(size_t(0), pixels.size() - 1);
    ggo::vec2_i p = pixels[i];

    int j = 0;
    for (int dy = -size; dy <= size; ++dy)
    {
      int y_n = ggo::loop_index(p.y() + dy, height());

      for (int dx = -size; dx <= size; ++dx)
      {
        int x_n = ggo::loop_index(p.x() + dx, width());
        
        neighbors[j++] = b(x_n, y_n);
      }
    }

    std::sort(neighbors.begin(), neighbors.end());

    uint8_t res = 0;
    int count_max = 0;
    for (int i = 0; i < neighbors.size(); ++i)
    {
      int count = 0;
      for (int j = i + 1; j < neighbors.size(); ++j)
      {
        if (neighbors[i] != neighbors[j])
          break;
        ++count;
      }
      if (count > count_max)
      {
        count_max = count;
        res = neighbors[i];
      }
    }

    if (count_max > 0)
    {
      b(p.x(), p.y()) = res;
      pixels.erase(pixels.begin() + i);
    }
  }

  ggo::blit<ggo::y_8u_yd, ggo::rgb_8u_yd>(b.data(), b.width(), b.height(), b.width(),
    buffer, width(), height(), line_step(), 0, 0);
}

