#include "ggo_duffing_realtime_artist.h"
#include <2d/paint/ggo_paint.h>
#include <2d/fill/ggo_fill.h>
#include <2d/paint/ggo_brush.h>
#include <2d/paint/ggo_blend.h>

//////////////////////////////////////////////////////////////
ggo::duffing_realtime_artist::duffing_realtime_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, ggo::ratio fps)
:
realtime_artist(width, height, line_byte_step, pixel_type, memory_lines_order),
_substeps_processing(2500 / fps)
{
  _radius = 0.01f * min_size();
  uint8_t gray = ggo::rand<uint8_t>(0x80, 0xff);
  _bkgd_color = ggo::rgb_8u(gray, gray, gray);
  _hue = ggo::rand<float>();
  _angle_offset = ggo::rand<float>(0.f, 2.f * ggo::pi<float>());
}

//////////////////////////////////////////////////////////////
void ggo::duffing_realtime_artist::preprocess_frame(void * buffer, uint32_t cursor_events, ggo::pos2_i cursor_pos)
{
  // Fill background.
  if (_substeps_processing._substeps_count == 0)
  {
    ggo::image_t<ggo::pixel_type::bgrx_8u, ggo::lines_order::down> img(buffer, size(), line_byte_step());

    ggo::fill_solid(img, _bkgd_color);
  }

  _paint_color = ggo::from_hsv<ggo::rgb_8u>(_hue, 1.0f, 0.75f);
  _hue += 0.0005f;

  _points.clear();

  _substeps_processing.call([&]
  {
    auto point = _duffing.update(0.01f);
    point = ggo::rotate(point, _angle_offset);
    point = map_fit(point, -1.7f, 1.7f);

    _points.push_back(point);

    _angle_offset = std::fmod(_angle_offset + 0.0001f, 2 * ggo::pi<float>());
  });
}

//////////////////////////////////////////////////////////////
void ggo::duffing_realtime_artist::render_tile(void * buffer, const ggo::rect_int & clipping)
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

  ggo::image_t<ggo::pixel_type::bgrx_8u, ggo::lines_order::down> img(buffer, size(), line_byte_step());

  // Fade out.
  for_each_pixel(img, clipping, [&](int x, int y)
  {
    const ggo::rgb_8u pixel = img.read_pixel(x, y);

    const int32_t diff_r = contract(pixel.r() - _bkgd_color.r());
    const int32_t diff_g = contract(pixel.g() - _bkgd_color.g());
    const int32_t diff_b = contract(pixel.b() - _bkgd_color.b());

    img.write_pixel(x, y, {
      static_cast<uint8_t>(_bkgd_color.r() + diff_r),
      static_cast<uint8_t>(_bkgd_color.g() + diff_g),
      static_cast<uint8_t>(_bkgd_color.b() + diff_b) });
  });

  // Paint the Duffing's points.
  const auto brush = ggo::make_solid_brush(_paint_color);
  const auto blender = ggo::alpha_blender_rgb8u(0.1f);

  for (const auto & point : _points)
  {
    ggo::paint<ggo::sampling_4x4>(img, ggo::disc_f(point, _radius), brush, blender, clipping);
  }
}

//////////////////////////////////////////////////////////////
bool ggo::duffing_realtime_artist::finished()
{
  return _substeps_processing._substeps_count > 30000;
}


