#ifndef __GGO_BOZONS_ANIMATION_ARTIST__
#define __GGO_BOZONS_ANIMATION_ARTIST__

#include "ggo_animation_artist_abc.h"

namespace ggo
{
  class bozons_animation_artist : public static_background_animation_artist_abc
  {
  public:

    bozons_animation_artist(int render_width, int render_height);

  private:

    void init_sub() override;
    void init_bkgd_buffer(void * buffer) override;
    bool render_next_frame_bkgd(void * buffer, int frame_index) override;

  private:

    struct ggo_particle
    {
      ggo::pos2f    _pos;
      ggo::color_8u _color;
      float				  _angle;
      float				  _dangle;
      int 				  _dangle_counter;
      float				  _radius;
      float				  _dradius;
      int 				  _dradius_counter;
      float				  _speed;
    };

    struct ggo_color_disc
    {
      ggo::disc_float _disc;
      ggo::color_8u   _color;
    };

    std::vector<std::vector<ggo_color_disc>>  _color_discs;
    float                                     _hue;
  };
}

#endif
