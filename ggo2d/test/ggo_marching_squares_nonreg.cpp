#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/marching_algorithms/ggo_marching_squares.h>
#include <2d/ggo_color.h>
#include <2d/paint/ggo_paint.h>
#include <2d/io/ggo_bmp.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(marching_squares, circle)
{
  auto func = [](float x, float y)
  {
    return x * x + y * y - 1.f;
  };
  auto cells = ggo::marching_squares(func, { -2.f, -2.f }, 40, 0.1f);

  const int size = 400;
  const ggo::pos2f offset(0.5f * size, 0.5f * size);

  using color_capsule = ggo::static_paint_shape<ggo::capsule_float, ggo::rgb_8u>;
  std::vector<color_capsule> segments;
  for (const auto & cell : cells)
  {
    for (const auto & segment : cell._segments)
    {
      ggo::capsule_float capsule(offset + 50.f * segment.p1(), offset + 50.f * segment.p2(), 2.f);
      segments.emplace_back(capsule, ggo::white<ggo::rgb_8u>());
    }
  }

  std::vector<uint8_t> buffer(3 * size * size, 0);

  ggo::paint_shapes<ggo::rgb_8u_yu, ggo::sampling_4x4>(buffer.data(), size, size, 3 * size, segments);

  ggo::save_bmp("marching_squares.bmp", buffer.data(), ggo::rgb_8u_yu, size, size, 3 * size);
}
