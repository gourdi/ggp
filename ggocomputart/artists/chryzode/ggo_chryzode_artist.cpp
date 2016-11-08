#include "ggo_chryzode_artist.h"
#include <ggo_gaussian_blur2d.h>
#include <ggo_buffer_paint.h>

//////////////////////////////////////////////////////////////
ggo::chryzode_artist::chryzode_artist(int render_width, int render_height)
:
artist(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo::chryzode_artist::render_chryzode(void * buffer, float radius, const chryzode_params & params, float hue_start, float hue_end) const
{
  // Float buffer.
  std::vector<float> buffer_32f(get_render_width() * get_render_height(), 0.f);

  // Render the chryzode.
  for (int modulo = params._modulo_start; modulo < params._modulo_end; ++modulo)
  {
    int value = 1;
    for (int value = 1; value <= modulo; ++value)
    {
      float result = (value + params._offset1) * params._multiplier1 + (value + params._offset2) * params._multiplier2;
      
      float angle1 = value * 2 * ggo::pi<float>() / modulo;
      float angle2 = result * 2 * ggo::pi<float>() / modulo;

      // Reorient the figure.
      angle1 += ggo::pi<float>() / 2;
      angle2 += ggo::pi<float>() / 2;
      
      ggo::pos2f p1 = get_center() + ggo::from_polar(angle1, radius);
      ggo::pos2f p2 = get_center() + ggo::from_polar(angle2, radius);
      
      ggo::pos2f middle = (p1 + p2) / 2.f;
      ggo::vec2f diff = p2 - p1;
      p1 = middle + 1000.f * diff;
      p2 = middle - 1000.f * diff;

      // Paint the segment.
      auto brush = [](int x, int y) { return 1.f; };
      auto blend = [](int x, int y, float bkgd_color, float brush_color) { return bkgd_color + brush_color; };
      ggo::paint_shape<y_32f_yu, sampling_2x2>(buffer_32f.data(), get_render_width(), get_render_height(), 3 * get_render_width(),
        ggo::extended_segment_float(p1, p2, 0.005f * radius), brush, blend);
    }
  }
  
  // Convert from gray to rgb.
  uint8_t * buffer_8u = static_cast<uint8_t *>(buffer);
  for (int i = get_render_width() * get_render_height() - 1; i >= 0; --i)
  {
    float coef = buffer_32f[i] / 512; // Normalize.
    float hue = ggo::map<float>(coef, 0, 1, hue_start, hue_end);
    const ggo::color_8u color = from_hsv<ggo::color_8u>(hue, 1 - coef, coef);
    buffer_8u[3 * i + 0] = color.r();
    buffer_8u[3 * i + 1] = color.g();
    buffer_8u[3 * i + 2] = color.b();
  }
}
