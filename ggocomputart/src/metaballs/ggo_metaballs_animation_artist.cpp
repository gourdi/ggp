#include "ggo_metaballs_animation_artist.h"
#include <raytracer/renderers/ggo_antialiasing_renderer.h>
#include <raytracer/cameras/ggo_point_camera.h>

//////////////////////////////////////////////////////////////
ggo::metaballs_animation_artist::metaballs_animation_artist(
  int width, int height, int line_byte_step,
  ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order,
  ggo::ratio fps)
:
  progress_animation_artist(width, height, line_byte_step, pixel_type, memory_lines_order, 8, fps)
{
  const float ball_size = 2;

  for (int i = 0; i < 200; ++i)
  {
    moving_center center;

    center._center = ggo::pos3_f(ggo::rand<float>(-ball_size, ball_size), ggo::rand<float>(-ball_size, ball_size), ggo::rand<float>(-ball_size, ball_size));
    center._radius = ggo::rand<float>(0, ball_size);
    center._start_angle1 = ggo::rand<float>(0, 2 * ggo::pi<float>());
    center._start_angle2 = ggo::rand<float>(0, 2 * ggo::pi<float>());
    center._end_angle1 = ggo::rand<float>(0, 2 * ggo::pi<float>());
    center._end_angle2 = ggo::rand<float>(0, 2 * ggo::pi<float>());

    _centers.push_back(center);
  }

  float angle = ggo::rand<float>(0, 2 * ggo::pi<float>());
  _params._light2 = ggo::pos3_f(1000 * std::cos(angle), 1000 * std::sin(angle), 1000.f);
  _camera_basis.set_pos(0, 0, 25);
}

//////////////////////////////////////////////////////////////
void ggo::metaballs_animation_artist::render_frame(void * buffer, float progress)
{
  std::vector<ggo::pos3_f> centers;

  progress = ggo::ease_inout(progress);

  _params._centers.clear();
  for (const auto & center : _centers)
  {
    float angle1 = ggo::linerp(center._start_angle1, center._end_angle1, progress);
    float angle2 = ggo::linerp(center._start_angle2, center._end_angle2, progress);

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

    _params._centers.push_back(ggo::pos3_f(x3, y3, z3));
  }

  ggo::antialiasing_point_camera camera(width(), height(), _camera_basis, 0.1f);
  ggo::antialiasing_renderer renderer(camera);
  ggo::metaballs_artist::render_bitmap(buffer, width(), height(), line_byte_step(), pixel_type(), memory_lines_order(), renderer, _params);
}
