#include "ggo_2d_nonreg.h"
#include <2d/processing/ggo_scaling.h>
#include <2d/io/ggo_bmp.h>
#include <2d/processing/ggo_blit.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(scaling, y32f)
{
  auto input = make_image_t<ggo::pixel_type::y_32f>({ 10, 10 }, {
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

  auto rescaled = make_image_t<ggo::pixel_type::y_32f>({ 100, 100 }, 0.f);

  ggo::image_rgb_8u image_rgb8u({ 100, 100 });

  ggo::scale_linear_interpolation_t(input, rescaled);
  ggo::blit_t(image_rgb8u, rescaled, { 0, 0 });
  ggo::save_bmp("scale_linear_interpolation.bmp", image_rgb8u);

  ggo::scale_cubic_interpolation_t(input, rescaled);
  ggo::blit_t(image_rgb8u, rescaled, { 0, 0 });
  ggo::save_bmp("scale_cubic_interpolation.bmp", image_rgb8u);

  ggo::scale_linear_integration_t(input, rescaled);
  ggo::blit_t(image_rgb8u, rescaled, { 0, 0 });
  ggo::save_bmp("scale_linear_integration.bmp", image_rgb8u);
}

