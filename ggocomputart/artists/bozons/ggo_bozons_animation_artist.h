#ifndef __GGO_BOZONS_ANIMATION_ARTIST__
#define __GGO_BOZONS_ANIMATION_ARTIST__

#include "ggo_animation_artist_abc.h"
#include <list>

namespace ggo
{
  class bozons_animation_artist : public animation_artist_abc
  {
  public:

          bozons_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt);

    void  init() override;
    bool  prepare_frame() override;
    void  process_frame(void * buffer, const ggo::rect_int & clipping) override;

  private:

    struct bozon
    {
      ggo::pos2f    _prv_pos;
      ggo::pos2f    _cur_pos;
      ggo::color_8u _color;
      float				  _angle;
      float				  _dangle;
      int           _counter;
      float				  _speed;
    };

    int               _frame_index;
    std::list<bozon>  _bozons;
    float             _hue;
    ggo::color_8u     _bkgd_color1;
    ggo::color_8u     _bkgd_color2;
    ggo::color_8u     _bkgd_color3;
    ggo::color_8u     _bkgd_color4; 
  };
}

#endif
