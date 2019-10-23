#include "ggo_polygus_bimap_artist.h"
#include <kernel/math/ggo_coordinates_conversions.h>
#include <2d/ggo_image.h>
#include <2d/fill/ggo_fill.h>
#include <2d/paint/ggo_brush.h>
#include <2d/processing/ggo_gaussian_blur.h>
#include <2d/processing/ggo_mask.h>
#include <2d/processing/ggo_blit.h>

namespace
{
#if 0
  template <ggo::image_format format>
  void fill_head1(void * buffer, int width, int height, int line_byte_step, ggo::rect_int clipping)
  {
    const ggo::rgb_8u colors[2] = {
      ggo::from_hsv_8u(ggo::rand<float>(), 1.f, 1.f),
      ggo::from_hsv_8u(ggo::rand<float>(), 1.f, 1.f) };

    ggo::fill_solid<format>(buffer, width, height, line_byte_step, colors[0], clipping);

    const float delta = std::min(width, height) / 50.f;
    const float radius = std::min(width, height) / 200.f;
    const float scramble_dist = std::min(width, height) / 400.f;
    ggo::pos2_f center = { delta / 2, delta / 2 };
    for (;;)
    {
      const float phase = ggo::rand(0.f, ggo::pi_f);
      const ggo::pos2_f current_center = center + ggo::vec2_f(ggo::rand(-scramble_dist, scramble_dist), ggo::rand(-scramble_dist, scramble_dist));
      const ggo::pos2_f p1 = current_center + radius * ggo::vec2_f::from_angle(phase);
      const ggo::pos2_f p2 = current_center + radius * ggo::vec2_f::from_angle(phase + 2 * ggo::pi_f / 3);
      const ggo::pos2_f p3 = current_center + radius * ggo::vec2_f::from_angle(phase + 4 * ggo::pi_f / 3);

      ggo::paint<format, ggo::sampling_4x4>(buffer, width, height, line_byte_step, ggo::triangle2d_f(p1, p2, p3), colors[1], 0.5f, clipping);

      center.x() += delta;
      if (center.x() > width)
      {
        center.x() = delta / 2;
        center.y() += delta;
      }
      if (center.y() > height)
      {
        break;
      }
    }
  }
#endif
}

//////////////////////////////////////////////////////////////
std::vector<ggo::pos2_f> ggo::polygus_bitmap_artist::create_head() const
{
  ggo::cubic_curve_float scaling;
  scaling.push_point(-1, 1);
  scaling.push_point(1, 1);

  {
    float dx = ggo::rand(0.5f, 0.75f);
    float dy = ggo::rand(0.f, 0.5f);
    scaling.push_point(-1 + dx, 1 - dy);
    scaling.push_point(-1 - dx, 1 + dy);
  }

  {
    float dx = ggo::rand(0.5f, 0.75f);
    float dy = ggo::rand(0.f, 0.5f);
    scaling.push_point(1 - dx, 1 - dy);
    scaling.push_point(1 + dx, 1 + dy);
  }

  auto compute_point = [&](int step)
  {
    float y = std::sin(2 * step * ggo::pi_f / 100);
    float s = scaling.evaluate(y);
    float x = s * std::cos(2 * step * ggo::pi_f / 100);
    return ggo::pos2_f(x, y);
  };

  std::vector<ggo::pos2_f> points;
  for (int i = 0; i < 100; ++i)
  {
    points.push_back(compute_point(i));
  }

  return points;
}

