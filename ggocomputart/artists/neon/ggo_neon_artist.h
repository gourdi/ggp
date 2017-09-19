#ifndef __GGO_NEON_ARTIST__
#define __GGO_NEON_ARTIST__

#include <ggo_animation_artist_abc.h>

namespace ggo
{
  class neon_artist : public animation_artist_abc
  {
  public:

          neon_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt);

  private:

    void  init() override;
    bool  update() override;
    void  render_frame(void * buffer, const ggo::rect_int & clipping) override;

    void	paint_point(void * buffer, const ggo::pos2f & point_pos, const ggo::color_8u & color, const ggo::rect_int & clipping) const;

  private:

    static constexpr int substeps_count = 10;

    int _frame_index;
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
