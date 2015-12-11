#include "ggo_color.h"
#include "ggo_color_conv.h"

namespace ggo
{
  const color color::WHITE(1, 1, 1);
  const color color::GRAY(0.5, 0.5, 0.5);
  const color color::BLACK(0, 0, 0);
  const color color::RED(1, 0, 0);
  const color color::GREEN(0, 1, 0);
  const color color::BLUE(0, 0, 1);
  const color color::CYAN(0, 1, 1);
  const color color::MAGENTA(1, 0, 1);
  const color color::YELLOW(1, 1, 0);
  const color color::ORANGE(1, 0.5, 0);

  //////////////////////////////////////////////////////////////
  void color::randomize() 
  {
    _r = ggo::rand_float();
    _g = ggo::rand_float();
    _b = ggo::rand_float();
  }

  //////////////////////////////////////////////////////////////
  color color::from_hsv(float hue, float saturation, float value)
  {
    color c;

    ggo::hsv2rgb(hue, saturation, value, c.r(), c.g(), c.b());

    return c;
  }

  //////////////////////////////////////////////////////////////
  color color::get_random()
  {
    return color(ggo::rand_float(), ggo::rand_float(), ggo::rand_float());
  }

  //////////////////////////////////////////////////////////////
  color color::linear_interpolate(float x0, const color & color0, float x1, const color & color1, float x)
  {
    float det = x0 - x1;
    if (std::abs(det) < 0.000001)
    {
      return 0.5 * (color0 + color1);
    }

    float inv = 1 / det;

    float a_r = color0.r() - color1.r();
    float b_r = x0 * color1.r() - x1 * color0.r();
    float r = (a_r * x + b_r) * inv;

    float a_g = color0.g() - color1.g();
    float b_g = x0 * color1.g() - x1 * color0.g();
    float g = (a_g * x + b_g) * inv;

    float a_b = color0.b() - color1.b();
    float b_b = x0 * color1.b() - x1 * color0.b();
    float b = (a_b * x + b_b) * inv;

    return color(r, g, b);
  }

  //////////////////////////////////////////////////////////////
  float color::max_diff(const color & color1, const color & color2)
  {
    float dr = std::abs(color1.r() - color2.r());
    float dg = std::abs(color1.g() - color2.g());
    float db = std::abs(color1.b() - color2.b());

    return std::max(dr, std::max(dg, dr));
  }
}



