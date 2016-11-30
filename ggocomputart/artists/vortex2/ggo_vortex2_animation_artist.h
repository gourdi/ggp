#ifndef __GGO_VORTEX2_ANIM_ARTIST__
#define __GGO_VORTEX2_ANIM_ARTIST__

#include "ggo_animation_artist_abc.h"
#include "ggo_vortex2_artist.h"
#include <map>

namespace ggo
{
  class vortex2_animation_artist : public animation_artist_abc
  {
  public:

          vortex2_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt);

  private:

    void  init() override;
    bool  update() override;
    void  render_frame(void * buffer, const ggo::pixel_rect & clipping) const override;

  private:

    struct vortex_path
    {
      ggo::pos2f	_start_pos;
      ggo::pos2f	_end_pos;
    };

    int                                                   _frame_index;
    std::map<ggo::vortex2_artist::vortex *, vortex_path>  _vortices_paths;
    ggo::vortex2_artist::params                           _params;
  };
}

#endif
