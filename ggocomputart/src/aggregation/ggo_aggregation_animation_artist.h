#ifndef __GGO_AGGREGATION_ANIMATION_ARTIST__
#define __GGO_AGGREGATION_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include <kernel/memory/ggo_array.h>

namespace ggo
{
  class aggregation_animation_artist : public fixed_frames_count_animation_artist_abc
  {
  public:

          aggregation_animation_artist(int width, int height, int line_step, ggo::image_format format);

  private:

    void  render_frame(void * buffer, int frame_index) override;

    void  register_point(const ggo::pos2f & pos, float hue, float sat, float val);
    void  update();
    void  update(int points_count);
    int   get_final_points_count() const;

    template <ggo::image_format format>
    void  render_t(void * buffer) const;

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

    ggo::rgb_8u         _background_color;
    ggo::array<cell, 2> _grid;
    float               _threshold_hypot;
    float               _threshold_dist;
  };
}

#endif
