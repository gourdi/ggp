#include "ggo_neon_artist.h"
#include <ggo_color.h>
#include <ggo_buffer_paint.h>
#include <ggo_buffer_fill.h>
#include <ggo_brush.h>
#include <ggo_blender.h>

namespace
{
  //////////////////////////////////////////////////////////////
  template <ggo::pixel_buffer_format pbf>
  void paint_point_t(const ggo::neon_artist & artist, void * buffer, const ggo::pos2f & point_pos, const ggo::color_8u & color)
  {
    const float radius = 0.01f * artist.get_min_size();

    ggo::pos2f pos = artist.map_fit(0.8f * point_pos, -1, 1);

    ggo::disc_float disc1(pos, radius);
    ggo::paint_shape<pbf, ggo::sampling_4x4>(
      buffer, artist.get_width(), artist.get_height(), artist.get_line_step(),
      disc1, ggo::make_solid_brush(color), ggo::additive_blender<ggo::color_8u>());

    pos.x() = artist.get_width() - pos.x() - 1;

    ggo::disc_float disc2(pos, radius);
    ggo::paint_shape<pbf, ggo::sampling_4x4>(
      buffer, artist.get_width(), artist.get_height(), artist.get_line_step(),
      disc2, ggo::make_solid_brush(color), ggo::additive_blender<ggo::color_8u>());
  }
}

//////////////////////////////////////////////////////////////
ggo::neon_artist::neon_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt)
:
accumulation_animation_artist_abc(width, height, line_step, pbf, rt)
{

}

//////////////////////////////////////////////////////////////
void ggo::neon_artist::init_sub()
{
	_angle 				    = 0;
	_radius_prv			  = ggo::rand<float>();
	_radius_cur			  = _radius_prv;
	_radius_attractor	= ggo::rand<float>();
	_angle				    = ggo::rand<float>(0, 2 * ggo::pi<float>());
	_hue			        = ggo::rand<float>();
}

//////////////////////////////////////////////////////////////
void ggo::neon_artist::init_output_buffer(void * buffer) const
{
  switch (get_pixel_buffer_format())
  {
  case ggo::rgb_8u_yu:
    ggo::fill_solid<ggo::rgb_8u_yu>(buffer, get_width(), get_height(), get_line_step(), ggo::black<ggo::color_8u>());
    break;
  case ggo::bgra_8u_yd:
    ggo::fill_solid<ggo::bgra_8u_yd>(buffer, get_width(), get_height(), get_line_step(), ggo::black<ggo::color_8u>());
    break;
  }
}

//////////////////////////////////////////////////////////////
bool ggo::neon_artist::render_next_frame_acc(void * buffer, int frame_index)
{
  const int frames_count = 2000;

	if (frame_index >= frames_count)
	{
		return false;
	}

  if ((frame_index % 100) == 0)
  {
    _radius_attractor = ggo::rand<float>(0.2f, 1);
    _hue = ggo::rand<float>();
  }
	
  for (int substep = 0; substep < 10; ++substep)
  {
    const float velocity = _radius_cur - _radius_prv;
    const float force = 0.0000075f * ggo::sign(_radius_attractor - _radius_cur);
    _radius_prv = _radius_cur;
    _radius_cur += velocity + force;

    _angle += 0.001f;

    ggo::pos2f pos;

    pos.x() = _radius_cur * cos(_angle);
    pos.y() = _radius_cur * sin(_angle);
    paint_point(buffer, pos, ggo::color_8u(uint8_t(0x08), uint8_t(0x08), uint8_t(0x08)));

    pos.x() = _radius_attractor * std::cos(_angle);
    pos.y() = _radius_attractor * std::sin(_angle);
    if (substep & 1)
    {
      paint_point(buffer, pos, ggo::from_hsv<ggo::color_8u>(_hue, 1.0f, 8.f / 255.f));
    }
  }
	
	return true;
}

//////////////////////////////////////////////////////////////
void ggo::neon_artist::paint_point(void * buffer, const ggo::pos2f & point_pos, const ggo::color_8u & color) const
{
  switch (get_pixel_buffer_format())
  {
  case ggo::rgb_8u_yu:
    paint_point_t<ggo::rgb_8u_yu>(*this, buffer, point_pos, color);
    break;
  case ggo::bgra_8u_yd:
    paint_point_t<ggo::bgra_8u_yd>(*this, buffer, point_pos, color);
    break;
  }
}

