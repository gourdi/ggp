#ifndef __GGO_NEWTON_ARTIST__
#define __GGO_NEWTON_ARTIST__

#include <ggo_animation_artist_abc.h>

namespace ggo
{
  class newton_artist : public static_background_animation_artist_abc
  {
  public:

    newton_artist(int render_width, int render_height);

  private:

    void	init_sub() override;
    void	init_bkgd_buffer(void * bkgd_buffer) const override;
    bool	render_next_frame_bkgd(void * buffer, int frame_index) override;

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

    float					      _hue;
    std::vector<newton> _newtons;
    std::vector<string> _strings;
    std::vector<float>	_max_velocities;
    ggo::vec2f		      _shadow_offset;
  };
}

#endif