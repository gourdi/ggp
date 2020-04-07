#include "ggo_2d_nonreg.h"
#include <2d/processing/ggo_scaling.h>
#include <2d/io/ggo_bmp.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(scaling, y32f)
{
  auto input = make_image<ggo::pixel_type::y_32f, ggo::lines_order::down>({
    { 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f },
    { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f },
    { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f },
    { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f },
    { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f },
    { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f },
    { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f },
    { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f },
    { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f },
    { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f } });

  auto output_linear_interpolation = ggo::scale_linear_interpolation(input, { 100, 100 });
  ggo::save_bmp("scale_linear_interpolation.bmp", output_linear_interpolation);

  auto output_bicubic_interpolation = ggo::scale_cubic_interpolation(input, { 100, 100 });
  ggo::save_bmp("scale_cubic_interpolation.bmp", output_bicubic_interpolation);

  auto output_linear_integration = ggo::scale_linear_integration(input, { 100, 100 });
  ggo::save_bmp("scale_linear_integration.bmp", output_linear_integration);
}