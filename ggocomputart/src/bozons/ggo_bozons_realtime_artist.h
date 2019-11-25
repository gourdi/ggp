#ifndef __GGO_BOZONS_REALTIME_ARTIST__
#define __GGO_BOZONS_REALTIME_ARTIST__

#include "ggo_realtime_artist.h"
#include <list>

namespace ggo
{
  class bozons_realtime_artist : public realtime_artist
  {
  public:

          bozons_realtime_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, ggo::ratio fps);

    void  preprocess_frame(void * buffer, uint32_t cursor_events, ggo::pos2_i cursor_pos) override;
    void  render_tile(void * buffer, const ggo::rect_int & clipping) override;
    bool  finished() override;

  private:

    void create_bozon();

    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
    void render_background_t(void * buffer) const;

    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
    void render_tile_t(void * buffer, const ggo::rect_int & clipping) const;

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

    struct capsule
    {
      capsule(ggo::pos2_f p1, ggo::pos2_f p2, float radius, ggo::rgb_8u color) :
        _p1(p1),
        _p2(p2),
        _radius(radius),
        _color(color)
      {
      }

      ggo::pos2_f _p1;
      ggo::pos2_f _p2;
      float _radius;
      ggo::rgb_8u _color;
    };

    std::list<bozon>      _bozons;
    std::vector<capsule>  _capsules;
    float                 _hue = 0.f;
    ggo::rgb_8u           _bkgd_color1;
    ggo::rgb_8u           _bkgd_color2;
    ggo::rgb_8u           _bkgd_color3;
    ggo::rgb_8u           _bkgd_color4; 
    float                 _substeps = 0.f;
    float                 _substeps_per_frame;
    int                   _substeps_count = 0;
  };
}

#endif
