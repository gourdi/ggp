#ifndef __GGO_IFS_ANIMATION_ARTIST__
#define __GGO_IFS_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include "ggo_ifs_artist.h"

namespace ggo
{
  class ifs_animation_artist : public animation_artist_abc
  {
  public:

          ifs_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt);

  private:

    void  init_animation() override;
    bool  prepare_frame() override;
    void  render_frame(void * buffer, const ggo::rect_int & clipping) override;

  private:

    int             _frame_index;
    ggo::ifs_artist	_artist;
    ggo::color_8u   _bkgd_colors[4];
    float			      _hue;
    float 		      _transform_start[4];
    float 		      _transform_end[4];
    float			      _angle1_start;
    float			      _angle1_end;
    float			      _angle2_start;
    float			      _angle2_end;
  };
}

#endif

