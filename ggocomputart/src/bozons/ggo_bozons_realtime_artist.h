#ifndef __GGO_BOZONS_REALTIME_ARTIST__
#define __GGO_BOZONS_REALTIME_ARTIST__

#include "ggo_realtime_artist_abc.h"
#include <list>

namespace ggo
{
  class bozons_realtime_artist : public realtime_artist_abc
  {
  public:

          bozons_realtime_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);

    void  preprocess_frame(int frame_index, uint32_t cursor_events, ggo::pos2_i cursor_pos, float time_step) override;
    void  render_tile(void * buffer, int frame_index, const ggo::rect_int & clipping) override;
    bool  finished(int frame_index) const override;

  private:

    void create_bozon();

    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
    void render_tile_t(void * buffer, int frame_index, const ggo::rect_int & clipping) const;

  private:

    struct bozon
    {
      ggo::pos2_f _prv_pos;
      ggo::pos2_f _cur_pos;
      ggo::rgb_8u _color;
      float				_angle;
      float				_dangle;
      int         _counter;
      float				_speed;
      float       _radius;
    };

    std::list<bozon>  _bozons;
    float             _hue = 0.f;
    ggo::rgb_8u       _bkgd_color1;
    ggo::rgb_8u       _bkgd_color2;
    ggo::rgb_8u       _bkgd_color3;
    ggo::rgb_8u       _bkgd_color4; 
  };

  template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
  void bozons_realtime_artist::render_tile_t(void * buffer, int frame_index, const ggo::rect_int & clipping) const
  {
    ggo::image_t<pixel_type, memory_lines_order> image(buffer, size(), line_byte_step());

    if (frame_index == 0)
    {
      ggo::fill_4_colors(image, _bkgd_color1, _bkgd_color2, _bkgd_color3, _bkgd_color4, clipping);
    }

    for (const auto & bozon : _bozons)
    {
      ggo::paint<ggo::sampling_4x4>(image,
        ggo::capsule_f(bozon._prv_pos, bozon._cur_pos, bozon._radius),
        ggo::solid_color_brush<ggo::rgb_8u>(bozon._color), ggo::overwrite_blender<rgb_8u>(), clipping);
    }
  }
}

#endif
