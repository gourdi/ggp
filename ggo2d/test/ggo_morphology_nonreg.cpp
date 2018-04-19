#include <kernel/nonreg/ggo_nonreg.h>
#include <2d/fill/ggo_fill.h>
#include <2d/processing/ggo_morphology.h>
#include <2d/io/ggo_bmp.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(morphology, dilatation_disc)
{
  const int width = 80;
  const int height = 60;
  const int line_step = 3 * width;

  ggo::array_8u input(line_step * height, 0);
  ggo::fill_solid<ggo::rgb_8u_yd>(input.data(), width, height, line_step, ggo::red_8u(), ggo::rect_int::from_left_right_bottom_top(5, 30, 15, 25));
  ggo::fill_solid<ggo::rgb_8u_yd>(input.data(), width, height, line_step, ggo::green_8u(), ggo::rect_int::from_left_right_bottom_top(35, 50, 20, 30));

  ggo::array_8u output(line_step * height);
  ggo::dilatation_disc<ggo::rgb_8u_yd>(input.data(), output.data(), width, height, line_step, 10.f);

  ggo::save_bmp("dilatation_disc.bmp", output.data(), ggo::rgb_8u_yd, width, height, line_step);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(morphology, erosion_rect)
{
  const int width = 80;
  const int height = 60;
  const int line_step = 3 * width;

  ggo::array_8u input(line_step * height, 0xff);
  input[3 * ((height / 2) * width + width / 2)] = 0; // Modify center pixel.

  ggo::array_8u output(line_step * height);
  ggo::erosion_rectangle<ggo::rgb_8u_yd>(input.data(), output.data(), width, height, line_step, 10, 5);

  ggo::save_bmp("erosion_rectangle.bmp", output.data(), ggo::rgb_8u_yd, width, height, line_step);
}