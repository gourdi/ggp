#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/memory/ggo_array.h>
#include <2d/paint/ggo_glow.h>
#include <2d/io/ggo_bmp.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(glow, rgb8)
{
  int width = 180;
  int height = 120;
  int line_byte_step = 3 * width;
  ggo::array_8u buffer(3 * width * height, 0);

  ggo::paint_glow<ggo::rgb_8u_yd>(buffer.data(), width, height, line_byte_step, ggo::disc_float({ 30.f, 30.f }, 15.f), 10.f, 0.2f, ggo::red_8u());
  ggo::paint_glow<ggo::rgb_8u_yd>(buffer.data(), width, height, line_byte_step, ggo::disc_float({ 90.f, 30.f }, 15.f), 10.f, 0.6f, ggo::red_8u());
  ggo::paint_glow<ggo::rgb_8u_yd>(buffer.data(), width, height, line_byte_step, ggo::disc_float({150.f, 30.f }, 15.f), 10.f, 1.0f, ggo::red_8u());

  ggo::paint_glow<ggo::rgb_8u_yd>(buffer.data(), width, height, line_byte_step, ggo::disc_float({ 30.f, 90.f }, 15.f), 10.f, 1.0f, ggo::red_8u());
  ggo::paint_glow<ggo::rgb_8u_yd>(buffer.data(), width, height, line_byte_step, ggo::disc_float({ 90.f, 90.f }, 15.f), 20.f, 1.0f, ggo::red_8u());
  ggo::paint_glow<ggo::rgb_8u_yd>(buffer.data(), width, height, line_byte_step, ggo::disc_float({150.f, 90.f }, 15.f), 30.f, 1.0f, ggo::red_8u());

  ggo::save_bmp("glow.bmp", buffer.data(), ggo::rgb_8u_yd, width, height, line_byte_step);
}


