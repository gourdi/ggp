#include "ggo_duffing_realtime_animation_artist.h"
#include <ggo_buffer_paint.h>
#include <ggo_buffer_fill.h>
#include <ggo_brush.h>
#include <ggo_blender.h>

//////////////////////////////////////////////////////////////
ggo::duffing_realtime_animation_artist::duffing_realtime_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt)
:
animation_artist_abc(width, height, line_step, pbf, rt)
{
  _radius = 0.01f * get_min_size();
}

//////////////////////////////////////////////////////////////
void ggo::duffing_realtime_animation_artist::init()
{
  _frame_index = -1;
  uint8_t gray = ggo::rand<uint8_t>(0x80, 0xff);
  _bkgd_color = ggo::color_8u(gray, gray, gray);
  _hue = ggo::rand<float>();
  _angle_offset = ggo::rand<float>(0.f, 2.f * ggo::pi<float>());
}

//////////////////////////////////////////////////////////////
bool ggo::duffing_realtime_animation_artist::update()
{
  ++_frame_index;

  if (_frame_index > 1500)
  {
    return false;
  }

  _paint_color = ggo::from_hsv<ggo::color_8u>(_hue, 1.0f, 0.75f);
  _hue += 0.0005f;

  for (auto & point : _points)
  {
    point = _duffing.update(0.01f);

    float angle = atan2(point.y(), point.x()) + _angle_offset;
    float dist = point.get_length();

    point.set(dist * std::cos(angle), dist * std::sin(angle));
    point = map_fit(point, -1.7f, 1.7f);

    _angle_offset = std::fmod(_angle_offset + 0.0001f, 2 * ggo::pi<float>());
  }

  return true;
}

//////////////////////////////////////////////////////////////
void ggo::duffing_realtime_animation_artist::render_frame(void * buffer, const ggo::pixel_rect & clipping) const
{
  auto contract = [](int v) -> int
  {
    if (v > 0)
    {
      return ggo::fixed_point_div<8>(uint32_t(v) * 0xf8) - 1;
    }
    else if (v < 0)
    {
      return -int32_t(ggo::fixed_point_div<8>(uint32_t(-v) * 0xf8)) + 1;
    }
    else
    {
      return 0;
    }
  };

  const auto brush = ggo::make_solid_brush(_paint_color);
  const auto blender = ggo::alpha_blender_rgb8u(0.1f);

  using format = ggo::pixel_buffer_format_info<ggo::bgra_8u_yd>;

  // Fill background.
  if (_frame_index == 0)
  {
    ggo::fill_solid<ggo::bgra_8u_yd>(buffer, get_width(), get_height(), get_line_step(), _bkgd_color, clipping);
  }

  // Fade out (process 1 frame out of 2).
  if (ggo::is_even(_frame_index) == true)
  {
    for (int y = clipping.bottom(); y <= clipping.top(); ++y)
    {
      void * ptr = ggo::get_pixel_ptr<ggo::bgra_8u_yd>(buffer, clipping.left(), y, get_height(), get_line_step());
      void * last_ptr = ggo::get_pixel_ptr<ggo::bgra_8u_yd>(buffer, clipping.right(), y, get_height(), get_line_step());

      for (; ptr <= last_ptr; ptr = ggo::ptr_offset<format::pixel_byte_size>(ptr))
      {
        const ggo::color_8u pixel = ggo::read_pixel<ggo::bgra_8u_yd>(ptr);

        const int32_t diff_r = contract(pixel.r() - _bkgd_color.r());
        const int32_t diff_g = contract(pixel.g() - _bkgd_color.g());
        const int32_t diff_b = contract(pixel.b() - _bkgd_color.b());

        ggo::write_pixel<ggo::bgra_8u_yd>(ptr, {
          static_cast<uint8_t>(_bkgd_color.r() + diff_r),
          static_cast<uint8_t>(_bkgd_color.g() + diff_g),
          static_cast<uint8_t>(_bkgd_color.b() + diff_b) });
      }
    }
  }

  // Paint the Duffing's points.
  for (const auto & point : _points)
  {
    ggo::paint_shape<ggo::bgra_8u_yd, ggo::sampling_4x4>(buffer, get_width(), get_height(), get_line_step(),
      ggo::disc_float(point, _radius), brush, blender, clipping);
  }
}
