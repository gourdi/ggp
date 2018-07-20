#ifndef __GGO_DEMECO_ANIMATION_ARTIST__
#define __GGO_DEMECO_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include <vector>
#include <memory>
#include <2d/ggo_image.h>

namespace ggo
{
  class demeco_animation_artist : public animation_artist_abc
  {
  public:

    demeco_animation_artist(int width, int height, int line_step, ggo::image_format format);

  private:

    void  render_frame(void * buffer, int frame_index, bool & finished) override;

  private:

    using palette = std::array<ggo::color_8u, 8>;

    struct demeco
    {
      demeco(const ggo::pos2f & pos, float radius, int counter) : _pos(pos), _radius(radius), _counter(counter) {}

      virtual bool render(ggo::image & image) const = 0;
      ggo::pos2f _pos;
      float _radius;
      int _counter;
    };

    struct demeco1 : public demeco
    {
      demeco1(const ggo::pos2f & pos, float radius, int counter, const palette & palette);
      bool render(ggo::image & image) const override;

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
      demeco2(const ggo::pos2f & pos, float radius, int counter, const palette & palette);
      bool render(ggo::image & image) const override;

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
      demeco3(const ggo::pos2f & pos, float radius, int counter, const palette & palette);
      bool render(ggo::image & image) const override;

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

    ggo::image _render_image;
    ggo::image _shadow_image;
    palette _palette;
    std::vector<std::unique_ptr<demeco>> _demecos;
  };
}

#endif