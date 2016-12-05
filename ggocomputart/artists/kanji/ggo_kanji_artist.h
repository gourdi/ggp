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
    void	render_frame(void * buffer, int line_step, ggo::pixel_buffer_format pbf, int frame_index, const ggo::pixel_rect & clipping) const;

  private:

    void	init_shaker();
    void	init_attractor();

  private:

    struct particle
    {
      ggo::pos2f _prv_pos;
      ggo::pos2f _cur_pos;
    };

    std::vector<particle> _particles;
    ggo::pos2f        		_attractor;
    int							      _timer_max;
    ggo::color_8u 	      _parts_color;
    int							      _attractor_counter;
    int							      _shake_counter;
  };
}

#endif
