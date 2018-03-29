#include <ggo_nonreg.h>
#include <ggo_kdtree.h>
#include <ggo_shapes2d.h>
#include <ggo_shapes3d.h>
#include <ggo_buffer_paint.h>
#include <ggo_bmp.h>

////////////////////////////////////////////////////////////////////
GGO_TEST(kdtree, random_points)
{
  const int size = 500;
  const float radius = 50;

  std::vector<uint8_t> buffer(3 * size * size, 0);

  // Create random points.
  using tree_point = ggo::kdtree<void *, 2>::data_point;
  std::vector<tree_point> points;
  for (int i = 0; i < 500; ++i)
  {
    float x = ggo::rand<float>(0.f, static_cast<float>(size));
    float y = ggo::rand<float>(0.f, static_cast<float>(size));
    points.push_back({ { x, y }, nullptr });
  }

  ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(buffer.data(), size, size, 3 * size, ggo::disc_float(size / 2.f, size / 2.f, radius), ggo::red_8u());

  for (const auto & point : points)
  {
    ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(buffer.data(), size, size, 3 * size, ggo::disc_float(point._pos.get<0>(), point._pos.get<1>(), 2.f), ggo::white_8u());
  }

  ggo::kdtree<void *, 2> tree(points);
  auto inside_points = tree.find_points({ size / 2.f, size / 2.f }, radius);

  for (const auto & point : inside_points)
  {
    ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(buffer.data(), size, size, 3 * size, ggo::disc_float(point._pos.get<0>(), point._pos.get<1>(), 2.f), ggo::blue_8u());
  }

  ggo::save_bmp("kdtree.bmp", buffer.data(), ggo::rgb_8u_yu, size, size, 3 * size);
}
