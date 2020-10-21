#include "ggo_2d_nonreg.h"
#include <2d/processing/ggo_scaling.h>
#include <2d/io/ggo_bmp.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(scaling, y32f)
{
  auto input = make_image<ggo::pixel_type::y_32f>({
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

  auto rescaled = make_image<ggo::pixel_type::y_32f>({ 100, 100 }, 0.f);

  ggo::scale_linear_interpolation(input, rescaled);
  ggo::save_bmp("scale_linear_interpolation.bmp", rescaled);

  ggo::scale_cubic_interpolation(input, rescaled);
  ggo::save_bmp("scale_cubic_interpolation.bmp", rescaled);

  ggo::scale_linear_integration(input, rescaled);
  ggo::save_bmp("scale_linear_integration.bmp", rescaled);
}