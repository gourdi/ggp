#ifndef __GGO_PLASTIC_ARTIST__
#define __GGO_PLASTIC_ARTIST__

#include <ggo_gpu_artist.h>
#include <kernel/ggo_vec2.h>
#include <2d/ggo_color.h>

namespace ggo
{
  class plastic_artist
  {
  public:

    plastic_artist();

    template <typename func_t>
    void for_each_param(float progress, func_t && func) const
    {
      progress = ggo::ease_inout(progress);

      for (const auto & param : _params)
      {
        float angle = ggo::linerp(param._angle_start, param._angle_end, progress);
        ggo::pos2_f p = param._center + param._radius * ggo::vec2_f::from_angle(angle);

        func(param._den, param._mult, param._power, p);
      }
    }

    ggo::rgb_32f  color() const { return _color; }
    float         altitude_factor() const { return _altitude_factor; }

  private:

    struct params
    {
      float	  _den;
      float	  _mult;
      float   _power;
      pos2_f	_center;
      float	  _angle_start;
      float	  _angle_end;
      float	  _radius;
    };
    std::vector<params> _params;

    ggo::rgb_32f  _color;
    float 		    _altitude_factor;
  };
}

#endif
