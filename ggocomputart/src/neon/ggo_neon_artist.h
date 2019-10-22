#ifndef __GGO_NEON_ARTIST__
#define __GGO_NEON_ARTIST__

#include <ggo_realtime_artist_abc.h>

namespace ggo
{
  class neon_artist : public fixed_frames_count_realtime_artist_abc
  {
  public:

          neon_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);

  private:

    void  preprocess_frame(int frame_index, uint32_t cursor_events, ggo::pos2_i cursor_pos, float time_step) override;
    void  render_tile(void * buffer, int frame_index, const ggo::rect_int & clipping) override;
    int   frames_count() const override { return 900; }

    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
    void  render_tile_t(void * buffer, int frame_index, const ggo::rect_int & clipping) const;

    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
    void paint_point_t(image_t<pixel_type, memory_lines_order> & img, const ggo::pos2_f & point_pos, const ggo::rgb_8u & color, const ggo::rect_int & clipping) const;

  private:

    static constexpr int substeps_count = 10;

    float	_angle;
    float	_radius_prv;
    float	_radius_cur;
    float	_radius_attractor;
    ggo::rgb_8u _attractor_color;
    std::array<ggo::pos2_f, substeps_count> _attractor_points;
    std::array<ggo::pos2_f, substeps_count> _points;
  };
}

#endif
