#ifndef __GGO_SONSON_ARTIST__
#define __GGO_SONSON_ARTIST__

#include <kernel/ggo_vec2.h>
#include <kernel/ggo_ratio.h>

namespace ggo
{
  class sonson_artist
  {
  public:

    static constexpr int duration = 10;

    sonson_artist();

    struct glowing_circle
    {
      ggo::pos2_f _center;

      float _r_angle;
      float _g_angle;
      float _b_angle;
      float _opacity;

      std::array<float, 4> _radius;
      float _outter_size;
    };

    std::vector<glowing_circle> get_circles(float progress) const;

  private:

    struct animated_glowing_circle
    {
      ggo::pos2_f _center_start;
      ggo::pos2_f _center_end;

      float _r_angle_start;
      float _g_angle_start;
      float _b_angle_start;
      float _r_angle_end;
      float _g_angle_end;
      float _b_angle_end;

      float _radius;
      float _inner_size;
      float _outter_size;
      float _opacity;
    };

  private:

    std::vector<animated_glowing_circle>  _circles;
    ggo::ratio                            _elapsed_time = 0;
  };
}

#endif