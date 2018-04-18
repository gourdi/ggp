#ifndef __GGO_STORNI_ANIMATION_ARTIST__
#define __GGO_STORNI_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include <ggo_sampling_paint.h>
#include <kernel/memory/ggo_buffer.h>

namespace ggo
{
  class storni_animation_artist : public animation_artist_abc
  {
  public:

          storni_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt);

    struct storni
    {
      storni() = default;
      storni(const ggo::pos2f & pos, const ggo::vec2f & vel) : _pos(pos), _vel(vel) {}

      void avoid_obstacles(int width, int height, const std::vector<ggo::pos2f> & obstacles);
      void avoid_stornis(int width, int height, const std::vector<storni> & stornis, float influence_hypot, float weight);
      void avoid_borders(int width, int height, float margin);
      void clamp_velocity(float velocity_hypot_min, float velocity_hypot_max);

      ggo::pos2f  _pos;
      ggo::vec2f  _vel;
    };

  private:

    void  init_animation() override;
    bool  prepare_frame() override;
    void  render_frame(void * buffer, const ggo::rect_int & clipping) override;

    void update_predators(float velocity_hypot_min, float velocity_hypot_max, float border_margin);
    void update_stornis(float velocity_hypot_min, float velocity_hypot_max, float border_margin);

    template <ggo::pixel_buffer_format pbf> void blit_background(void * buffer, const ggo::rect_int & clipping) const;
    template <ggo::pixel_buffer_format pbf, ggo::sampling smp> void paint_stornies(void * buffer, const ggo::rect_int & clipping);
    template <ggo::pixel_buffer_format pbf, ggo::sampling smp> void paint_predators(void * buffer, const ggo::rect_int & clipping) const;
    template <ggo::pixel_buffer_format pbf> void paint_obstacles(void * buffer, const ggo::rect_int & clipping, int frame_index) const;

    float get_obstacle_hypot() const { return ggo::square(0.1f) * get_width() * get_height(); }

    int                           _frame_index;
    float                         _hue;
    std::vector<storni>           _stornis;
    std::vector<storni>           _predators;
    std::vector<ggo::pos2f>       _obstacles;
    std::vector<uint8_t>          _stornis_buffer;
    std::array<ggo::color_8u, 4>  _background_colors;
    std::vector<uint8_t>          _background;
  };
}

#include "ggo_storni_animation_artist.imp.h"

#endif