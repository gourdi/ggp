#ifndef __GGO_REDIFF_ANIMPATION_ARTIST__
#define __GGO_REDIFF_ANIMPATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include <kernel/memory/ggo_array.h>

namespace ggo
{
  class rediff_animation_artist : public fixed_frames_count_animation_artist_abc
  {
  public:

          rediff_animation_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);

  private:

    void  render_frame(void * buffer, int frame_index, float time_step) override;

  private:

    float                 _hue;
    ggo::array<float, 2>  _feed_map;
    ggo::array<float, 2>  _kill_map;
    float                 _food_stddev;
    float                 _life_stddev;
    ggo::array<float, 2>  _food;
    ggo::array<float, 2>  _life;
  };
}

#endif
