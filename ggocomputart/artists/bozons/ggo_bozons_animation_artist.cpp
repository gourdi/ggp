#include "ggo_bozons_animation_artist.h"
#include <ggo_buffer_fill.h>
#include <ggo_buffer_paint.h>
#include <ggo_brush.h>
#include <ggo_blend.h>
#include <ggo_multi_shape_paint.h>

namespace
{
  const float dangle_amp = 0.025f * ggo::pi<float>();
}

//////////////////////////////////////////////////////////////
ggo::bozons_animation_artist::bozons_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt)
:
ggo::animation_artist_abc(width, height, line_step, pbf, rt)
{
	
}

//////////////////////////////////////////////////////////////
void ggo::bozons_animation_artist::init()
{
  _frame_index = -1;

  _hue = ggo::rand<float>();
  _bkgd_color1 = from_hsv<ggo::color_8u>(_hue, ggo::rand<float>(), ggo::rand<float>());
  _bkgd_color2 = from_hsv<ggo::color_8u>(_hue, ggo::rand<float>(), ggo::rand<float>());
  _bkgd_color3 = from_hsv<ggo::color_8u>(_hue, ggo::rand<float>(), ggo::rand<float>());
  _bkgd_color4 = from_hsv<ggo::color_8u>(_hue, ggo::rand<float>(), ggo::rand<float>());

  // Create bozons.
  _bozons.clear();

  for (int i = 0; i < 32; ++i)
  {
    bozon new_bozon;
    new_bozon._prv_pos = get_center();
    new_bozon._cur_pos = get_center();

    new_bozon._color = from_hsv<ggo::color_8u>(_hue, ggo::rand<float>(), ggo::rand<float>());

    new_bozon._angle = ggo::rand<float>(0, 2 * ggo::pi<float>());
    new_bozon._dangle = ggo::rand<float>(-dangle_amp, dangle_amp);

    new_bozon._counter = ggo::rand<int>(200, 300);

    new_bozon._speed = ggo::rand<float>(0.01f, 0.02f) * get_min_size();

    _bozons.push_back(new_bozon);
  }
}

//////////////////////////////////////////////////////////////
bool ggo::bozons_animation_artist::update()
{
  ++_frame_index;

  if (_bozons.empty() == true)
  {
    return false;
  }

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
      if (_frame_index < 100 && ggo::rand<int>(0, 256) == 0)
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
        new_bozon._color = bozon_it->_color;
        new_bozon._counter = bozon_it->_counter;

        _bozons.push_back(new_bozon);
      }

      // Update current bozon.
      bozon_it->_angle += bozon_it->_dangle;
      bozon_it->_dangle *= 1.01f;
      bozon_it->_speed *= 0.99f;
      bozon_it->_prv_pos = bozon_it->_cur_pos;
      bozon_it->_cur_pos += ggo::from_polar(bozon_it->_angle, bozon_it->_speed);

      ++bozon_it;
    }
  }

  // Create new bozons.
  if (_frame_index < 100 && ggo::rand<int>(0, 12) == 0)
  {
    bozon new_bozon;
    new_bozon._prv_pos = get_center();
    new_bozon._cur_pos = get_center();

    new_bozon._color = from_hsv<ggo::color_8u>(_hue, ggo::rand<float>(), ggo::rand<float>());

    new_bozon._angle = ggo::rand<float>(0, 2 * ggo::pi<float>());
    new_bozon._dangle = ggo::rand<float>(-dangle_amp, dangle_amp);

    new_bozon._counter = ggo::rand<int>(200, 300);

    new_bozon._speed = ggo::rand<float>(0.01f, 0.02f) * get_min_size();

    _bozons.push_back(new_bozon);
  }

  return true;
}

//////////////////////////////////////////////////////////////
void ggo::bozons_animation_artist::render_frame(void * buffer, const ggo::pixel_rect & clipping)
{
  if (_frame_index == 0)
  {
    ggo::fill_4_colors<ggo::bgra_8u_yd>(buffer, get_width(), get_height(), get_line_step(),
      _bkgd_color1, _bkgd_color2, _bkgd_color3, _bkgd_color4, clipping);
  }

  const float radius = 0.001f * get_min_size();

  switch (get_rendering_type())
  {
  case rendering_type::realtime_rendering_android:
  case rendering_type::realtime_rendering_pc:
    for (const auto & bozon : _bozons)
    {
      ggo::paint_shape<ggo::bgra_8u_yd, ggo::sampling_4x4>(buffer, get_width(), get_height(), get_line_step(),
        ggo::extended_segment_float(bozon._prv_pos, bozon._cur_pos, radius),
        ggo::solid_brush<ggo::color_8u>(bozon._color), ggo::overwrite_blender<color_8u>(), clipping);
    }
    break;
  default:
    GGO_FAIL();
    break;
  }
}

