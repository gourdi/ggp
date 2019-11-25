#include "ggo_bozons_realtime_artist.h"
#include <2d/fill/ggo_fill.h>
#include <2d/paint/ggo_paint.h>
#include <2d/paint/ggo_brush.h>
#include <2d/paint/ggo_blend.h>

namespace
{
  const float dangle_amp = 0.025f * ggo::pi<float>();
}

//////////////////////////////////////////////////////////////
ggo::bozons_realtime_artist::bozons_realtime_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, ggo::ratio fps)
:
ggo::realtime_artist(width, height, line_byte_step, pixel_type, memory_lines_order)
{
  _substeps_per_frame = 40.f * static_cast<float>(fps._den) / static_cast<float>(fps._num);

  _hue = ggo::rand<float>();
  _bkgd_color1 = from_hsv<ggo::rgb_8u>(_hue, ggo::rand<float>(), ggo::rand<float>());
  _bkgd_color2 = from_hsv<ggo::rgb_8u>(_hue, ggo::rand<float>(), ggo::rand<float>());
  _bkgd_color3 = from_hsv<ggo::rgb_8u>(_hue, ggo::rand<float>(), ggo::rand<float>());
  _bkgd_color4 = from_hsv<ggo::rgb_8u>(_hue, ggo::rand<float>(), ggo::rand<float>());

  // Create bozons.
  for (int i = 0; i < 32; ++i)
  {
    create_bozon();
  }
}

//////////////////////////////////////////////////////////////
void ggo::bozons_realtime_artist::create_bozon()
{
  bozon new_bozon;

  new_bozon._prv_pos = center();
  new_bozon._cur_pos = center();

  new_bozon._color = from_hsv<ggo::rgb_8u>(_hue, ggo::rand<float>(), ggo::rand<float>());

  new_bozon._angle = ggo::rand<float>(0, 2 * ggo::pi<float>());
  new_bozon._dangle = ggo::rand<float>(-dangle_amp, dangle_amp);

  new_bozon._counter = ggo::rand<int>(200, 300);

  new_bozon._speed = ggo::rand<float>(0.01f, 0.02f) * min_size();

  new_bozon._radius = 0.0015f * min_size();

  _bozons.push_back(new_bozon);
}

//////////////////////////////////////////////////////////////
void ggo::bozons_realtime_artist::preprocess_frame(void * buffer, uint32_t cursor_events, ggo::pos2_i cursor_pos)
{
  if (_substeps_count == 0)
  {
    if (pixel_type() == ggo::pixel_type::bgrx_8u && memory_lines_order() == ggo::lines_order::down)
    {
      render_background_t<ggo::pixel_type::bgrx_8u, ggo::lines_order::down>(buffer);
    }
    else if (pixel_type() == ggo::pixel_type::rgb_8u && memory_lines_order() == ggo::lines_order::up)
    {
      render_background_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up>(buffer);
    }
    else if (pixel_type() == ggo::pixel_type::rgb_8u && memory_lines_order() == ggo::lines_order::down)
    {
      render_background_t<ggo::pixel_type::rgb_8u, ggo::lines_order::down>(buffer);
    }
    else
    {
      GGO_FAIL();
    }
  }

  _capsules.clear();

  for (_substeps += _substeps_per_frame; _substeps >= 1.f; _substeps -= 1.f, ++_substeps_count)
  {
    for (auto bozon_it = _bozons.begin(); bozon_it != _bozons.end(); /* Do NOT increment iterator here since we erase bozons inside the loop. */)
    {
      --bozon_it->_counter;
      if (bozon_it->_counter <= 0)
      {
        bozon_it = _bozons.erase(bozon_it);
      }
      else
      {
        // Split bozon?
        if (_substeps_count < 100 && ggo::rand<int>(0, 256) == 0)
        {
          bozon new_bozon;

          const float angle_offset = ggo::rand<float>(0.f, ggo::pi<float>() / 4.f);

          new_bozon._angle = bozon_it->_angle + angle_offset;
          bozon_it->_angle -= angle_offset;
          new_bozon._dangle = -bozon_it->_dangle; // Change direction too.

                                                  // Since this new bozon will be updated at the end of the current loop, 
                                                  // there is no need to move it now.
          new_bozon._prv_pos = bozon_it->_cur_pos;
          new_bozon._cur_pos = bozon_it->_cur_pos;
          new_bozon._speed = bozon_it->_speed;
          new_bozon._radius = bozon_it->_radius;
          new_bozon._color = bozon_it->_color;
          new_bozon._counter = bozon_it->_counter;

          _bozons.push_back(new_bozon);
        }

        // Update current bozon.
        bozon_it->_angle += bozon_it->_dangle;
        bozon_it->_dangle *= 1.01f;
        bozon_it->_speed *= 0.99f;
        bozon_it->_radius *= 0.995f;
        bozon_it->_prv_pos = bozon_it->_cur_pos;
        bozon_it->_cur_pos += bozon_it->_speed * ggo::vec2_f::from_angle(bozon_it->_angle);

        // Store it as to be painted.
        _capsules.emplace_back(bozon_it->_prv_pos, bozon_it->_cur_pos, bozon_it->_radius, bozon_it->_color);

        ++bozon_it;
      }
    }

    // Create new bozons.
    if (_substeps_count < 100 && ggo::rand<int>(0, 12) == 0)
    {
      create_bozon();
    }
  }
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
void ggo::bozons_realtime_artist::render_background_t(void * buffer) const
{
  ggo::image_t<pixel_type, memory_lines_order> image(buffer, size(), line_byte_step());

  fill_4_colors(image, _bkgd_color1, _bkgd_color2, _bkgd_color3, _bkgd_color4);
}

//////////////////////////////////////////////////////////////
void ggo::bozons_realtime_artist::render_tile(void * buffer, const ggo::rect_int & clipping)
{
  if (pixel_type() == ggo::pixel_type::bgrx_8u && memory_lines_order() == ggo::lines_order::down)
  {
    render_tile_t<ggo::pixel_type::bgrx_8u, ggo::lines_order::down>(buffer, clipping);
  }
  else if (pixel_type() == ggo::pixel_type::rgb_8u && memory_lines_order() == ggo::lines_order::up)
  {
    render_tile_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up>(buffer, clipping);
  }
  else if (pixel_type() == ggo::pixel_type::rgb_8u && memory_lines_order() == ggo::lines_order::down)
  {
    render_tile_t<ggo::pixel_type::rgb_8u, ggo::lines_order::down>(buffer, clipping);
  }
  else
  {
    GGO_FAIL();
  }
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
void ggo::bozons_realtime_artist::render_tile_t(void * buffer, const ggo::rect_int & clipping) const
{
  ggo::image_t<pixel_type, memory_lines_order> image(buffer, size(), line_byte_step());

  for (const auto & capsule : _capsules)
  {
    ggo::paint<ggo::sampling_4x4>(image,
      ggo::capsule_f(capsule._p1, capsule._p2, capsule._radius),
      ggo::solid_color_brush<ggo::rgb_8u>(capsule._color), ggo::overwrite_blender<rgb_8u>(), clipping);
  }
}

//////////////////////////////////////////////////////////////
bool ggo::bozons_realtime_artist::finished()
{
  return _bozons.empty();
}


