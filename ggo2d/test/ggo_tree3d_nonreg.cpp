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
  std::vector<std::pair<ggo::point3d_float, void*>> points;
  for (int i = 0; i < 500; ++i)
  {
    float x = ggo::rand_float(0.f, static_cast<float>(SIZE));
    float y = ggo::rand_float(0.f, static_cast<float>(SIZE));
    points.push_back({ { x, y, 0.f }, nullptr });
  }

  auto disc = std::make_shared<ggo::disc_float>(SIZE / 2.f, SIZE / 2.f, RADIUS);

  ggo::paint(image_data, disc, ggo::color::RED);

  for (const auto & point : points)
  {
    ggo::paint(image_data, std::make_shared<ggo::disc_float>(point.first.x(), point.first.y(), 2.f), ggo::color::WHITE);
  }

  ggo::tree3d<void*> tree(points);
  auto inside_points = tree.find_points({ SIZE / 2.f, SIZE / 2.f, 0.f }, RADIUS);

  for (const auto & point : inside_points)
  {
    ggo::paint(image_data, std::make_shared<ggo::disc_float>(point.first.x(), point.first.y(), 4.f), ggo::color::BLUE);
  }

  ggo::save_bmp("tree3d.bmp", image_data.get_buffer(), SIZE, SIZE);
}