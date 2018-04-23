#ifndef __GGO_METABALLS_ANIMATION_ARTIST__
#define __GGO_METABALLS_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include "ggo_metaballs_artist.h"

namespace ggo
{
  class metaballs_animation_artist : public animation_artist_abc
  {
  public:

    metaballs_animation_artist(int width, int height, int line_step, ggo::image_format format, rendering_type rt);

  private:

    struct moving_center
    {
      ggo::pos3f  _center;
      float				_radius = 0.f;
      float				_start_angle1 = 0.f;
      float				_start_angle2 = 0.f;
      float				_end_angle1 = 0.f;
      float				_end_angle2 = 0.f;
    };

  private:

    void  init_animation() override;
    bool  prepare_frame() override;
    void  render_frame(void * buffer, const ggo::rect_int & clipping) override;

  private:

    int                             _frame_index = 0;
    ggo::metaballs_artist::params   _params;
    std::vector<moving_center>      _centers;
    ggo::basis3d_float              _camera_basis;
  };
}

#endif
