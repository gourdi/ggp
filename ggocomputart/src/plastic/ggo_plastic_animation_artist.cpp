//#include "ggo_plastic_animation_artist.h"
//#include <kernel/ggo_vec3.h>
//#include <2d/ggo_image.h>
//
////////////////////////////////////////////////////////////////
//ggo::plastic_animation_artist::plastic_animation_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order)
//:
//fixed_frames_count_animation_artist_abc(width, height, line_byte_step, pixel_type, memory_lines_order, 200)
//{
//  for (int i = 0; i < 50; ++i)
//  {
//    plastic_animation_artist::params params;
//
//    params._center = { ggo::rand(-1.5f, 1.5f), ggo::rand(-1.5f, 1.5f) };
//    params._radius = ggo::rand(0.1f, 0.5f);
//    params._angle = ggo::rand(0.f, 2 * ggo::pi<float>());
//    params._dangle = ggo::rand(0.02f, 0.08f);
//    params._den = ggo::rand(0.5f, 2.f);
//    params._mult = ggo::rand(0.5f, 2.f);
//    params._power = ggo::rand(2.f, 3.f);
//
//    _params.push_back(params);
//  }
//
//  _color = ggo::from_hsv<ggo::rgb_32f>(ggo::rand<float>(), ggo::rand<float>(), 1);
//  _altitude_factor = ggo::rand<float>(0.04f, 0.08f);
//}
//
////////////////////////////////////////////////////////////////
//void ggo::plastic_animation_artist::render_frame(void * buffer, int frame_index, float time_step)
//{
//  for (auto & anim_params : _params)
//  {
//    anim_params._angle += anim_params._dangle;
//  }
//
//  if (pixel_type() == ggo::pixel_type::bgrx_8u && memory_lines_order() == ggo::lines_order::down)
//  {
//    render_frame_t<ggo::pixel_type::bgrx_8u, ggo::lines_order::down>(buffer);
//  }
//  else if (pixel_type() == ggo::pixel_type::rgb_8u && memory_lines_order() == ggo::lines_order::up)
//  {
//    render_frame_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up>(buffer);
//  }
//  else if (pixel_type() == ggo::pixel_type::rgb_8u && memory_lines_order() == ggo::lines_order::down)
//  {
//    render_frame_t<ggo::pixel_type::rgb_8u, ggo::lines_order::down>(buffer);
//  }
//  else
//  {
//    GGO_FAIL();
//  }
//}
//
////////////////////////////////////////////////////////////////
//template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
//void ggo::plastic_animation_artist::render_frame_t(void * buffer) const
//{
//  ggo::image_t<pixel_type, memory_lines_order> img(buffer, { width(), height() }, line_byte_step());
//
//  const float width_f = static_cast<float>(width());
//  const float height_f = static_cast<float>(height());
//  const float range_x = width_f > height_f ? 1.f : width_f / height_f;
//  const float range_y = height_f > width_f ? 1.f : height_f / width_f;
//
//  for (int y = 0; y < height(); ++y)
//  {
//    const float y1 = ggo::map(y - 3 / 8.f, 0.f, height_f, -range_y, range_y);
//    const float y2 = ggo::map(y + 3 / 8.f, 0.f, height_f, -range_y, range_y);
//
//    for (int x = 0; x < width(); ++x)
//    {
//      // Compute altitude.
//      const float x1 = ggo::map(x - 3 / 8.f, 0.f, width_f, -range_x, range_x);
//      const float x2 = ggo::map(x + 3 / 8.f, 0.f, width_f, -range_x, range_x);
//
//      const float z11 = compute_altitude(x1, y1);
//      const float z12 = compute_altitude(x1, y2);
//      const float z21 = compute_altitude(x2, y1);
//      const float z22 = compute_altitude(x2, y2);
//
//      // Render the slope.
//      const float dx = x2 - x1;
//      const float dy = y2 - y1;
//      const ggo::vec3_f v1(dx, dy, z22 - z11);
//      const ggo::vec3_f v2(dx, -dy, z21 - z12);
//      const ggo::vec3_f normal = ggo::normalize(ggo::cross(v1, v2));
//
//      const ggo::rgb_32f pixel_color = _color * (_altitude_factor + std::abs(normal.z()) / _altitude_factor);
//
//      img.write_pixel(x, y, ggo::convert_color_to<ggo::rgb_8u>(pixel_color));
//    }
//  }
//}
//
////////////////////////////////////////////////////////////////
//float ggo::plastic_animation_artist::compute_altitude(float x, float y) const
//{
//  float z = 0;
//  for (const auto & param : _params)
//  {
//    pos2_f delta = param._radius * vec2_f::from_angle(param._angle) - vec2_f(x, y);
//
//    float h = hypot(delta);
//    z += param._den / (0.1f + std::pow(h * param._mult, param._power));
//    z += param._den / (0.1f + std::pow(h * param._mult, param._power));
//  }
//
//  return z;
//}
//
//
