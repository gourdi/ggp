#ifndef __GGO_ENTABENI_ANIMATION_ARTIST__
#define __GGO_ENTABENI_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include <ggo_curve.h>
#include <ggo_random_interpolator_abc.h>

class ggo_entabeni_animation_artist : public ggo_animation_artist_abc
{
public:

  ggo_entabeni_animation_artist(int render_width, int render_height);

private:

  void init_sub() override;
  bool render_next_frame_sub(uint8_t * buffer, int frame_index) override;

private:

  class angle_interpolator : public ggo::random_interpolator_abc<float, float>
  {
    void get_random_data(float & data, float & dt) override
    {
      data = ggo::rand_float(0.f, 2.f * ggo::pi<float>());
      dt = ggo::rand_float(50, 100);
    }
  };
  
  angle_interpolator                  _angle;
  float                               _z;
  ggo::array<float, 2>                _grid_start;
  ggo::array<float, 2>                _grid_end;
  ggo::cubic_curve<float, ggo::color> _color_map;
};

#endif