namespace ggo
{
  template <typename container_t, typename func_t>
  void transform(container_t & container, func_t func)
  {
    std::transform(container.begin(), container.end(), container.begin(), func);
  }
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
void ggo::polygus_bitmap_artist::render_polygus(void * buffer) const
{
#if 0
  constexpr int nb_gus = 4;
  constexpr ggo::image_format gray_format = ggo::image_format_traits<format>::gray_format;
  const float thickness = 0.002f * min_size();

  // Image background.
  {
    auto bkgd_color = ggo::from_hsv_8u(ggo::rand<float>(), 1.f, 1.f);
    ggo::fill_solid<format>(buffer, width(), height(), line_byte_step(), bkgd_color);
  }

  float cell_size = (width() > height() ? height() : width()) / static_cast<float>(nb_gus);
  float offset_x = width() > height() ? (width() - height()) / 2.f : 0.f;
  float offset_y = height() > width() ? (height() - width()) / 2.f : 0.f;

  struct gus
  {
    struct
    {
      ggo::pos2_f _center;
      float _radius;
      float _outter_radius;
      float _tongue_radius;
      float _dangle;
    } _mouth;

    struct 
    {
      std::vector<ggo::pos2_f> _points;
      ggo::pos2_f _center;
    } _nose;

    ggo::rect_f _rect;
    std::vector<ggo::pos2_f> _head_points;
    std::array<ggo::disc_f, 2> _eyes;
    std::array<ggo::disc_f, 2> _pupils;
  };

  std::vector<gus> all_gus;

  for (int gus_y = 0; gus_y < nb_gus; ++gus_y)
  {
    float bottom = gus_y * cell_size + offset_y;
    float top = (gus_y + 1) * cell_size + offset_y;

    for (int gus_x = 0; gus_x < nb_gus; ++gus_x)
    {
      float left = gus_x * cell_size + offset_x;
      float right = (gus_x + 1) * cell_size + offset_x;

      auto map_point = [&](ggo::pos2_f p)
      {
        float x = ggo::map(p.x(), -1.5f, 1.5f, left, right);
        float y = ggo::map(p.y(), -1.5f, 1.5f, bottom, top);
        return ggo::pos2_f(x, y);
      };

      auto map_distance = [&](float d)
      {
        return ggo::map(d, 0.f, 2 * 1.5f, 0.f, cell_size);
      };

      gus new_gus;
      new_gus._rect = ggo::rect_f::from_left_right_bottom_top(left, right, bottom, top);

      // Head.
      new_gus._head_points = create_head();
      ggo::transform(new_gus._head_points, map_point);

      // Eyes.
      const float eye_radius = ggo::rand(0.2f, 0.3f);
      const float y = ggo::rand(0.3f, 0.7f);
      const float eye_offset = ggo::rand(0.3f, 0.6f - eye_radius);
      const ggo::pos2_f eye_left(-eye_offset, y);
      const ggo::pos2_f eye_right(eye_offset, y);
      new_gus._eyes[0] = ggo::disc_f(map_point(eye_left), map_distance(eye_radius));
      new_gus._eyes[1] = ggo::disc_f(map_point(eye_right), map_distance(eye_radius));

      const float angle = ggo::rand(-ggo::pi_f, ggo::pi_f);
      const float pupil_radius = ggo::rand(0.04f, 0.08f);
      const float dist = ggo::rand(0.f, eye_radius - pupil_radius);
      const ggo::vec2_f pupil_offset = map_distance(dist) * vec2_f::from_angle(angle);
      new_gus._pupils[0] = ggo::disc_f(new_gus._eyes[0].center() + pupil_offset, map_distance(pupil_radius));
      new_gus._pupils[1] = ggo::disc_f(new_gus._eyes[1].center() + pupil_offset, map_distance(pupil_radius));

      // Mouth.
      new_gus._mouth._center = new_gus._rect.center() + rand(-0.1f, 0.1f) * new_gus._rect.height() * ggo::vec2_f(0, 1);
      new_gus._mouth._radius = ggo::rand(0.04f, 0.06f) * min_size();
      new_gus._mouth._outter_radius = thickness + ggo::rand(0.0f, 0.02f) * min_size();
      new_gus._mouth._tongue_radius = 0.04f * min_size();
      new_gus._mouth._dangle = ggo::rand(0.2f, 0.5f);

      // Nose.
      const float nose_sup = new_gus._eyes[0].center().y() - new_gus._eyes[0].radius();
      const float nose_inf = new_gus._mouth._center.y() - new_gus._mouth._radius + new_gus._mouth._outter_radius;
      const float nose_radius = ggo::rand(0.3f, 0.4f) * (nose_sup - nose_inf); 
      new_gus._nose._center = { new_gus._rect.center().x(), 0.5f * (nose_inf + nose_sup) };
      for (int i = 0; i < 100; ++i)
      {
        const float x = new_gus._nose._center.x() + nose_radius * std::cos(i * 2 * ggo::pi_f / 100);
        const float y = new_gus._nose._center.y() + nose_radius * std::sin(i * 2 * ggo::pi_f / 100);
        new_gus._nose._points.push_back(ggo::pos2_f(x, y));
      }

      all_gus.push_back(std::move(new_gus));
    }
  }

  // Shadow.
  {
    ggo::image_t<gray_format> mask(size());
    mask.fill(0x00);

    const ggo::vec2_f offset(min_size() / 100.f, -min_size() / 100.f);

    for (const auto & gus : all_gus)
    {
      // Head.
      ggo::pos2_f center = gus._rect.center();

      std::vector<ggo::static_paint_shape<triangle2d_f, uint8_t>> triangles;
      ggo::pos2_f p_prv = gus._head_points.back();
      for (auto p : gus._head_points)
      {
        ggo::static_paint_shape<triangle2d_f, uint8_t> shape({ center + offset, p + offset, p_prv + offset }, 0xff);

        triangles.emplace_back(shape);
        p_prv = p;
      }
      ggo::paint<mask.format(), ggo::sampling_4x4>(mask.data(), mask.width(), mask.height(), mask.line_byte_step(), triangles);

      // Eyes.
      ggo::paint<mask.format(), ggo::sampling_4x4>(mask.data(), mask.width(), mask.height(), mask.line_byte_step(), gus._eyes[0], 0xff);
      ggo::paint<mask.format(), ggo::sampling_4x4>(mask.data(), mask.width(), mask.height(), mask.line_byte_step(), gus._eyes[1], 0xff);
    }

    ggo::gaussian_blur<mask.format()>(mask.data(), mask.size(), mask.line_byte_step(), min_size() / 100.f);
    ggo::apply_mask<format, gray_format>(buffer, line_byte_step(), mask.data(), mask.line_byte_step(), size(), ggo::black_brush_8u());
  }

  // Heads.
  {
    ggo::image_t<format> heads_bkgd(size());
    for (const auto & gus : all_gus)
    {
      ggo::rect_int clipping = from_continuous_to_pixel_exclusive(gus._rect.data());

      fill_head1<heads_bkgd.format()>(heads_bkgd.data(), heads_bkgd.width(), heads_bkgd.height(), heads_bkgd.line_byte_step(), clipping);
    }

    ggo::image_t<gray_format> mask(size());
    mask.fill(0x00);
    for (const auto & gus : all_gus)
    {
      ggo::pos2_f center = gus._rect.center();

      std::vector<ggo::static_paint_shape<triangle2d_f, uint8_t>> triangles;
      ggo::pos2_f p_prv = gus._head_points.back();
      for (auto p : gus._head_points)
      {
        ggo::static_paint_shape<triangle2d_f, uint8_t> shape({ center, p, p_prv }, 0xff);

        triangles.emplace_back(shape);
        p_prv = p;
      }
      ggo::paint<mask.format(), ggo::sampling_4x4>(mask.data(), mask.width(), mask.height(), mask.line_byte_step(), triangles);
    }

    ggo::apply_mask<format, gray_format>(buffer, line_byte_step(), mask.data(), mask.line_byte_step(), heads_bkgd.data(), heads_bkgd.line_byte_step(), size());
  }

  // Heads border.
  {
    for (const auto & gus : all_gus)
    {
      std::vector<ggo::static_paint_shape<capsule_f, ggo::rgb_8u>> capsules;

      ggo::pos2_f p_prv = gus._head_points.back();
      for (auto p : gus._head_points)
      {
        ggo::static_paint_shape<capsule_f, ggo::rgb_8u> shape({ p_prv, p, 0.5f * thickness }, ggo::black_8u());

        capsules.emplace_back(shape);
        p_prv = p;
      }
      ggo::paint<format, ggo::sampling_4x4>(buffer, width(), height(), line_byte_step(), capsules);
    }
  }

  // Eyes.
  {
    for (const auto & gus : all_gus)
    {
      auto shrink = [](const ggo::disc_f & disc, float shrink_factor)
      {
        return ggo::disc_f(disc.center(), disc.radius() - shrink_factor);
      };

      ggo::paint<format, ggo::sampling_4x4>(buffer, width(), height(), line_byte_step(), gus._eyes[0], ggo::black_8u());
      ggo::paint<format, ggo::sampling_4x4>(buffer, width(), height(), line_byte_step(), gus._eyes[1], ggo::black_8u());
      ggo::paint<format, ggo::sampling_4x4>(buffer, width(), height(), line_byte_step(), shrink(gus._eyes[0], thickness), ggo::white_8u());
      ggo::paint<format, ggo::sampling_4x4>(buffer, width(), height(), line_byte_step(), shrink(gus._eyes[1], thickness), ggo::white_8u());
      ggo::paint<format, ggo::sampling_4x4>(buffer, width(), height(), line_byte_step(), gus._pupils[0], ggo::black_8u());
      ggo::paint<format, ggo::sampling_4x4>(buffer, width(), height(), line_byte_step(), gus._pupils[1], ggo::black_8u());
    }
  }

  // Mounth.
  {
    ggo::image_t<gray_format> outter_mask(size());
    outter_mask.fill(0x00);

    ggo::image_t<gray_format> inner_mask(size());
    inner_mask.fill(0x00);

    ggo::image_t<format> outter_layer(size());
    outter_layer.fill(ggo::black_8u());

    ggo::image_t<format> inner_layer(size());
    inner_layer.fill({ 0xa0, 0x00, 0x00 });

    for (const auto & gus : all_gus)
    {
      // The tongue.
      ggo::paint<format, ggo::sampling_4x4>(inner_layer.data(), inner_layer.width(), inner_layer.height(), inner_layer.line_byte_step(),
        ggo::disc_f(gus._mouth._center - ggo::vec2_f(0.f, 0.3f * gus._rect.height()), gus._mouth._tongue_radius), ggo::black_8u());

      ggo::paint<format, ggo::sampling_4x4>(inner_layer.data(), inner_layer.width(), inner_layer.height(), inner_layer.line_byte_step(),
        ggo::disc_f(gus._mouth._center - ggo::vec2_f(0.f, 0.3f * gus._rect.height()), gus._mouth._tongue_radius - thickness), ggo::rgb_8u(0xdd, 0x00, 0x00));

      // The masks.
      std::vector<ggo::static_paint_shape<disc_f, uint8_t>> discs_outter_mask;
      std::vector<ggo::static_paint_shape<disc_f, uint8_t>> discs_inner_mask;

      for (float angle = -pi_f / 2 - gus._mouth._dangle; angle <= -pi_f / 2 + gus._mouth._dangle; angle += 0.01f)
      {
        ggo::pos2_f cur_center = gus._mouth._center + gus._mouth._radius * ggo::vec2_f::from_angle(angle);

        ggo::static_paint_shape<disc_f, uint8_t> outter_disc(disc_f(cur_center, gus._mouth._outter_radius), 0xff);
        discs_outter_mask.push_back(outter_disc);

        ggo::static_paint_shape<disc_f, uint8_t> inner_disc(disc_f(cur_center, gus._mouth._outter_radius - thickness), 0xff);
        discs_inner_mask.push_back(inner_disc);
      }

      ggo::paint<gray_format, ggo::sampling_4x4>(outter_mask.data(), outter_mask.width(), outter_mask.height(), outter_mask.line_byte_step(), discs_outter_mask);
      ggo::paint<gray_format, ggo::sampling_4x4>(inner_mask.data(), inner_mask.width(), inner_mask.height(), inner_mask.line_byte_step(), discs_inner_mask);
    }

    apply_mask<format, gray_format>(buffer, line_byte_step(), outter_mask.data(), outter_mask.line_byte_step(), outter_layer.data(), outter_layer.line_byte_step(), size());
    apply_mask<format, gray_format>(buffer, line_byte_step(), inner_mask.data(), inner_mask.line_byte_step(), inner_layer.data(), inner_layer.line_byte_step(), size());
  }

  // The nose.
  {
    for (const auto & gus : all_gus)
    {
      std::vector<ggo::static_paint_shape<triangle2d_f, ggo::rgb_8u>> triangles;
      ggo::pos2_f p_prv = gus._nose._points.back();
      for (auto p : gus._nose._points)
      {
        ggo::static_paint_shape<triangle2d_f, ggo::rgb_8u> shape({ gus._nose._center, p, p_prv }, ggo::white_8u());

        triangles.emplace_back(shape);
        p_prv = p;
      }
      ggo::paint<format, ggo::sampling_4x4>(buffer, width(), height(), line_byte_step(), triangles);

      std::vector<ggo::static_paint_shape<capsule_f, ggo::rgb_8u>> capsules;
      p_prv = gus._nose._points.back();
      for (auto p : gus._nose._points)
      {
        ggo::static_paint_shape<capsule_f, ggo::rgb_8u> shape({ p, p_prv, 0.5f * thickness }, ggo::black_8u());

        capsules.emplace_back(shape);
        p_prv = p;
      }
      ggo::paint<format, ggo::sampling_4x4>(buffer, width(), height(), line_byte_step(), capsules);

    }
  }
#endif
}

//////////////////////////////////////////////////////////////
void ggo::polygus_bitmap_artist::render_bitmap(void * buffer, int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order) const
{
  if (pixel_type == ggo::pixel_type::bgrx_8u && memory_lines_order == ggo::lines_order::down)
  {
    render_polygus<ggo::pixel_type::bgrx_8u, ggo::lines_order::down>(buffer);
  }
  else if (pixel_type == ggo::pixel_type::rgb_8u && memory_lines_order == ggo::lines_order::up)
  {
    render_polygus<ggo::pixel_type::rgb_8u, ggo::lines_order::up>(buffer);
  }
  else if (pixel_type == ggo::pixel_type::rgb_8u && memory_lines_order == ggo::lines_order::down)
  {
    render_polygus<ggo::pixel_type::rgb_8u, ggo::lines_order::down>(buffer);
  }
  else
  {
    GGO_FAIL();
  }
}