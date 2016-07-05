#include <ggo_nonreg.h>
#include <ggo_marching_squares.h>
#include <ggo_paint.h>
#include <ggo_rgb_image_buffer.h>
#include <ggo_bmp.h>

GGO_TEST(marching_squares, test1)
{
  auto func = [](float x, float y)
  {
    return x * x + y * y - 1.f;
  };
  auto cells = ggo::marching_squares(func, { -2.f, -2.f }, 40, 0.1f);

  const int size = 400;
  const ggo::pos2f offset(0.5f * size, 0.5f * size);

  std::vector<ggo::rgb_layer> layers;
  for (const auto & cell : cells)
  {
    for (const auto & segment : cell._segments)
    {
      auto extended_segment = std::make_shared<ggo::extended_segment_float>(offset + 50.f * segment.p1(), offset + 50.f * segment.p2(), 2.f);
      layers.emplace_back(extended_segment, ggo::color::BLUE);
    }
  }

  ggo::rgb_image_buffer_uint8 image(size, size, ggo::color::WHITE);
  ggo::paint(image, layers);

  ggo::save_bmp("marching_squares.bmp", image.data(), size, size);
}
