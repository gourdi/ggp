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

                                aggregation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf);

    void                        update(int points_count);
    void                        render(void * buffer) const;

    int                         get_final_points_count() const;
    ggo::color_8u               get_background_color() const { return _background_color; }
    const ggo::array<cell, 2> & get_grid() const { return _grid; }
    float                       get_threshold_dist() const { return _threshold_dist; }


  private:

    void                        register_point(const ggo::pos2f & pos, float hue, float sat, float val);
    void                        update();

  private:

    ggo::color_8u       _background_color;
    ggo::array<cell, 2> _grid;
    float               _threshold_hypot;
    float               _threshold_dist;
  };
}

#endif
