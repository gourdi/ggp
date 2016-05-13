#include <ggo_nonreg.h>
#include <ggo_kdtree.h>
#include <ggo_shapes2d.h>
#include <ggo_shapes3d.h>
#include <ggo_rgb_image_buffer.h>
#include <ggo_paint.h>
#include <ggo_bmp.h>

////////////////////////////////////////////////////////////////////
GGO_TEST(tree3d, random_points)
{
  const int SIZE = 500;
  const float RADIUS = 50;

  ggo::rgb_image_buffer_uint8 data(SIZE, SIZE, ggo::color::BLACK);

  // Create random points.
  using tree_point = ggo::kdtree<2, void *>::data_point;
  std::vector<tree_point> points;
  for (int i = 0; i < 500; ++i)
  {
    float x = ggo::rand_float(0.f, static_cast<float>(SIZE));
    float y = ggo::rand_float(0.f, static_cast<float>(SIZE));
    points.push_back({ { x, y }, nullptr });
  }

  auto disc = std::make_shared<ggo::disc_float>(SIZE / 2.f, SIZE / 2.f, RADIUS);

  ggo::paint(data, disc, ggo::color::RED);

  for (const auto & point : points)
  {
    ggo::paint(data, std::make_shared<ggo::disc_float>(point._pos[0], point._pos[1], 2.f), ggo::color::WHITE);
  }

  ggo::kdtree<2, void *> tree(points);
  auto inside_points = tree.find_points({ SIZE / 2.f, SIZE / 2.f }, RADIUS);

  for (const auto & point : inside_points)
  {
    ggo::paint(data, std::make_shared<ggo::disc_float>(point._pos[0], point._pos[1], 4.f), ggo::color::BLUE);
  }

  ggo::save_bmp("kdtree.bmp", data.data(), SIZE, SIZE);
}