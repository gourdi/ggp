#ifndef __GGO_NEON_ARTIST__
#define __GGO_NEON_ARTIST__

#include <ggo_realtime_artist_abc.h>

namespace ggo
{
  class neon_artist : public fixed_frames_count_realtime_artist_abc
  {
  public:

          neon_artist(int width, int height, int line_step, ggo::image_format format);

  private:

    void  preprocess_frame(int frame_index) override;
    void  render_tile(void * buffer, int frame_index, const ggo::rect_int & clipping) override;
    int   frames_count() const override { return 900; }

    void	paint_point(void * buffer, const ggo::pos2f & point_pos, const ggo::color_8u & color, const ggo::rect_int & clipping) const;

  private:

    static constexpr int substeps_count = 10;

    float	_angle;
    float	_radius_prv;
    float	_radius_cur;
    float	_radius_attractor;
    ggo::color_8u _attractor_color;
    std::array<ggo::pos2f, substeps_count> _attractor_points;
    std::array<ggo::pos2f, substeps_count> _points;
  };
}

#endif
