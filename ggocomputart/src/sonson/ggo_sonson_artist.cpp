#include "ggo_sonson_artist.h"
#include <kernel/ggo_kernel.h>

//////////////////////////////////////////////////////////////
ggo::sonson_artist::sonson_artist()
{
  for (int i = 0; i < 32; ++i)
  {
    animated_glowing_circle circle;

    circle._center_start = { ggo::rand(-0.25f, 1.25f),  ggo::rand(-0.25f, 1.25f) };
    circle._center_end   = circle._center_start + ggo::rand<float>(0.1f, 0.2f) * ggo::vec2_f::from_angle(ggo::rand(0.f, 2 * ggo::pi<float>()));

    circle._r_angle_start = ggo::rand(0.f, 2 * ggo::pi<float>());
    circle._g_angle_start = ggo::rand(0.f, 2 * ggo::pi<float>());
    circle._b_angle_start = ggo::rand(0.f, 2 * ggo::pi<float>());

    circle._r_angle_end = circle._r_angle_start + ggo::rand(1.f, 2.f);
    circle._g_angle_end = circle._g_angle_start + ggo::rand(1.f, 2.f);
    circle._b_angle_end = circle._b_angle_start + ggo::rand(1.f, 2.f);

    circle._radius = ggo::rand<float>(0.1f, 0.25f);
    circle._inner_size = ggo::rand<float>(0.01f, 0.04f);
    circle._outter_size = ggo::rand<float>(0.002f, 0.08f);
    circle._opacity = ggo::rand<float>(0.25f, 0.75f);

    _circles.push_back(circle);
  }
}

//////////////////////////////////////////////////////////////
std::vector<ggo::sonson_artist::glowing_circle> ggo::sonson_artist::get_circles(float progress) const
{
  std::vector<glowing_circle> circles;

  for (const auto & animated_circle : _circles)
  {
    glowing_circle circle;

    circle._center = ggo::linerp(animated_circle._center_start, animated_circle._center_end, 1 - progress);
    circle._r_angle = ggo::linerp(animated_circle._r_angle_start, animated_circle._r_angle_end, 1 - progress);
    circle._g_angle = ggo::linerp(animated_circle._g_angle_start, animated_circle._g_angle_end, 1 - progress);
    circle._b_angle = ggo::linerp(animated_circle._b_angle_start, animated_circle._b_angle_end, 1 - progress);
    circle._opacity = animated_circle._opacity;
    circle._outter_size = animated_circle._outter_size;
    circle._radius = {
      animated_circle._radius - animated_circle._outter_size - animated_circle._inner_size,
      animated_circle._radius - animated_circle._inner_size,
      animated_circle._radius + animated_circle._inner_size,
      animated_circle._radius + animated_circle._outter_size + animated_circle._inner_size };

    circles.push_back(circle);
  }

  return circles;
}
