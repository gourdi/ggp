#include "ggo_chryzode_artist.h"
#include <2d/blur/ggo_gaussian_blur.h>
#include <2d/paint/ggo_paint.h>

//////////////////////////////////////////////////////////////
ggo::chryzode_artist::chryzode_artist(int width, int height)
:
artist(width, height)
{
}

//////////////////////////////////////////////////////////////
void ggo::chryzode_artist::render_chryzode(void * buffer, int line_step, ggo::image_format format,
  float radius, const chryzode_params & params, float hue_start, float hue_end) const
{
  // Float buffer.
  std::vector<float> buffer_32f(get_width() * get_height(), 0.f);

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
      ggo::paint_shape<y_32f_yu, sampling_2x2>(buffer_32f.data(), get_width(), get_height(), line_step,
        ggo::capsule_float(p1, p2, 0.005f * radius),
        ggo::solid_color_brush<float>(1.f), ggo::add_blender<float>());
    }
  }
  
  // Convert from gray to rgb.
  uint8_t * buffer_8u = static_cast<uint8_t *>(buffer);
  for (int i = get_width() * get_height() - 1; i >= 0; --i)
  {
    float coef = buffer_32f[i] / 512; // Normalize.
    float hue = ggo::map<float>(coef, 0, 1, hue_start, hue_end);
    const ggo::color_8u color = from_hsv<ggo::color_8u>(hue, 1 - coef, coef);
    buffer_8u[3 * i + 0] = color.r();
    buffer_8u[3 * i + 1] = color.g();
    buffer_8u[3 * i + 2] = color.b();
  }
}
