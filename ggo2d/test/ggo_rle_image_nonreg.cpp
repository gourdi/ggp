#if 0 


#include <ggo_nonreg.h>
#include <ggo_paint.h>
#include <ggo_rle_image.h>
#include <ggo_rle_rgb8_image.h>
#include <ggo_rgb_image_buffer.h>
#include <ggo_bmp.h>

////////////////////////////////////////////////////////////////////
GGO_TEST(rle_image, rle_color)
{
  const int width = 100;
  const int height = 60;

  ggo::rle_image<ggo::color> rle_image(width, height);

  ggo::paint(rle_image, std::make_shared<ggo::disc_float>(static_cast<float>(width), static_cast<float>(height), static_cast<float>(height / 2)), ggo::color::GREEN);

  ggo::rgb_image_buffer_uint8 image(width, height);
  image.from(rle_image);

  ggo::save_bmp("rle_color.bmp", image.data(), width, height);
}


////////////////////////////////////////////////////////////////////
GGO_TEST(rle_image, rle_rgb8)
{
  const int width = 100;
  const int height = 60;

  ggo::rle_rgb8_image rle_image(width, height);

  ggo::paint(rle_image, std::make_shared<ggo::disc_float>(static_cast<float>(width), static_cast<float>(height), static_cast<float>(height / 2)), ggo::color::GREEN);

  ggo::rgb_image_buffer_uint8 image(width, height);
  image.from(rle_image);

  ggo::save_bmp("rle_rgb8.bmp", image.data(), width, height);
}

#endif
