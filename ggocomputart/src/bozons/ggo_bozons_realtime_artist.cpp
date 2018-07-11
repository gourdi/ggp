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
ggo::bozons_realtime_artist::bozons_realtime_artist(int width, int height, int line_step, ggo::image_format format)
:
ggo::realtime_artist_abc(width, height, line_step, format)
{
  _hue = ggo::rand<float>();
  _bkgd_color1 = from_hsv<ggo::color_8u>(_hue, ggo::rand<float>(), ggo::rand<float>());
  _bkgd_color2 = from_hsv<ggo::color_8u>(_hue, ggo::rand<float>(), ggo::rand<float>());
  _bkgd_color3 = from_hsv<ggo::color_8u>(_hue, ggo::rand<float>(), ggo::rand<float>());
  _bkgd_color4 = from_hsv<ggo::color_8u>(_hue, ggo::rand<float>(), ggo::rand<float>());

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

  new_bozon._prv_pos = get_center();
  new_bozon._cur_pos = get_center();

  new_bozon._color = from_hsv<ggo::color_8u>(_hue, ggo::rand<float>(), ggo::rand<float>());

  new_bozon._angle = ggo::rand<float>(0, 2 * ggo::pi<float>());
  new_bozon._dangle = ggo::rand<float>(-dangle_amp, dangle_amp);

  new_bozon._counter = ggo::rand<int>(200, 300);

  new_bozon._speed = ggo::rand<float>(0.01f, 0.02f) * min_size();

  new_bozon._radius = 0.0015f * min_size();

  _bozons.push_back(new_bozon);
}

//////////////////////////////////////////////////////////////
void ggo::bozons_realtime_artist::preprocess_frame(int frame_index)
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
      if (frame_index < 100 && ggo::rand<int>(0, 256) == 0)
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
      bozon_it->_cur_pos += ggo::from_polar(bozon_it->_angle, bozon_it->_speed);

      ++bozon_it;
    }
  }

  // Create new bozons.
  if (frame_index < 100 && ggo::rand<int>(0, 12) == 0)
  {
    create_bozon();
  }
}

//////////////////////////////////////////////////////////////
void ggo::bozons_realtime_artist::render_tile(void * buffer, int frame_index, const ggo::rect_int & clipping)
{
  switch (format())
  {
  case ggo::bgrx_8u_yd:
    render_tile_t<ggo::bgrx_8u_yd>(buffer, frame_index, clipping);
    break;
  case ggo::rgb_8u_yu:
    render_tile_t<ggo::rgb_8u_yu>(buffer, frame_index, clipping);
    break;
  case ggo::rgb_8u_yd:
    render_tile_t<ggo::rgb_8u_yd>(buffer, frame_index, clipping);
    break;
  default:
    GGO_FAIL();
    break;
  }
}

//////////////////////////////////////////////////////////////
bool ggo::bozons_realtime_artist::finished(int frame_index) const
{
  return _bozons.empty();
}

