#ifndef __GGO_NEON_ARTIST__
#define __GGO_NEON_ARTIST__

#include <ggo_realtime_artist.h>
#include <2d/ggo_image.h>

namespace ggo
{
  class neon_realtime_artist : public realtime_artist
  {
  public:

          neon_realtime_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, ggo::ratio fps);

  private:

    void  preprocess_frame(void * buffer, uint32_t cursor_events, ggo::pos2_i cursor_pos) override;
    void  render_tile(void * buffer, const ggo::rect_int & clipping) override;
    bool  finished() override;

    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
    void  render_tile_t(void * buffer, const ggo::rect_int & clipping) const;

    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
    void paint_point_t(image_t<pixel_type, memory_lines_order> & img, const ggo::pos2_f & point_pos, const ggo::rgb_8u & color, const ggo::rect_int & clipping) const;

  private:

    float	_angle;
    float	_radius_prv;
    float	_radius_cur;
    float	_radius_attractor;
    ggo::rgb_8u _attractor_color;
    std::vector<ggo::pos2_f> _attractor_points;
    std::vector<ggo::pos2_f> _points;
    float _substeps = 0.f;
    float _substeps_per_frame;
    int _substeps_count = 0;
  };
}

#endif
