#ifndef __GGO_DEMECO_ARTIST__
#define __GGO_DEMECO_ARTIST__

#include <vector>
#include <memory>
#include <2d/ggo_image.h>
#include <2d/paint/ggo_pixel_sampling.h>
#include <2d/paint/ggo_paint_shape_abc.h>

namespace ggo
{
  template <ggo::image_format format, ggo::sampling>
  class demeco_artist
  {
  public:

    demeco_artist(int width, int height);

    bool  update();
    void  render_tile(void * buffer, int line_byte_step, int frame_index, const ggo::rect_int & clipping);

  private:

    int   min_size() const { return std::min(_width, _height); }
    pos2f center() const { return { static_cast<float>(_width - 1) / 2, static_cast<float>(_height - 1) / 2 }; }

    static typename image_format_traits<format>::color_t from_8u(const ggo::color_8u & c);

  private:

    using palette_t = std::array<ggo::color_8u, 8>;
    using paint_shapes_t = std::vector<std::unique_ptr<ggo::paint_shape_abc<float, typename ggo::image_format_traits<format>::color_t>>>;

    struct demeco
    {
      demeco(const ggo::pos2f & pos, float radius, int counter) : _pos(pos), _radius(radius), _counter(counter) {}

      virtual typename paint_shapes_t get_paint_shapes() const = 0;
      virtual bool finished() const = 0;

      ggo::pos2f _pos;
      float _radius;
      int _counter;
    };

    struct demeco1 : public demeco
    {
      demeco1(const ggo::pos2f & pos, float radius, int counter, const palette_t & palette);

      typename paint_shapes_t get_paint_shapes() const override;
      bool finished() const override;

      static const int _angle_counter_max = 50;
      static const int _fade_in_counter_max = 40;

      struct arc
      {
        arc(ggo::color_8u c, float radius, float width, float start_angle, bool ccw)
        :
        _color(c), _radius(radius), _width(width), _start_angle(start_angle), _ccw(ccw) {}

        ggo::color_8u _color;
        float _radius;
        float _width;
        float _start_angle;
        bool _ccw;
      };
      std::vector<arc> _arcs;
    };

    struct demeco2 : public demeco
    {
      demeco2(const ggo::pos2f & pos, float radius, int counter, const palette_t & palette);

      typename paint_shapes_t get_paint_shapes() const override;
      bool finished() const override;

      static const int _anim1_counter_start = 0;
      static const int _anim1_counter_end = 10;
      static const int _anim2_counter_start = 40;
      static const int _anim2_counter_end = 50;
      static const int _anim3_counter_start = 80;
      static const int _anim3_counter_end = 90;

      struct animation
      {
        animation(ggo::color_8u c, float size, float end_angle) : _color(c), _size(size), _end_angle(end_angle) {}

        ggo::color_8u _color;
        float _size;
        float _end_angle;
      };
      std::vector<animation> _animations;
      float _init_angle;
    };

    struct demeco3 : public demeco
    {
      demeco3(const ggo::pos2f & pos, float radius, int counter, const palette_t & palette);

      typename paint_shapes_t get_paint_shapes() const override;
      bool finished() const override;

      const int _layer_delay = 30;
      const int _layers_count = 4;
      const int _anim1_duration = 10;
      const int _anim2_duration = 10;

      struct peak
      {
        peak(float angle, float aperture, float radius_inf, float radius_sup, ggo::color_8u c, int delay)
        :
        _angle(angle), _aperture(aperture), _radius_inf(radius_inf), _radius_sup(radius_sup), _color(c), _delay(delay) {}

        float _angle;
        float _aperture;
        float _radius_inf;
        float _radius_sup;
        ggo::color_8u _color;
        int _delay;
      };
      std::vector<peak> _peaks;
    };

    int _width;
    int _height;
    palette_t _palette;
    std::vector<std::unique_ptr<demeco>> _demecos;
    paint_shapes_t _background_paint_shapes;
    paint_shapes_t _active_paint_shapes;
    ggo::static_image<format> _background_image;
  };
}

#include "ggo_demeco_artist.imp.h"

#endif