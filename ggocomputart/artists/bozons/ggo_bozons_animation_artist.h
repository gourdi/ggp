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

    void  init_animation() override;
    bool  prepare_frame() override;
    void  render_frame(void * buffer, const ggo::rect_int & clipping) override;

  private:

    void create_bozon();

    template <pixel_buffer_format pbf>
    void process_frame(void * buffer, const ggo::rect_int & clipping) const;

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
      float         _radius;
    };

    int               _frame_index;
    std::list<bozon>  _bozons;
    float             _hue;
    ggo::color_8u     _bkgd_color1;
    ggo::color_8u     _bkgd_color2;
    ggo::color_8u     _bkgd_color3;
    ggo::color_8u     _bkgd_color4; 
  };

  template <pixel_buffer_format pbf>
  void bozons_animation_artist::process_frame(void * buffer, const ggo::rect_int & clipping) const
  {
    if (_frame_index == 0)
    {
      ggo::fill_4_colors<pbf>(buffer, get_width(), get_height(), get_line_step(),
        _bkgd_color1, _bkgd_color2, _bkgd_color3, _bkgd_color4, clipping);
    }

    for (const auto & bozon : _bozons)
    {
      ggo::paint_shape<pbf, ggo::sampling_4x4>(buffer, get_width(), get_height(), get_line_step(),
        ggo::extended_segment_float(bozon._prv_pos, bozon._cur_pos, bozon._radius),
        ggo::solid_color_brush<ggo::color_8u>(bozon._color), ggo::overwrite_blender<color_8u>(), clipping);
    }
  }
}

#endif
