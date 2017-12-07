#ifndef __GGO_ARTIST_KANJI__
#define __GGO_ARTIST_KANJI__

#include <ggo_artist.h>
#include <vector>

namespace ggo
{
  class kanji_artist : public artist
  {
  public:

          kanji_artist(int width, int height);

    void	init();
    bool	update(int frame_index);
    void	process_frame(void * buffer, int line_step, ggo::pixel_buffer_format pbf, int frame_index, const ggo::rect_int & clipping) const;

  private:

    static constexpr int substeps_count = 8;

    std::vector<std::array<ggo::pos2f, substeps_count>> _particles;
    ggo::pos2f        		                              _attractor;
    int							                                    _timer_max;
    ggo::color_8u 	                                    _parts_color;
    int							                                    _attractor_counter;
    int							                                    _shake_counter;
  };
}

#endif
