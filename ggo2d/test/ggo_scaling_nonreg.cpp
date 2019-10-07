#include "ggo_2d_nonreg.h"
#include <2d/processing/ggo_scaling.h>
#include <2d/io/ggo_bmp.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(scaling, y32f)
{
  auto input = make_image<ggo::pixel_type::y_32f, ggo::lines_order::down>(10, 10, {
    1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f });

  auto output_linear = ggo::scale(input, { 100, 100 }, ggo::scaling_algo::linear_integration);
  ggo::save_bmp("scaling_linear.bmp", output_linear);

  auto output_bicubic = ggo::scale(input, { 100, 100 }, ggo::scaling_algo::cubic_integration);
  ggo::save_bmp("scaling_cubic.bmp", output_bicubic);
}