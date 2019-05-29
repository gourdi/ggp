#include "ggo_demeco_animation_artist.h"
#include <2d/ggo_blit.h>
#include <2d/processing/ggo_gaussian_blur.h>
#include <2d/paint/ggo_paint.h>
#include <2d/fill/ggo_fill.h>

namespace
{
  constexpr ggo::image_format render_format = ggo::rgba_8u_yd;
  constexpr ggo::image_format shadow_format = ggo::y_8u_yd;

  using render_format_traits = ggo::image_format_traits<render_format>;
  using shadow_format_traits = ggo::image_format_traits<shadow_format>;
}

//////////////////////////////////////////////////////////////
ggo::demeco_animation_artist::demeco_animation_artist(int width, int height, int line_step, ggo::image_format format)
:
animation_artist_abc(width, height, line_step, format),
_artist(width, height),
_render_image(width, height, render_format),
_shadow_image(width, height, shadow_format)
{
}

//////////////////////////////////////////////////////////////
void ggo::demeco_animation_artist::render_frame(void * buffer, int frame_index, float time_step, bool & finished)
{
  // Render demecos.
  finished = false;
  if (_artist.update() == true)
  {
    finished = true;
    return;
  }

  _artist.render_tile(_render_image.data(), _render_image.line_byte_step(), frame_index, ggo::rect_int::from_size(_render_image.size()));

  // Blit render buffer alpha channel into shadow buffer.
  ggo::fill_solid<shadow_format>(_shadow_image.data(), _shadow_image.width(), _shadow_image.height(), _shadow_image.line_byte_step(), 255);
  const int shadow_dx = min_size() / 100;
  const int shadow_dy = min_size() / 100;
  for (int y = shadow_dy; y < height(); ++y)
  {
    const void * src = _render_image.pixel_ptr(0, y);
    void * dst = _shadow_image.pixel_ptr(shadow_dx, y - shadow_dy);

    for (int x = shadow_dx; x < width(); ++x)
    {
      auto pixel = read_pixel<render_format>(src);
      write_pixel<shadow_format>(dst, 0xff - pixel.a());
      
      src = ggo::move_ptr<render_format_traits::pixel_byte_size>(src);
      dst = ggo::move_ptr<shadow_format_traits::pixel_byte_size>(dst);
    }
  }

  // Blur shadow.
  float stddev = min_size() / 100.f;
  gaussian_blur<shadow_format>(_shadow_image.data(), _shadow_image.size(), _shadow_image.line_byte_step(), stddev);
  
  // Blit shadow and render buffers into output buffer.
  blit<shadow_format, ggo::rgb_8u_yu>(_shadow_image.data(), _shadow_image.width(), _shadow_image.height(), _shadow_image.line_byte_step(),
    buffer, width(), height(), line_step(), 0, 0);

  blit<render_format, ggo::rgb_8u_yu>(_render_image.data(), _render_image.width(), _render_image.height(), _render_image.line_byte_step(),
    buffer, width(), height(), line_step(), 0, 0);
}
