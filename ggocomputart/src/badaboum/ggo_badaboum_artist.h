#ifndef __GGO_BADABOUM_ARTIST__
#define __GGO_BADABOUM_ARTIST__

#include <ggo_artist.h>
#include <2d/ggo_image.h>
#include <physics/ggo_physics_world.h>

namespace ggo
{
  class badaboum_artist : public artist
  {
  public:

    badaboum_artist(int width, int height, int line_step, ggo::image_format format, ggo::pixel_sampling sampling);

    void  preprocess_frame(int frame_index, uint32_t cursor_events, ggo::pos2_i cursor_pos, float time_step);
    void  render_tile(void * buffer, int frame_index, const ggo::rect_int & clipping);
    
    static int frames_count() { return 400; }

  private:

    ggo::rgb_8u new_box_color() const;

    template <ggo::image_format format>
    void  paint_bkgd(const std::vector<ggo::oriented_box_f> & floors);

    template <ggo::image_format format>
    void  render_tile(void * buffer, const ggo::rect_int & clipping) const;

    template <ggo::image_format format, ggo::pixel_sampling sampling>
    void  render_boxes(void * buffer, const ggo::rect_int & clipping) const;

    template <ggo::image_format format, ggo::pixel_sampling sampling>
    void  render_box(void * buffer, int line_step, const ggo::oriented_box_f & box, ggo::rgb_8u color, const ggo::rect_int & clipping) const;

    ggo::oriented_box_f map_fit(const ggo::oriented_box_f & box) const;

    struct body
    {
      const rigid_body * _body;
      ggo::rgb_8u _color;
    };
    std::vector<body> _bodies;

    struct box
    {
      box(const ggo::oriented_box_f & box, ggo::rgb_8u color) : _box(box), _color(color) {}
      ggo::oriented_box_f _box;
      ggo::rgb_8u _color;
    };
    std::vector<box> _boxes;

    ggo::physics_world _world;
    float _new_bodies_count = 0.f;
    float _hues[2];
    ggo::pixel_sampling _sampling;
    const float _thickness;
    ggo::image _bkgd;
  };
}

#endif

