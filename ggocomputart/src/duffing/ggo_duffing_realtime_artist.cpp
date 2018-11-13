#include "ggo_duffing_realtime_artist.h"
#include <2d/paint/ggo_paint.h>
#include <2d/fill/ggo_fill.h>
#include <2d/paint/ggo_brush.h>
#include <2d/paint/ggo_blend.h>

//////////////////////////////////////////////////////////////
ggo::duffing_realtime_artist::duffing_realtime_artist(int width, int height, int line_step, ggo::image_format format)
:
fixed_frames_count_realtime_artist_abc(width, height, line_step, format)
{
  _radius = 0.01f * min_size();
  uint8_t gray = ggo::rand<uint8_t>(0x80, 0xff);
  _bkgd_color = ggo::rgb_8u(gray, gray, gray);
  _hue = ggo::rand<float>();
  _angle_offset = ggo::rand<float>(0.f, 2.f * ggo::pi<float>());
}

//////////////////////////////////////////////////////////////
void ggo::duffing_realtime_artist::preprocess_frame(int frame_index, uint32_t cursor_events, ggo::pos2i cursor_pos)
{
  _paint_color = ggo::from_hsv<ggo::rgb_8u>(_hue, 1.0f, 0.75f);
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
}

//////////////////////////////////////////////////////////////
void ggo::duffing_realtime_artist::render_tile(void * buffer, int frame_index, const ggo::rect_int & clipping)
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

  using format_traits = ggo::image_format_traits<ggo::bgrx_8u_yd>;

  // Fill background.
  if (frame_index == 0)
  {
    ggo::fill_solid<ggo::bgrx_8u_yd>(buffer, width(), height(), line_step(), _bkgd_color, clipping);
  }

  // Fade out (process 1 frame out of 2).
  if (ggo::is_even(frame_index) == true)
  {
    for (int y = clipping.bottom(); y <= clipping.top(); ++y)
    {
      void * ptr = ggo::get_pixel_ptr<ggo::bgrx_8u_yd>(buffer, clipping.left(), y, height(), line_step());
      void * last_ptr = ggo::get_pixel_ptr<ggo::bgrx_8u_yd>(buffer, clipping.right(), y, height(), line_step());

      for (; ptr <= last_ptr; ptr = ggo::ptr_offset<format_traits::pixel_byte_size>(ptr))
      {
        const ggo::rgb_8u pixel = ggo::read_pixel<ggo::bgrx_8u_yd>(ptr);

        const int32_t diff_r = contract(pixel._r - _bkgd_color._r);
        const int32_t diff_g = contract(pixel._g - _bkgd_color._g);
        const int32_t diff_b = contract(pixel._b - _bkgd_color._b);

        ggo::write_pixel<ggo::bgrx_8u_yd>(ptr, {
          static_cast<uint8_t>(_bkgd_color._r + diff_r),
          static_cast<uint8_t>(_bkgd_color._g + diff_g),
          static_cast<uint8_t>(_bkgd_color._b + diff_b) });
      }
    }
  }

  // Paint the Duffing's points.
  for (const auto & point : _points)
  {
    ggo::paint<ggo::bgrx_8u_yd, ggo::sampling_4x4>(buffer, width(), height(), line_step(),
      ggo::disc_float(point, _radius), brush, blender, clipping);
  }
}

