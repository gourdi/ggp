#ifndef __GGO_HEXA_ANIMATION_ARTIST__
#define __GGO_HEXA_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include <ggo_scalar_field_2d_abc.h>
#include <ggo_point_camera.h>
#include "ggo_hexa_artist.h"

namespace ggo
{
  class object3d;
}

class ggo_hexa_animation_artist : public ggo_animation_artist_abc
{
public:
    
        ggo_hexa_animation_artist(int render_width, int render_height);

private:

	void	init_sub() override;
	bool	render_next_frame_sub(uint8_t * buffer, int frame_index) override;
  
private:

  ggo::color                              _color_top;
  ggo::color                              _color_side;
  float                                   _camera_rotation_start;
  float                                   _camera_rotation_end;
  std::vector<ggo::point3d_float>         _lights_pos;
  ggo::color                              _fog_color;
  std::vector<ggo_hexa_artist::hexa_info> _hexa_infos;
};

#endif

