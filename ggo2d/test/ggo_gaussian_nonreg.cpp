#define GGO_GAUSSIAN_DEBUG 1

#include <ggo_nonreg.h>
#include <ggo_gaussian_blur.h>
#include <ggo_rgb_image_buffer.h>
#include <ggo_paint.h>
#include <ggo_bmp.h>

namespace
{
  ////////////////////////////////////////////////////////////////////
  GGO_TEST(gaussian, rgb)
  {
    const int WIDTH = 800;
    const int HEIGHT = 600;

    ggo::rgb_image_buffer_uint8 image(WIDTH, HEIGHT, ggo::color::BLUE);

    ggo::paint(image, std::make_shared<ggo::disc_float>(0.5f * WIDTH, 0.5f * HEIGHT, 0.25f * std::min(WIDTH, HEIGHT)), ggo::color::WHITE);

    ggo::gaussian_blur_2d_mirror<3, 3>(image.data() + 0, image.data() + 0, WIDTH, HEIGHT, 5, 0.001f);
    ggo::gaussian_blur_2d_mirror<3, 3>(image.data() + 1, image.data() + 1, WIDTH, HEIGHT, 5, 0.001f);
    ggo::gaussian_blur_2d_mirror<3, 3>(image.data() + 2, image.data() + 2, WIDTH, HEIGHT, 5, 0.001f);

    ggo::save_bmp("gaussian.bmp", image.data(), WIDTH, HEIGHT);
  }
}
