#ifndef __GGO_HEXA_ANIMATION_ARTIST__
#define __GGO_HEXA_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include <ggo_scalar_field_2d_abc.h>
#include <ggo_point_camera.h>
#include "ggo_hexa_artist.h"

namespace ggo
{
  class object3d;

  class hexa_animation_artist : public animation_artist_abc
  {
  public:

    hexa_animation_artist(int render_width, int render_height);

  private:

    void	init_sub() override;
    bool	render_next_frame_sub(void * buffer, int frame_index) override;

  private:

    ggo::color_32f                            _color_top;
    ggo::color_32f                            _color_side;
    float                                     _camera_rotation_start;
    float                                     _camera_rotation_end;
    std::vector<ggo::pos3f>                   _lights_pos;
    ggo::color_32f                            _fog_color;
    std::vector<ggo::hexa_artist::hexa_info>  _hexa_infos;
  };
}

#endif

