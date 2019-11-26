#ifndef __GGO_KANJI_REALTIME_ARTIST__
#define __GGO_KANJI_REALTIME_ARTIST__

#include <ggo_realtime_artist.h>
#include <ggo_substeps_processing.h>

namespace ggo
{
  class kanji_realtime_artist : public realtime_artist
  {
  public:

          kanji_realtime_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, ggo::ratio fps);

  private:

    void  preprocess_frame(void * buffer, uint32_t cursor_events, ggo::pos2_i cursor_pos) override;
    void  render_tile(void * buffer, const ggo::rect_int & clipping) override;
    bool  finished() override;

    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
    void  render_tile_t(void * buffer, const ggo::rect_int & clipping);

  private:

    struct particle
    {
      ggo::pos2_f _prv_pos;
      ggo::pos2_f _cur_pos;
    };

    std::vector<particle> _particles;
    std::vector<pos2_f> _points;
    ggo::pos2_f _attractor;
    int _timer_max;
    ggo::rgb_8u _parts_color;
    int _attractor_counter;
    int _shake_counter;
    substeps_processing _substeps_processing;
  };
}

#endif
