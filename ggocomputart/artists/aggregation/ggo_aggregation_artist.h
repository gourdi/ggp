#ifndef __GGO_AGGREGATION_ARTIST__
#define __GGO_AGGREGATION_ARTIST__

#include <ggo_artist_abc.h>
#include <ggo_array.h>

namespace ggo
{
  class aggregation_artist : public ggo_artist_abc
  {
  public:

          aggregation_artist(int width, int height);

    void  update(int points_count);
    void  render(uint8_t * buffer) const;

    int   get_final_points_count() const;

  private:

    void  register_point(const ggo::pos2f & pos, float hue, float sat, float val);
    void  update();

  private:

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

    ggo::color          _background_color;
    ggo::array<cell, 2> _grid;
    float               _threshold_hypot;
    float               _threshold_dist;
  };
}

#endif
