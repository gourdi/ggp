#ifndef __GGO_SONSON_ARTIST__
#define __GGO_SONSON_ARTIST__

#include <kernel/ggo_vec2.h>
#include <kernel/ggo_ratio.h>

namespace ggo
{
  class sonson_artist
  {
  public:

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

    bool update(ggo::ratio frame_duration);
    std::vector<glowing_circle> get_circles() const;

  private:

    struct animated_glowing_circle
    {
      ggo::pos2_f _center;
      ggo::vec2_f _velocity;

      float _r_angle;
      float _g_angle;
      float _b_angle;
      float _dr_angle;
      float _dg_angle;
      float _db_angle;

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