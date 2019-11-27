#include "ggo_plastic_animation_artist.h"
#include <kernel/ggo_vec3.h>
#include <2d/ggo_image.h>

//////////////////////////////////////////////////////////////
ggo::plastic_animation_artist::plastic_animation_artist(
  int width, int height, int line_byte_step, 
  ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order,
  ggo::ratio fps)
:
progress_animation_artist(width, height, line_byte_step, pixel_type, memory_lines_order, 10, fps)
{
}

//////////////////////////////////////////////////////////////
void ggo::plastic_animation_artist::render_frame(void * buffer, float progress)
{
  if (pixel_type() == ggo::pixel_type::bgrx_8u && memory_lines_order() == ggo::lines_order::down)
  {
    render_frame_t<ggo::pixel_type::bgrx_8u, ggo::lines_order::down>(buffer, progress);
  }
  else if (pixel_type() == ggo::pixel_type::rgb_8u && memory_lines_order() == ggo::lines_order::up)
  {
    render_frame_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up>(buffer, progress);
  }
  else if (pixel_type() == ggo::pixel_type::rgb_8u && memory_lines_order() == ggo::lines_order::down)
  {
    render_frame_t<ggo::pixel_type::rgb_8u, ggo::lines_order::down>(buffer, progress);
  }
  else
  {
    GGO_FAIL();
  }
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
void ggo::plastic_animation_artist::render_frame_t(void * buffer, float progress) const
{
  ggo::image_t<pixel_type, memory_lines_order> img(buffer, { width(), height() }, line_byte_step());

  const float width_f = static_cast<float>(width());
  const float height_f = static_cast<float>(height());
  const float range_x = width_f > height_f ? 1.f : width_f / height_f;
  const float range_y = height_f > width_f ? 1.f : height_f / width_f;

  for (int y = 0; y < height(); ++y)
  {
    const float y1 = ggo::map(y - 3 / 8.f, 0.f, height_f, -range_y, range_y);
    const float y2 = ggo::map(y + 3 / 8.f, 0.f, height_f, -range_y, range_y);

    for (int x = 0; x < width(); ++x)
    {
      // Compute altitude.
      const float x1 = ggo::map(x - 3 / 8.f, 0.f, width_f, -range_x, range_x);
      const float x2 = ggo::map(x + 3 / 8.f, 0.f, width_f, -range_x, range_x);

      const float z11 = compute_altitude(progress, x1, y1);
      const float z12 = compute_altitude(progress, x1, y2);
      const float z21 = compute_altitude(progress, x2, y1);
      const float z22 = compute_altitude(progress, x2, y2);

      // Render the slope.
      const float dx = x2 - x1;
      const float dy = y2 - y1;
      const ggo::vec3_f v1(dx, dy, z22 - z11);
      const ggo::vec3_f v2(dx, -dy, z21 - z12);
      const ggo::vec3_f normal = ggo::normalize(ggo::cross(v1, v2));

      const ggo::rgb_32f pixel_color = _artist.color() * (_artist.altitude_factor() + std::abs(normal.z()) / _artist.altitude_factor());

      img.write_pixel(x, y, ggo::convert_color_to<ggo::rgb_8u>(pixel_color));
    }
  }
}

//////////////////////////////////////////////////////////////
float ggo::plastic_animation_artist::compute_altitude(float progress, float x, float y) const
{
  float z = 0;

  _artist.for_each_param(progress, [&](float den, float mult, float power, ggo::pos2_f pos)
  {
    vec2_f delta = pos - vec2_f(x, y);

    float h = dot(delta, delta);
    z += den / (0.1f + std::pow(h * mult, power));
    z += den / (0.1f + std::pow(h * mult, power));
  });

  return z;
}


