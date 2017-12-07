#ifndef __GGO_REDIFF_ANIMPATION_ARTIST__
#define __GGO_REDIFF_ANIMPATION_ARTIST__

#include <ggo_animation_artist_abc.h>

namespace ggo
{
  class rediff_animation_artist : public animation_artist_abc
  {
  public:

          rediff_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt);

  private:

    void  init() override;
    bool  prepare_frame() override;
    void  process_frame(void * buffer, const ggo::rect_int & clipping) override;

  private:

    int                   _frame_index;
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
