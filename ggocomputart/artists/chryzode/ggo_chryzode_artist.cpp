#include "ggo_chryzode_artist.h"
#include <ggo_gaussian_blur.h>
#include <ggo_paint.h>
#include <ggo_layer.h>
#include <ggo_gray_image_buffer.h>

//////////////////////////////////////////////////////////////
ggo_chryzode_artist::ggo_chryzode_artist(int render_width, int render_height)
:
ggo_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo_chryzode_artist::render_chryzode(uint8_t * buffer, float radius, const ggo_chryzode_params & params, float hue_start, float hue_end) const
{
  // Float buffer.
  ggo::gray_image_buffer_float image_data(get_render_width(), get_render_height());
  image_data.fill(0);

  // Render the chryzode.
  for (int modulo = params._modulo_start; modulo < params._modulo_end; ++modulo)
  {
    int value = 1;
    for (int value = 1; value <= modulo; ++value)
    {
      float result = (value + params._offset1) * params._multiplier1 + (value + params._offset2) * params._multiplier2;
      
      float angle1 = value * 2 * ggo::PI<float>() / modulo;
      float angle2 = result * 2 * ggo::PI<float>() / modulo;

      // Reorient the figure.
      angle1 += ggo::PI<float>() / 2;
      angle2 += ggo::PI<float>() / 2;
      
      ggo::pos2f p1 = get_center() + ggo::from_polar(angle1, radius);
      ggo::pos2f p2 = get_center() + ggo::from_polar(angle2, radius);
      
      ggo::pos2f middle = (p1 + p2) / 2.f;
      ggo::vec2f diff = p2 - p1;
      p1 = middle + 1000.f * diff;
      p2 = middle - 1000.f * diff;

      // Paint the segment.
      ggo::paint(image_data,
                 std::make_shared<ggo::extended_segment_float>(p1, p2, 0.005f * radius),
                 1, 1, std::make_shared<ggo::gray_additive_blender>(),
                 ggo::pixel_sampler_2X2());
    }
  }
  
  // Convert from gray to rgb.
  for (int i = get_render_width() * get_render_height() - 1; i >= 0; --i)
  {
    float coef = image_data[i] / 512; // Normalize.
    float hue = ggo::map<float>(coef, 0, 1, hue_start, hue_end);
    ggo::color color = ggo::color::from_hsv(hue, 1 - coef, coef);
    buffer[3 * i + 0] = color.r8();
    buffer[3 * i + 1] = color.g8();
    buffer[3 * i + 2] = color.b8();
  }
}
