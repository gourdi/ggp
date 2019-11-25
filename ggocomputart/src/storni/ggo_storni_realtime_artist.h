#ifndef __GGO_STORNI_REALTIME_ARTIST__
#define __GGO_STORNI_REALTIME_ARTIST__

#include <ggo_realtime_artist.h>
#include <kernel/memory/ggo_array.h>
#include <kernel/math/ggo_pixel_sampling.h>

namespace ggo
{
  class storni_realtime_artist : public realtime_artist
  {
  public:

    storni_realtime_artist(
      int width, int height, int line_byte_step,
      ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order,
      ggo::ratio fps);

    struct storni
    {
      storni() = default;
      storni(const ggo::pos2_f & pos, const ggo::vec2_f & vel, const ggo::rgb_8u & c) : _pos(pos), _vel(vel), _color(c) {}

      void avoid_obstacles(int width, int height, const std::vector<ggo::pos2_f> & obstacles);
      void avoid_stornis(int width, int height, const std::vector<storni> & stornis, float influence_hypot, float weight);
      void avoid_cursor(int width, int height, ggo::pos2_i cursor_pos, float influence_hypot, float weight);
      void avoid_borders(int width, int height, float margin);
      void clamp_velocity(float velocity_hypot_max);

      ggo::vec2_f _vel;
      ggo::pos2_f _pos;
      ggo::rgb_8u _color;
    };

  private:

    void  preprocess_frame(void * buffer, uint32_t cursor_events, ggo::pos2_i cursor_pos) override;
    void  render_tile(void * buffer, const ggo::rect_int & clipping) override;
    bool  finished() override;

    float get_velocity_hypot_max() const { return ggo::square(to<float>(min_size() / (10 * _fps))); }

    void update_predators(float velocity_hypot_max, float border_margin);
    void update_stornis(float velocity_hypot_max, float border_margin, ggo::pos2_i cursor_pos);

    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order> void fade_background_to_white(const ggo::rect_int & clipping);
    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, ggo::pixel_sampling sampling> void paint_stornies_background(const ggo::rect_int & clipping);
    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order> void blit_background(void * buffer, const ggo::rect_int & clipping) const;
    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, ggo::pixel_sampling sampling> void paint_stornies(void * buffer, const std::vector<storni> & stornis, float size, const ggo::rect_int & clipping) const;
    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order> void paint_obstacles(void * buffer, const ggo::rect_int & clipping, ggo::ratio elapsed_time) const;

    static float get_obstacle_hypot(int width, int height) { return ggo::square(0.05f) * width * height; }

    float                     _hue;
    std::vector<storni>       _stornis;
    std::vector<storni>       _predators;
    std::vector<ggo::pos2_f>  _obstacles;
    std::vector<uint8_t>      _background_buffer;
    ggo::ratio                _fps;
    ggo::ratio                _elapsed_time = { 0, 1 };
  };
}

#endif