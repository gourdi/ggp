#ifndef __GGO_VORTEX_ANIM_ARTIST__
#define __GGO_VORTEX_ANIM_ARTIST__

#include <ggo_animation_artist_abc.h>
#include "ggo_vortex_artist.h"
#include <map>

namespace ggo
{
  class vortex_animation_artist : public animation_artist_abc
  {
  public:

          vortex_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt);

  private:

    void  init() override;
    bool  prepare_frame() override;
    void  process_frame(void * buffer, const ggo::rect_int & clipping) override;

  private:

    struct vortex_path
    {
      ggo::pos2f	_start_pos;
      ggo::pos2f	_end_pos;
    };

    int                                                 _frame_index;
    std::map<ggo::vortex_artist::vortex *, vortex_path> _vortices_paths;
    ggo::vortex_artist::params                          _params;
  };
}

#endif
