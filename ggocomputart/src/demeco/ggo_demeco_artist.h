#ifndef __GGO_DEMECO_ARTIST__
#define __GGO_DEMECO_ARTIST__

#include <vector>
#include <memory>
#include <kernel/math/ggo_pixel_sampling.h>
#include <2d/ggo_image.h>
#include <2d/paint/ggo_paint_shape_abc.h>
#include <2d/paint/ggo_multi_scale_paint.h>

namespace ggo
{
  template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, ggo::pixel_sampling sampling>
  class demeco_artist : public artist
  {
  public:

    demeco_artist(int width, int height);

    bool  update();
    void  render_tile(void * buffer, int line_byte_step, int frame_index, const ggo::rect_int & clipping);

  private:

    static typename pixel_type_traits<pixel_type>::color_t from_8u(const ggo::rgb_8u & c);

  private:

    using palette_t = std::array<ggo::rgb_8u, 8>;
    using scene_t = scene2d<typename pixel_type_traits<pixel_type>::color_t>;

    struct demeco
    {
      demeco(const ggo::pos2_f & pos, float radius, int counter) : _pos(pos), _radius(radius), _counter(counter) {}

      virtual void get_paint_shapes(scene_t & scene) const = 0;
      virtual bool finished() const = 0;

      ggo::pos2_f _pos;
      float _radius;
      int _counter;
    };

    struct demeco1 : public demeco
    {
      demeco1(const ggo::pos2_f & pos, float radius, int counter, const palette_t & palette);

      void get_paint_shapes(scene_t & scene) const override;
      bool finished() const override;

      static const int _angle_counter_max = 50;
      static const int _fade_in_counter_max = 40;

      struct arc
      {
        arc(ggo::rgb_8u c, float radius, float width, float start_angle, bool ccw)
        :
        _color(c), _radius(radius), _width(width), _start_angle(start_angle), _ccw(ccw) {}

        ggo::rgb_8u _color;
        float _radius;
        float _width;
        float _start_angle;
        bool _ccw;
      };
      std::vector<arc> _arcs;
    };

    struct demeco2 : public demeco
    {
      demeco2(const ggo::pos2_f & pos, float radius, int counter, const palette_t & palette);

      void get_paint_shapes(scene_t & scene) const override;
      bool finished() const override;

      static const int _anim1_counter_start = 0;
      static const int _anim1_counter_end = 10;
      static const int _anim2_counter_start = 40;
      static const int _anim2_counter_end = 50;
      static const int _anim3_counter_start = 80;
      static const int _anim3_counter_end = 90;

      struct animation
      {
        animation(ggo::rgb_8u c, float size, float end_angle) : _color(c), _size(size), _end_angle(end_angle) {}

        ggo::rgb_8u _color;
        float _size;
        float _end_angle;
      };
      std::vector<animation> _animations;
      float _init_angle;
    };

    struct demeco3 : public demeco
    {
      demeco3(const ggo::pos2_f & pos, float radius, int counter, const palette_t & palette);

      void get_paint_shapes(scene_t & scene) const override;
      bool finished() const override;

      const int _layer_delay = 30;
      const int _layers_count = 4;
      const int _anim1_duration = 10;
      const int _anim2_duration = 10;

      struct peak
      {
        peak(float angle, float aperture, float radius_inf, float radius_sup, ggo::rgb_8u c, int delay)
        :
        _angle(angle), _aperture(aperture), _radius_inf(radius_inf), _radius_sup(radius_sup), _color(c), _delay(delay) {}

        float _angle;
        float _aperture;
        float _radius_inf;
        float _radius_sup;
        ggo::rgb_8u _color;
        int _delay;
      };
      std::vector<peak> _peaks;
    };

    palette_t _palette;
    std::vector<std::unique_ptr<demeco>> _demecos;
    scene2d<typename pixel_type_traits<pixel_type>::color_t> _background_scene;
    scene2d<typename pixel_type_traits<pixel_type>::color_t> _active_scene;
    ggo::image_t<pixel_type, memory_lines_order> _background_image;
  };
}

#include "ggo_demeco_artist.imp.h"

#endif