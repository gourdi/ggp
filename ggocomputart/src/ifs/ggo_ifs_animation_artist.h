#ifndef __GGO_IFS_ANIMATION_ARTIST__
#define __GGO_IFS_ANIMATION_ARTIST__

#include <ggo_animation_artist.h>

namespace ggo
{
  class ifs_animation_artist : public progress_animation_artist
  {
  public:

          ifs_animation_artist(
            int width, int height, int line_byte_step,
            ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order,
            ggo::ratio fps);

  private:

    void  render_frame(void * buffer, float progress) override;

  private:

    ggo::rgb_8u     _bkgd_colors[4];
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


