#ifndef __GGO_REDIFF_ANIMPATION_ARTIST__
#define __GGO_REDIFF_ANIMPATION_ARTIST__

#include <ggo_animation_artist_abc.h>

namespace ggo
{
  class rediff_animation_artist : public accumulation_animation_artist_abc
  {
  public:

          rediff_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt);

  private:

    void	init_sub() override;
    void	init_output_buffer(void * buffer) const override;
    bool	render_next_frame_acc(void * buffer, int frame_index) override;

    void  update();
    void  render(void * buffer) const;

  private:

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
