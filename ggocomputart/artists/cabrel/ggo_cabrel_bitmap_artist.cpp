#include "ggo_cabrel_bitmap_artist.h"
#include "ggo_cabrel_curve.h"
#include <ggo_paint.h>
#include <ggo_gaussian_blur.h>

//////////////////////////////////////////////////////////////
ggo_cabrel_bitmap_artist::ggo_cabrel_bitmap_artist(int render_width, int render_height)
:
ggo_bitmap_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo_cabrel_bitmap_artist::render_bitmap(uint8_t * buffer)
{
  ggo_cabrel_curve<6> curve;

  auto image = make_image_buffer(buffer);

  for (int i = 0; i < 150; ++i)
  {
    if ((i % 10) == 0)
    {
      float stddev = 0.001f * get_render_min_size();
      ggo::gaussian_blur_2d_mirror<3, 3>(buffer + 0, buffer + 0, get_render_width(), get_render_height(), stddev, 0.0025f);
      ggo::gaussian_blur_2d_mirror<3, 3>(buffer + 1, buffer + 1, get_render_width(), get_render_height(), stddev, 0.0025f);
      ggo::gaussian_blur_2d_mirror<3, 3>(buffer + 2, buffer + 2, get_render_width(), get_render_height(), stddev, 0.0025f);
    }

    curve.update();
    curve.paint(image);
  }
}
