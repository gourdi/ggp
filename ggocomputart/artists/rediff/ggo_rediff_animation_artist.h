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
    bool  update() override;
    void  render_frame(void * buffer, const ggo::pixel_rect & clipping) override;

  private:

    int                 _frame_index;
    float               _hue;
    std::vector<float>  _feed_map;
    std::vector<float>  _kill_map;
    float               _food_stddev;
    float               _life_stddev;
    std::vector<float>  _food;
    std::vector<float>  _life;
  };
}

#endif
