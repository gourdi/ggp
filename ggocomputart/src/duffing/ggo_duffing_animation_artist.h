#ifndef __GGO_DUFFING_ANIMATION_ARTIST__
#define __GGO_DUFFING_ANIMATION_ARTIST__

#include <ggo_animation_artist.h>
#include <kernel/memory/ggo_array.h>
#include <kernel/math/interpolation/ggo_curve.h>
#include "ggo_duffing.h"

namespace ggo
{
  class duffing_animation_artist : public animation_artist
  {
  public:

    duffing_animation_artist(
      int width, int height, int line_byte_step,
      ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order,
      ggo::ratio fps);

  private:

    void  render_frame(void * buffer, bool & finished) override;

  private:

    struct paint_point
    {
      ggo::pos2_f _pos;
      float       _opacity;
    };

    ggo::duffing              _duffing;
    float                     _angle;
    float                     _opacity_start;
    float                     _opacity_decay;
    ggo::rgb_8u               _point_color;
    ggo::rgb_8u               _bkgd_color1;
    ggo::rgb_8u               _bkgd_color2;
    ggo::rgb_8u               _bkgd_color3;
    ggo::rgb_8u               _bkgd_color4;
    std::vector<paint_point>  _paint_points;
    float                     _paint_points_per_frame;
    float                     _paint_points_to_create = 0.f;
    ggo::ratio                _fps;
    ggo::ratio                _elapsed_time{ 0, 1 };
  };
}

#endif
