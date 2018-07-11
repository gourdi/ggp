#include "ggo_demeco_animation_artist.h"
#include <2d/ggo_blit.h>
#include <2d/blur/ggo_gaussian_blur.h>
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
_render_image(width, height, render_format),
_shadow_image(width, height, shadow_format)
{
  const int steps = ggo::rand<int>(3, 5);
  const float main_radius = ggo::rand<float>(0.4f, 0.5f) * min_size();
  const float delta = 2 * main_radius / float(2 * steps + 1);

  for (int y = -steps; y <= steps; ++y)
  {
    for (int x = -steps; x <= steps; ++x)
    {
      float x_f = float(x) * delta;
      float y_f = float(y) * delta;
      float radius = ggo::distance(x_f, y_f);

      bool create = true;
      if (radius > main_radius)
      {
        create = false;
      }
      else if (radius > main_radius / 2)
      {
        create = ggo::rand<float>() > ggo::ease_inout((radius - main_radius / 2) / (main_radius / 2));
      }

      if (create == true)
      {
        auto demeco = std::make_unique<toto>(get_center() + pos2f(x_f, y_f), 0.5f * delta, ggo::rand<int>(-25, 0));

        _demecos.emplace_back(std::move(demeco));
      }
    }
  }
}

//////////////////////////////////////////////////////////////
void ggo::demeco_animation_artist::render_frame(void * buffer, int frame_index, bool & finished)
{
  finished = false;
  if (_demecos.empty() == true && frame_index >= 1)
  {
    finished = true;
    return;
  }

  if (frame_index == 0)
  {
    ggo::fill_solid<ggo::rgba_8u_yd>(_render_image.data(), _render_image.width(), _render_image.height(), _render_image.line_byte_step(), ggo::alpha_color_8u(0, 0, 0, 0));
  }

  // Render the demecos.
  if (_demecos.empty() == false)
  {
    size_t i = _demecos.size() - 1;
    while (true)
    {
      const auto & demeco = _demecos[i];

      if (demeco->_counter >= 0 && demeco->render(_render_image) == false)
      {
        _demecos.erase(_demecos.begin() + i);
      }
      else
      {
        demeco->_counter++;
      }

      if (i == 0)
      {
        break;
      }
      --i;
    }
  }

  // Blit render buffer into shadow buffer.
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
      
      src = ggo::ptr_offset<render_format_traits::pixel_byte_size>(src);
      dst = ggo::ptr_offset<shadow_format_traits::pixel_byte_size>(dst);
    }
  }

  // Blur shadow.
  float stddev = min_size() / 100.f;
  gaussian_blur2d<shadow_format>(_shadow_image.data(), _shadow_image.line_byte_step(), _shadow_image.size(), stddev);
  
  // Blit shadow and render buffers into output buffer.
  blit<shadow_format, ggo::rgb_8u_yu>(_shadow_image.data(), _shadow_image.width(), _shadow_image.height(), _shadow_image.line_byte_step(),
    buffer, width(), height(), line_step(), 0, 0);

  blit<render_format, ggo::rgb_8u_yu>(_render_image.data(), _render_image.width(), _render_image.height(), _render_image.line_byte_step(),
    buffer, width(), height(), line_step(), 0, 0);
}

//////////////////////////////////////////////////////////////
bool ggo::demeco_animation_artist::toto::render(ggo::image & image)
{
  ggo::paint_shape<render_format, ggo::sampling_8x8>(image.data(), image.width(), image.height(), image.line_byte_step(), ggo::disc_float(_pos, _radius), { 255, 0, 0, 255 });

  return false;
}
