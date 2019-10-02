#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/memory/ggo_array.h>
#include <kernel/math/signal_processing/ggo_scale2d.h>
#include <2d/io/ggo_bmp.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(scaling, y32f)
{
  ggo::array2d_32f input({
    { 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, },
    { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, },
    { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, },
    { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, },
    { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, },
    { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, },
    { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, },
    { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, },
    { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, },
    { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, } });

  ggo::array2d_32f output(100, 100);

  ggo::scale_2d<ggo::scaling_algo::linear_integration, ggo::scaling_algo::linear_integration>(
    input.data(), input.width(), input.height(), int(sizeof(float) * input.width()),
    output.data(), output.width(), output.height(), int(sizeof(float) * output.width()));

  ggo::save_bmp("scaling_linear.bmp", output.data(), ggo::pixel_type::y_32f, ggo::lines_order::up, output.width(), output.height(), int(sizeof(float) * output.width()));

  ggo::scale_2d<ggo::scaling_algo::cubic_integration, ggo::scaling_algo::cubic_integration>(
    input.data(), input.width(), input.height(), int(sizeof(float) * input.width()),
    output.data(), output.width(), output.height(), int(sizeof(float) * output.width()));

  ggo::save_bmp("scaling_cubic.bmp", output.data(), ggo::pixel_type::y_32f, ggo::lines_order::up, output.width(), output.height(), int(sizeof(float) * output.width()));
}