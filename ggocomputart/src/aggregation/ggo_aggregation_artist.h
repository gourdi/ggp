#ifndef __GGO_AGGREGATION_ARTIST__
#define __GGO_AGGREGATION_ARTIST__

#include <ggo_array.h>
#include <ggo_artist.h>

namespace ggo
{
  class aggregation_artist : public artist
  {
  public:

    struct point
    {
      ggo::pos2f  _pos;
      float       _hue;
      float       _sat;
      float       _val;
      int         _counter;
    };

    struct cell
    {
      std::vector<point>  _points;
      ggo::rect_float     _rect;
    };

                                aggregation_artist(int width, int height);

    void                        update(int points_count);
    void                        render(void * buffer, int line_step, ggo::pixel_buffer_format pbf) const;

    int                         get_final_points_count() const;

  private:

    void                        register_point(const ggo::pos2f & pos, float hue, float sat, float val);
    void                        update();

    template <ggo::pixel_buffer_format pbf>
    void                        render(void * buffer, int line_step) const;

  private:

    ggo::color_8u       _background_color;
    ggo::array<cell, 2> _grid;
    float               _threshold_hypot;
    float               _threshold_dist;
  };
}

#include "ggo_aggregation_artist.imp.h"

#endif
