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
  float color::max_diff(const color & color1, const color & color2)
  {
    float dr = std::abs(color1.r() - color2.r());
    float dg = std::abs(color1.g() - color2.g());
    float db = std::abs(color1.b() - color2.b());

    return std::max(dr, std::max(dg, dr));
  }
}



