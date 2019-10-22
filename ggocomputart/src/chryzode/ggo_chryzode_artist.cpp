#include "ggo_chryzode_artist.h"
#include <2d/processing/ggo_gaussian_blur.h>
#include <2d/paint/ggo_paint.h>
#include <2d/fill/ggo_fill.h>

//////////////////////////////////////////////////////////////
ggo::chryzode_artist::chryzode_artist(int width, int height, int line_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order)
:
artist(width, height, line_step, pixel_type, memory_lines_order)
{
}

//////////////////////////////////////////////////////////////
void ggo::chryzode_artist::render_chryzode(void * buffer, float radius, const chryzode_params & params, float hue_start, float hue_end) const
{
  // Float buffer.
  ggo::image_t<ggo::pixel_type::y_32f, ggo::lines_order::up> img_32f(size());
  fill_black(img_32f);

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
      
      ggo::pos2_f p1 = get_center() + radius * ggo::vec2_f::from_angle(angle1);
      ggo::pos2_f p2 = get_center() + radius * ggo::vec2_f::from_angle(angle2);
      
      ggo::pos2_f middle = (p1 + p2) / 2.f;
      ggo::vec2_f diff = p2 - p1;
      p1 = middle + 1000.f * diff;
      p2 = middle - 1000.f * diff;

      // Paint the segment.
      ggo::paint<sampling_2x2>(img_32f,
        ggo::capsule_f(p1, p2, 0.005f * radius),
        ggo::solid_color_brush<float>(1.f), ggo::add_blender<float>());
    }
  }
  
  // Convert from gray to rgb.
  uint8_t * buffer_8u = static_cast<uint8_t *>(buffer);
  const float * buffer_32f = static_cast<const float *>(img_32f.data());
  for (int i = width() * height() - 1; i >= 0; --i)
  {
    float coef = buffer_32f[i] / 512; // Normalize.
    float hue = ggo::map<float>(coef, 0, 1, hue_start, hue_end);
    const ggo::rgb_8u color = from_hsv<ggo::rgb_8u>(hue, 1 - coef, coef);
    buffer_8u[3 * i + 0] = color.r();
    buffer_8u[3 * i + 1] = color.g();
    buffer_8u[3 * i + 2] = color.b();
  }
}
