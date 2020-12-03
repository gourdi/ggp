#include "ggo_2d_nonreg.h"
#include <2d/processing/ggo_mask.h>
#include <2d/brush/ggo_solid_color_brush.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(mask, color)
{
  auto image = make_image_rows_down_t<ggo::pixel_type::y_8u>({ 3, 2 }, 0xff);
  auto mask  = make_image_rows_down_t<ggo::pixel_type::y_8u>({ 3, 2 }, {
    0x00, 0x10, 0x20,
    0x40, 0xff, 0x80 });

  ggo::apply_mask(image, mask, ggo::make_solid_brush(0x00_u8));

  GGO_CHECK_PIXELS(image, {
    0xff, 0xef, 0xdf,
    0xbf, 0x00, 0x7f })
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(mask, layer)
{
  auto image = make_image_rows_down_t<ggo::pixel_type::y_8u>({ 3, 2 }, 0xff);
  auto mask  = make_image_rows_down_t<ggo::pixel_type::y_8u>({ 3, 2 }, {
    0x00, 0x10, 0x20,
    0x40, 0xff, 0x80 });
  auto layer = make_image_rows_down_t<ggo::pixel_type::y_8u>({ 3, 2 }, 0x00);

  ggo::apply_mask(image, mask, layer);

  GGO_CHECK_PIXELS(image, {
    0xff, 0xef, 0xdf,
    0xbf, 0x00, 0x7f });
}

