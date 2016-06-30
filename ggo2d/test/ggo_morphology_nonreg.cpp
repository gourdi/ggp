#include <ggo_nonreg.h>
#include <ggo_morphology.h>
#include <ggo_gray_image_buffer.h>
#include <ggo_rgb_image_buffer.h>
#include <ggo_paint.h>
#include <ggo_bmp.h>

GGO_TEST(morphology, dilatation)
{
  const int width = 300;
  const int height = 200;

  ggo::gray_image_buffer_uint8 image_in(width, height, 0.f);
  ggo::gray_image_buffer_uint8 image_out(width, height, 0.f);
  
  ggo::paint(image_in, std::make_shared<ggo::rect_float>(20.f, 30.f, 50.f, 40.f), 1.0f);
  
  ggo::dilatation_disc(image_in.data(), image_out.data(),
                       width, height, 7.f);
  
  ggo::rgb_image_buffer_uint8 rgb_image(width, height);
  rgb_image.from(image_out);

  ggo::save_bmp("dilatation.bmp", rgb_image.data(), rgb_image.get_width(), rgb_image.get_height());
}

