#ifndef __GGO_ENTABENI_ANIMATION_ARTIST__
#define __GGO_ENTABENI_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include <ggo_curve.h>
#include <ggo_random_interpolator_abc.h>

namespace ggo
{
  class entabeni_animation_artist : public animation_artist_abc
  {
  public:

          entabeni_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt);

  private:

    void  init() override;
    bool  update() override;
    void  render_frame(void * buffer, const ggo::pixel_rect & clipping) override;

  private:

    class angle_interpolator : public ggo::random_interpolator_abc<float, float>
    {
      void get_random_data(float & data, float & dt) override
      {
        data = ggo::rand<float>(0.f, 2.f * ggo::pi<float>());
        dt = ggo::rand<float>(50, 100);
      }
    };

    int                                     _frame_index;
    float                                   _angle;
    angle_interpolator                      _angle_interpolator;
    float                                   _z;
    ggo::array<float, 2>                    _grid_start;
    ggo::array<float, 2>                    _grid_end;
    ggo::cubic_curve<float, ggo::color_32f> _color_map;
  };
}

#endif
