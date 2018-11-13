#ifndef __GGO_KANJI_REALTIME_ARTIST__
#define __GGO_KANJI_REALTIME_ARTIST__

#include <ggo_realtime_artist_abc.h>

namespace ggo
{
  class kanji_realtime_artist : public fixed_frames_count_realtime_artist_abc
  {
  public:

          kanji_realtime_artist(int width, int height, int line_step, ggo::image_format format);

  private:

    void  preprocess_frame(int frame_index, uint32_t cursor_events, ggo::pos2i cursor_pos) override;
    void  render_tile(void * buffer, int frame_index, const ggo::rect_int & clipping) override;
    int   frames_count() const override { return 500; }

    template <image_format format>
    void  render_tile_t(void * buffer, int frame_index, const ggo::rect_int & clipping);

  private:

    static constexpr int substeps_count = 8;

    std::vector<std::array<ggo::pos2f, substeps_count>> _particles;
    ggo::pos2f        		                              _attractor;
    int							                                    _timer_max;
    ggo::rgb_8u 	                                      _parts_color;
    int							                                    _attractor_counter;
    int							                                    _shake_counter;
  };
}

#endif
