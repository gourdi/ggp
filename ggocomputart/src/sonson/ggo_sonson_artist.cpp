#include "ggo_sonson_artist.h"
#include <kernel/ggo_kernel.h>

//////////////////////////////////////////////////////////////
ggo::sonson_artist::sonson_artist()
{
  for (int i = 0; i < 32; ++i)
  {
    animated_glowing_circle circle;

    circle._center = { ggo::rand(-0.25f, 1.25f),  ggo::rand(-0.25f, 1.25f) };
    circle._velocity = ggo::rand<float>(0.01f, 0.02f) * ggo::vec2_f::from_angle(ggo::rand(0.f, 2 * ggo::pi<float>()));

    circle._r_angle = ggo::rand(0.f, 2 * ggo::pi<float>());
    circle._g_angle = ggo::rand(0.f, 2 * ggo::pi<float>());
    circle._b_angle = ggo::rand(0.f, 2 * ggo::pi<float>());

    circle._dr_angle = ggo::rand(0.01f, 0.02f);
    circle._dg_angle = ggo::rand(0.01f, 0.02f);
    circle._db_angle = ggo::rand(0.01f, 0.02f);

    circle._radius = ggo::rand<float>(0.1f, 0.25f);
    circle._inner_size = ggo::rand<float>(0.01f, 0.04f);
    circle._outter_size = ggo::rand<float>(0.002f, 0.08f);
    circle._opacity = ggo::rand<float>(0.25f, 0.75f);

    _circles.push_back(circle);
  }
}

//////////////////////////////////////////////////////////////
bool ggo::sonson_artist::update(ggo::ratio frame_duration)
{
  _elapsed_time += frame_duration;
  if (_elapsed_time > 10)
  {
    return false;
  }

  for (auto & animated_circle : _circles)
  {
    animated_circle._center += animated_circle._velocity * to<float>(frame_duration);

    animated_circle._r_angle += animated_circle._dr_angle * to<float>(frame_duration);
    animated_circle._g_angle += animated_circle._dg_angle * to<float>(frame_duration);
    animated_circle._b_angle += animated_circle._db_angle * to<float>(frame_duration);
  }

  return true;
}

//////////////////////////////////////////////////////////////
std::vector<ggo::sonson_artist::glowing_circle> ggo::sonson_artist::get_circles() const
{
  std::vector<glowing_circle> circles;

  for (const auto & animated_circle : _circles)
  {
    glowing_circle circle;

    circle._center = animated_circle._center;
    circle._r_angle = animated_circle._r_angle;
    circle._g_angle = animated_circle._g_angle;
    circle._b_angle = animated_circle._b_angle;
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
