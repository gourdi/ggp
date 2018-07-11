#ifndef __GGO_STORNI_REALTIME_ARTIST__
#define __GGO_STORNI_REALTIME_ARTIST__

#include <ggo_realtime_artist_abc.h>
#include <kernel/memory/ggo_array.h>
#include <2D/paint/ggo_pixel_sampling.h>

namespace ggo
{
  class storni_realtime_artist : public fixed_frames_count_realtime_artist_abc
  {
  public:

    storni_realtime_artist(int width, int height, int line_step, ggo::image_format format);

    struct storni
    {
      storni() = default;
      storni(const ggo::pos2f & pos, const ggo::vec2f & vel, const ggo::color_8u & c) : _pos(pos), _vel(vel), _color(c) {}

      void avoid_obstacles(int width, int height, const std::vector<ggo::pos2f> & obstacles);
      void avoid_stornis(int width, int height, const std::vector<storni> & stornis, float influence_hypot, float weight);
      void avoid_borders(int width, int height, float margin);
      void clamp_velocity(float velocity_hypot_max);

      ggo::vec2f    _vel;
      ggo::pos2f    _pos;
      ggo::color_8u _color;
    };

  private:

    void  preprocess_frame(int frame_index) override;
    void  render_tile(void * buffer, int frame_index, const ggo::rect_int & clipping) override;
    int   frames_count() const override { return 800; }

    float get_velocity_hypot_max() const { return ggo::square(0.004f * min_size()); }

    void update_predators(float velocity_hypot_max, float border_margin);
    void update_stornis(float velocity_hypot_max, float border_margin);

    template <ggo::image_format format> void fade_background_to_white(const ggo::rect_int & clipping);
    template <ggo::image_format format, ggo::sampling smp> void paint_stornies_background(const ggo::rect_int & clipping);
    template <ggo::image_format format> void blit_background(void * buffer, const ggo::rect_int & clipping) const;
    template <ggo::image_format format, ggo::sampling smp> void paint_stornies(void * buffer, const std::vector<storni> & stornis, float size, const ggo::rect_int & clipping) const;
    template <ggo::image_format format> void paint_obstacles(void * buffer, const ggo::rect_int & clipping, int frame_index) const;

    static float get_obstacle_hypot(int width, int height) { return ggo::square(0.05f) * width * height; }

    float                   _hue;
    std::vector<storni>     _stornis;
    std::vector<storni>     _predators;
    std::vector<ggo::pos2f> _obstacles;
    std::vector<uint8_t>    _background_buffer;
  };
}

#include "ggo_storni_animation_artist.imp.h"

#endif