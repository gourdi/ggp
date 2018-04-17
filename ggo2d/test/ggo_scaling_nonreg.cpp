#include <ggo_nonreg.h>
#include <ggo_buffer2d.h>
#include <ggo_scale2d.h>
#include <ggo_bmp.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(scaling, y32f)
{
  ggo::buffer2d_32f input{
    { 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, },
    { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, },
    { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, },
    { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, },
    { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, },
    { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, },
    { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, },
    { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, },
    { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, },
    { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, } };

  ggo::buffer2d_32f output(100, 100);

  ggo::scale_2d<ggo::scaling_algo::linear_integration, ggo::scaling_algo::linear_integration>(
    input.data(), input.width(), input.height(), input.line_byte_step(),
    output.data(), output.width(), output.height(), output.line_byte_step());

  ggo::save_bmp("scaling_linear.bmp", output.data(), ggo::y_32f_yu, output.width(), output.height(), output.line_byte_step());

  ggo::scale_2d<ggo::scaling_algo::cubic_integration, ggo::scaling_algo::cubic_integration>(
    input.data(), input.width(), input.height(), input.line_byte_step(),
    output.data(), output.width(), output.height(), output.line_byte_step());

  ggo::save_bmp("scaling_cubic.bmp", output.data(), ggo::y_32f_yu, output.width(), output.height(), output.line_byte_step());

}