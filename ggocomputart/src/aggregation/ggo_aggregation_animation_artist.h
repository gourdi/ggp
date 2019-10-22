#ifndef __GGO_AGGREGATION_ANIMATION_ARTIST__
#define __GGO_AGGREGATION_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include <kernel/memory/ggo_array.h>

namespace ggo
{
  class aggregation_animation_artist : public fixed_frames_count_animation_artist_abc
  {
  public:

          aggregation_animation_artist(int width, int height, int line_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);

  private:

    void  render_frame(void * buffer, int frame_index, float time_step) override;

    void  register_point(const ggo::pos2_f & pos, float hue, float sat, float val);
    void  update();
    void  update(int points_count);
    int   get_final_points_count() const;

    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
    void  render_t(void * buffer) const;

  private:

    struct point
    {
      ggo::pos2_f _pos;
      float       _hue;
      float       _sat;
      float       _val;
      int         _counter;
    };

    struct cell
    {
      std::vector<point>  _points;
      ggo::rect_f         _rect;
    };

    ggo::rgb_8u         _background_color;
    ggo::array<cell, 2> _grid;
    float               _threshold_hypot;
    float               _threshold_dist;
  };
}

#endif
