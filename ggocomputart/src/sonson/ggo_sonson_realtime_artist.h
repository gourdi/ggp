#ifndef __GGO_SONSON_ARTIST__
#define __GGO_SONSON_ARTIST__

#include <ggo_realtime_artist_abc.h>
#include <kernel/memory/ggo_array.h>

namespace ggo
{
  class sonson_realtime_artist : public realtime_artist_abc
  {
  public:

    sonson_realtime_artist(int width, int height, int line_step, ggo::image_format format);

    struct glowing_circle
    {
      ggo::pos2_f _center;
      float _radius;
      float _inner_size;
      float _outter_size;
      float _opacity;
      ggo::vec2_f _velocity;
    };

  private:

    void preprocess_frame(int frame_index, uint32_t cursor_events, ggo::pos2_i cursor_pos, float time_step) override;
    void render_tile(void * buffer, int frame_index, const ggo::rect_int & clipping) override;
    bool finished(int frame_index) const override;

    template <ggo::image_format format>
    void render_tile_t(void * buffer, const ggo::rect_int & clipping) const;

  private:

    std::vector<glowing_circle> _circles;
    float _r_angle;
    float _g_angle;
    float _b_angle;
    float _dr_angle;
    float _dg_angle;
    float _db_angle;
  };
}

#endif