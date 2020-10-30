#include <kernel/nonreg/ggo_nonreg.h>
#include <2d/ggo_image.h>
#include <2d/paint/ggo_glow.h>
#include <2d/io/ggo_bmp.h>
#if 0
/////////////////////////////////////////////////////////////////////
GGO_TEST(glow, rgb8)
{
  ggo::image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::down> image({ 180, 120 });

  ggo::paint_glow(image, ggo::disc_f({ 30.f, 30.f }, 15.f), 10.f, 0.2f, ggo::red_8u());
  ggo::paint_glow(image, ggo::disc_f({ 90.f, 30.f }, 15.f), 10.f, 0.6f, ggo::red_8u());
  ggo::paint_glow(image, ggo::disc_f({150.f, 30.f }, 15.f), 10.f, 1.0f, ggo::red_8u());

  ggo::paint_glow(image, ggo::disc_f({ 30.f, 90.f }, 15.f), 10.f, 1.0f, ggo::red_8u());
  ggo::paint_glow(image, ggo::disc_f({ 90.f, 90.f }, 15.f), 20.f, 1.0f, ggo::red_8u());
  ggo::paint_glow(image, ggo::disc_f({150.f, 90.f }, 15.f), 30.f, 1.0f, ggo::red_8u());

  ggo::save_bmp("glow.bmp", image);
}
#endif

