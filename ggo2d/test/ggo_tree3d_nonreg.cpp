#include <ggo_nonreg.h>
#include <ggo_tree3d.h>
#include <ggo_shapes2d.h>
#include <ggo_shapes3d.h>
#include <ggo_rgb_image_data.h>
#include <ggo_paint.h>
#include <ggo_bmp.h>

////////////////////////////////////////////////////////////////////
GGO_TEST(tree3d, random_points)
{
  const int SIZE = 500;
  const float RADIUS = 50;

  ggo::rgb_image_data_uint8 image_data(SIZE, SIZE, ggo::color::BLACK);

  // Create random points.
  std::vector<ggo::point3d_float> points;
  for (int i = 0; i < 500; ++i)
  {
    points.push_back({ ggo::rand_float(0, SIZE), ggo::rand_float(0, SIZE), 0.f });
  }

  auto disc = std::make_shared<ggo::disc_float>(SIZE / 2, SIZE / 2, RADIUS);

  ggo::paint(image_data, disc, ggo::color::RED);

  for (const auto & point : points)
  {
    ggo::paint(image_data, std::make_shared<ggo::disc_float>(point.x(), point.y(), 2), ggo::color::WHITE);
  }

  ggo::tree3d tree(points);
  auto inside_points = tree.find_points({ SIZE / 2.f, SIZE / 2.f, 0.f }, RADIUS);

  for (const auto & point : inside_points)
  {
    ggo::paint(image_data, std::make_shared<ggo::disc_float>(point.x(), point.y(), 4), ggo::color::BLUE);
  }

  ggo::save_bmp("tree3d.bmp", image_data.get_buffer(), SIZE, SIZE);
}