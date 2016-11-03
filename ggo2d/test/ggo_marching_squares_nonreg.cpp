#include <ggo_nonreg.h>
#include <ggo_marching_squares.h>
#include <ggo_color.h>
#include <ggo_multi_shape_paint.h>
#include <ggo_bmp.h>

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

  using color_segment = ggo::solid_color_shape<ggo::extended_segment_float, ggo::color_8u>;
  std::vector<color_segment> segments;
  for (const auto & cell : cells)
  {
    for (const auto & segment : cell._segments)
    {
      ggo::extended_segment_float segment(offset + 50.f * segment.p1(), offset + 50.f * segment.p2(), 2.f);
      segments.emplace_back(segment, ggo::white<ggo::color_8u>());
    }
  }

  std::vector<uint8_t> buffer(3 * size * size, 0);
  ggo::paint_shapes<ggo::rgb_8u_yu, ggo::sampling_4x4>(buffer.data(), size, size, 3 * size, segments.begin(), segments.end());

  ggo::save_bmp("marching_squares.bmp", buffer.data(), ggo::rgb_8u_yu, size, size, 3 * size);
}
