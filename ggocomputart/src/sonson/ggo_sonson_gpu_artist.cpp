#include "ggo_sonson_gpu_artist.h"
#include <2d/ggo_color.h>

//////////////////////////////////////////////////////////////
std::string ggo::sonson_gpu_artist::get_fragment_shader() const
{
  std::string source{
#include "sonson.glsl"
  };

  return source;
}

//////////////////////////////////////////////////////////////
std::map<std::string, ggo::uniform> ggo::sonson_gpu_artist::update(bool& finished, ggo::size render_size)
{
  std::map<std::string, ggo::uniform> uniforms;

  auto now = std::chrono::steady_clock::now();
  auto frame_duration_us = std::chrono::duration_cast<std::chrono::microseconds>(now - _last_frame_time_point).count();
  _last_frame_time_point = now;

  finished = true;

  if (_artist.update(ggo::ratio(frame_duration_us, 1000000)) == true)
  {
    finished = false;

    int min_size = std::min(render_size.width(), render_size.height());
    float offset_length = 0.01f * min_size;

    std::vector<float> params;
    auto circles = _artist.get_circles();
    for (const auto& circle : circles)
    {
      auto push_circle = [&](const auto & circle, vec2_f center_offset, rgb_32f color)
      {
        params.push_back(circle._center.x() * render_size.width() + center_offset.x());
        params.push_back(circle._center.y() * render_size.height() + center_offset.y());
        params.push_back(color.r());
        params.push_back(color.g());
        params.push_back(color.b());
        params.push_back(circle._radius[0] * min_size);
        params.push_back(circle._radius[1] * min_size);
        params.push_back(circle._radius[2] * min_size);
        params.push_back(circle._radius[3] * min_size);
        params.push_back(circle._outter_size * min_size);
      };

      ggo::vec2_f r_offset = offset_length * ggo::vec2_f::from_angle(circle._r_angle);
      ggo::vec2_f g_offset = offset_length * ggo::vec2_f::from_angle(circle._g_angle);
      ggo::vec2_f b_offset = offset_length * ggo::vec2_f::from_angle(circle._b_angle);

      push_circle(circle, r_offset, circle._opacity * ggo::red_32f());
      push_circle(circle, g_offset, circle._opacity * ggo::green_32f());
      push_circle(circle, b_offset, circle._opacity * ggo::blue_32f());
    }

    uniforms = { { "params", { params, 1 } } };
  }

  return uniforms;
}
