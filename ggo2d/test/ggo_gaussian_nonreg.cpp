#define GGO_GAUSSIAN_DEBUG 1

#include <array>
#include <ggo_nonreg.h>
#include <ggo_gaussian_blur.h>
#include <ggo_rgb_image_data.h>
#include <ggo_paint.h>
#include <ggo_bmp.h>

namespace
{
  ////////////////////////////////////////////////////////////////////
  GGO_TEST(gaussian, rgb)
  {
    const int WIDTH = 800;
    const int HEIGHT = 600;

    ggo::rgb_image_data_uint8 image_data(WIDTH, HEIGHT, ggo::color::BLUE);

    ggo::paint(image_data, std::make_shared<ggo::disc_float>(0.5f * WIDTH, 0.5f * HEIGHT, 0.25f * std::min(WIDTH, HEIGHT)), ggo::color::WHITE);

    ggo::gaussian_blur_2d_uint8(image_data.get_buffer() + 0, image_data.get_buffer() + 0, WIDTH, HEIGHT, 5, 3, 3);
    ggo::gaussian_blur_2d_uint8(image_data.get_buffer() + 1, image_data.get_buffer() + 1, WIDTH, HEIGHT, 5, 3, 3);
    ggo::gaussian_blur_2d_uint8(image_data.get_buffer() + 2, image_data.get_buffer() + 2, WIDTH, HEIGHT, 5, 3, 3);

    ggo::save_bmp("gaussian.bmp", image_data.get_buffer(), WIDTH, HEIGHT);
  }
}