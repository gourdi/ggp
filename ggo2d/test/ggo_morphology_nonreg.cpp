#include <kernel/nonreg/ggo_nonreg.h>
#include <2d/fill/ggo_fill.h>
#include <2d/processing/ggo_morphology.h>
#include <2d/io/ggo_bmp.h>
#if 0

/////////////////////////////////////////////////////////////////////
GGO_TEST(morphology, dilatation_disc)
{
  const int width = 80;
  const int height = 60;

  ggo::image_t<ggo::pixel_type::rgb_8u> input({ width, height });
  ggo::fill_black(input);
  ggo::fill_solid(input, ggo::red_8u(), ggo::rect_int::from_left_right_bottom_top(5, 30, 15, 25));
  ggo::fill_solid(input, ggo::green_8u(), ggo::rect_int::from_left_right_bottom_top(35, 50, 20, 30));

  ggo::image_t<ggo::pixel_type::rgb_8u> output({ width, height });
  ggo::dilatation_disc(input, output, 10.f);

  ggo::save_bmp("dilatation_disc.bmp", output);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(morphology, erosion_rect)
{
  const int width = 80;
  const int height = 60;

  ggo::image_t<ggo::pixel_type::rgb_8u> input({ width, height });
  ggo::fill_solid(input, ggo::white_8u());
  input.write_pixel(width / 2, height / 2, ggo::red_8u()); // Modify center pixel.

  ggo::image_t<ggo::pixel_type::rgb_8u> output({ width, height });
  ggo::erosion_rectangle(input, output, 10, 5);

  ggo::save_bmp("erosion_rectangle.bmp", output);
}
#endif
