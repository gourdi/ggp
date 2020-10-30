#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/marching_algorithms/ggo_marching_squares.h>
#include <2d/ggo_color.h>
#include <2d/paint/ggo_paint_layer.h>
#include <2d/io/ggo_bmp.h>
#if 0
/////////////////////////////////////////////////////////////////////
GGO_TEST(marching_squares, circle)
{
  auto func = [](float x, float y)
  {
    return x * x + y * y - 1.f;
  };
  auto cells = ggo::marching_squares(func, { -2.f, -2.f }, 40, 0.1f);

  const int size = 400;
  const ggo::pos2_f offset(0.5f * size, 0.5f * size);

  ggo::canvas<ggo::rgb_8u> canvas;
  for (const auto & cell : cells)
  {
    for (const auto & segment : cell._segments)
    {
      ggo::capsule_f capsule(offset + 50.f * segment.p1(), offset + 50.f * segment.p2(), 2.f);
      
      canvas.make_layer(capsule, ggo::white_8u());
    }
  }

  ggo::image_t<ggo::pixel_type::rgb_8u> image({ size, size });

  ggo::paint<ggo::sampling_4x4>(image, canvas);

  ggo::save_bmp("marching_squares.bmp", image);
}
#endif