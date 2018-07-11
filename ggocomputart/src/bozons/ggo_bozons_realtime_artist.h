#ifndef __GGO_BOZONS_REALTIME_ARTIST__
#define __GGO_BOZONS_REALTIME_ARTIST__

#include "ggo_realtime_artist_abc.h"
#include <list>

namespace ggo
{
  class bozons_realtime_artist : public realtime_artist_abc
  {
  public:

          bozons_realtime_artist(int width, int height, int line_step, ggo::image_format format);

    void  preprocess_frame(int frame_index) override;
    void  render_tile(void * buffer, int frame_index, const ggo::rect_int & clipping) override;
    bool  finished(int frame_index) const override;

  private:

    void create_bozon();

    template <ggo::image_format format>
    void render_tile_t(void * buffer, int frame_index, const ggo::rect_int & clipping) const;

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

    std::list<bozon>  _bozons;
    float             _hue = 0.f;
    ggo::color_8u     _bkgd_color1;
    ggo::color_8u     _bkgd_color2;
    ggo::color_8u     _bkgd_color3;
    ggo::color_8u     _bkgd_color4; 
  };

  template <ggo::image_format format>
  void bozons_realtime_artist::render_tile_t(void * buffer, int frame_index, const ggo::rect_int & clipping) const
  {
    if (frame_index == 0)
    {
      ggo::fill_4_colors<format>(buffer, width(), height(), line_step(),
        _bkgd_color1, _bkgd_color2, _bkgd_color3, _bkgd_color4, clipping);
    }

    for (const auto & bozon : _bozons)
    {
      ggo::paint_shape<format, ggo::sampling_4x4>(buffer, width(), height(), line_step(),
        ggo::capsule_float(bozon._prv_pos, bozon._cur_pos, bozon._radius),
        ggo::solid_color_brush<ggo::color_8u>(bozon._color), ggo::overwrite_blender<color_8u>(), clipping);
    }
  }
}

#endif