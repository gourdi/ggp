#ifndef __GGO_NEWTON_ARTIST__
#define __GGO_NEWTON_ARTIST__

#include <ggo_animation_artist_abc.h>

namespace ggo
{
  class newton_artist : public animation_artist_abc
  {
  public:

          newton_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt);

  private:

    void  init() override;
    bool  prepare_frame() override;
    void  process_frame(void * buffer, const ggo::rect_int & clipping) override;

  private:

    bool	find_string(int index1, int index2) const;

    void	newton_update();
    void	newton_paint(void * buffer) const;

  private:

    struct newton
    {
      ggo::pos2f 	_cur_pos;
      ggo::pos2f  _prv_pos;
      float				_weight;
      ggo::vec2f	_forces;
      int					_timer;
    };

    struct string
    {
      int		_index1;
      int		_index2;
      float	_length;
    };

    int                       _frame_index;
    std::unique_ptr<uint8_t>  _background;
    float					            _hue;
    std::vector<newton>       _newtons;
    std::vector<string>       _strings;
    std::vector<float>	      _max_velocities;
    ggo::vec2f		            _shadow_offset;
  };
}

#endif