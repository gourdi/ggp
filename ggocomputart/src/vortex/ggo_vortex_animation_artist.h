#ifndef __GGO_VORTEX_ANIM_ARTIST__
#define __GGO_VORTEX_ANIM_ARTIST__

#include <ggo_animation_artist.h>
#include "ggo_vortex_artist.h"
#include <map>

namespace ggo
{
  class vortex_animation_artist : public progress_animation_artist_abc
  {
  public:

          vortex_animation_artist(
            int width, int height, int line_byte_step,
            ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order,
            ggo::ratio fps);

  private:

    void  render_frame(void * buffer, float progress) override;

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
