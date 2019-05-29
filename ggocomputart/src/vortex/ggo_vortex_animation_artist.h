#ifndef __GGO_VORTEX_ANIM_ARTIST__
#define __GGO_VORTEX_ANIM_ARTIST__

#include <ggo_animation_artist_abc.h>
#include "ggo_vortex_artist.h"
#include <map>

namespace ggo
{
  class vortex_animation_artist : public fixed_frames_count_animation_artist_abc
  {
  public:

          vortex_animation_artist(int width, int height, int line_step, ggo::image_format format);

  private:

    void  render_frame(void * buffer, int frame_index, float time_step) override;

  private:

    struct vortex_path
    {
      ggo::pos2_f	_start_pos;
      ggo::pos2_f	_end_pos;
    };

    std::map<ggo::vortex_artist::vortex *, vortex_path> _vortices_paths;
    ggo::vortex_artist::params                          _params;
  };
}

#endif
