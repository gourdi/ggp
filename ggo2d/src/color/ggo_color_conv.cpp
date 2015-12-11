#include "ggo_color_conv.h"
#include <ggo_kernel.h>
#include <math.h>

//////////////////////////////////////////////////////////////
// Reference: http://en.wikipedia.org/wiki/HSL_color_space
namespace ggo
{
  void hsv2rgb(float h, float s, float v, float & r, float & g, float & b)
  {
    h = std::fmod(h, 1.f);
    if (h < 0)
    {
      h += 1;
    }
    s = ggo::clamp(s, 0.0f, 1.0f);
    v = ggo::clamp(v, 0.0f, 1.0f);

    int		h_i	= ggo::clamp(int(h * 6), 0, 5);
    float	f	= h * 6 - std::floor(h * 6);
    float	p	= v * (1 - s);
    float	q	= v * (1 - f * s);
    float	t	= v * (1 - (1 - f) * s);

    switch (h_i)
    {
    case 0:
      r = v;
      g = t;
      b = p;
      break;
    case 1:
      r = q;
      g = v;
      b = p;
      break;
    case 2:
      r = p;
      g = v;
      b = t;
      break;
    case 3:
      r = p;
      g = q;
      b = v;
      break;
    case 4:
      r = t;
      g = p;
      b = v;
      break;
    case 5:
      r = v;
      g = p;
      b = q;
      break;
    default:
      GGO_FAIL();
      break;
    }
  }
}
