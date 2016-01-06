#include <ggo_nonreg.h>
#include <ggo_paint.h>
#include <ggo_layer.h>
#include <ggo_bmp.h>

////////////////////////////////////////////////////////////////////
GGO_TEST(layer, test)
{
  const int IMAGE_WIDTH = 200;
  const int IMAGE_HEIGHT = 150;
  ggo::rgb_image_data_uint8 image(IMAGE_WIDTH, IMAGE_HEIGHT);
  
  image.fill(ggo::color(0.2f, 0.1f, 0.1f));
  
  const int SPRITE_WIDTH = 200;
  const int SPRITE_HEIGHT = 160;
  ggo::gray_image_data_uint8 sprite_opacity(SPRITE_WIDTH, SPRITE_HEIGHT);
  ggo::rgb_image_data_float sprite_image(SPRITE_WIDTH, SPRITE_HEIGHT);
  
  sprite_opacity.fill(0.1f);
  sprite_image.fill(ggo::color::BLUE);
  
  ggo::paint(sprite_opacity, std::make_shared<ggo::disc_float>(0.5f * SPRITE_WIDTH, 0.5f * SPRITE_HEIGHT, 50.f), 1.f);
  
  for (int y = 0; y <= SPRITE_HEIGHT; y += 20)
  {
    for (int x = 0; x <= SPRITE_WIDTH; x += 20)
    {
      ggo::paint(sprite_image, std::make_shared<ggo::disc_float>(static_cast<float>(x), static_cast<float>(y), 8.f), ggo::color::GREEN);
    }
  }

  ggo::flatten_sprite(image, sprite_opacity, sprite_image, 10, 15, ggo::rgb_alpha_blender());
  
  ggo::save_bmp("test_sprite_image.bmp", image.get_buffer(), IMAGE_WIDTH, IMAGE_HEIGHT);
}