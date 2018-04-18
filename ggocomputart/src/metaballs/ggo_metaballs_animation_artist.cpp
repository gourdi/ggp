#include "ggo_metaballs_animation_artist.h"
#include <ggo_antialiasing_renderer.h>
#include <raytracer/cameras/ggo_point_camera.h>

namespace
{
  const int frames_count = 300;
}

//////////////////////////////////////////////////////////////
ggo::metaballs_animation_artist::metaballs_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt)
:
animation_artist_abc(width, height, line_step, pbf, rt)
{
	
}

//////////////////////////////////////////////////////////////
void ggo::metaballs_animation_artist::init_animation()
{
  _frame_index = -1;

  const float ball_size = 2;

  for (int i = 0; i < 200; ++i)
  {
    moving_center center;

    center._center = ggo::pos3f(ggo::rand<float>(-ball_size, ball_size), ggo::rand<float>(-ball_size, ball_size), ggo::rand<float>(-ball_size, ball_size));
    center._radius = ggo::rand<float>(0, ball_size);
    center._start_angle1 = ggo::rand<float>(0, 2 * ggo::pi<float>());
    center._start_angle2 = ggo::rand<float>(0, 2 * ggo::pi<float>());
    center._end_angle1 = ggo::rand<float>(0, 2 * ggo::pi<float>());
    center._end_angle2 = ggo::rand<float>(0, 2 * ggo::pi<float>());

    _centers.push_back(center);
  }

  float angle = ggo::rand<float>(0, 2 * ggo::pi<float>());
  _params._light2 = ggo::pos3f(1000 * std::cos(angle), 1000 * std::sin(angle), 1000.f);
  _camera_basis.set_pos(0, 0, 25);
}

//////////////////////////////////////////////////////////////
bool ggo::metaballs_animation_artist::prepare_frame()
{
  ++_frame_index;

  if (_frame_index >= frames_count)
  {
    return false;
  }

  std::vector<ggo::pos3f> centers;

  float t = ggo::ease_inout_to<float>(_frame_index, frames_count);

  _params._centers.clear();
  for (const auto & center : _centers)
  {
    float angle1 = ggo::map<float>(t, 0, 1, center._start_angle1, center._end_angle1);
    float angle2 = ggo::map<float>(t, 0, 1, center._start_angle2, center._end_angle2);

    float x1 = center._radius;
    float y1 = 0;
    float z1 = 0;

    float x2 = std::cos(angle1) * x1 - std::sin(angle1) * y1;
    float y2 = std::sin(angle1) * x1 + std::cos(angle1) * y1;
    float z2 = z1;

    float x3 = std::cos(angle2) * x2 - std::sin(angle2) * z2;
    float y3 = y2;
    float z3 = std::sin(angle2) * x2 + std::cos(angle2) * z2;

    x3 += center._center.x();
    y3 += center._center.y();
    z3 += center._center.z();

    _params._centers.push_back(ggo::pos3f(x3, y3, z3));
  }

  return true;
}

//////////////////////////////////////////////////////////////
void ggo::metaballs_animation_artist::render_frame(void * buffer, const ggo::rect_int & clipping)
{
  ggo::antialiasing_point_camera camera(get_width(), get_height(), _camera_basis, 0.1f);
  ggo::antialiasing_renderer renderer(camera);
  ggo::metaballs_artist::render_bitmap(buffer, get_width(), get_height(), get_line_step(), get_pixel_buffer_format(), renderer, _params);
}
