#ifndef __GGO_WAKENDA_ANIMATION_ARTIST__
#define __GGO_WAKENDA_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include <2d/ggo_image.h>

namespace ggo
{
  class wakenda_animation_artist : public animation_artist_abc
  {
  public:

    wakenda_animation_artist(int width, int height, int line_step, ggo::image_format format);

  private:

    void  render_frame(void * buffer, int frame_index, bool & finished) override;

    void  process_bkgd(void * buffer) const;
    void  process_points(void * buffer, int frame_index);


  private:

    struct point
    {
      point(float x, float y, const ggo::rgb_8u & color) : _pos(x, y), _color(color) {}
      point(const ggo::pos2f & p, const ggo::rgb_8u & color) : _pos(p), _color(color) {}

      ggo::pos2f _pos;
      ggo::rgb_8u _color;
      float _opacity = 0.f;
      int _counter = 0;
    };

    struct transform
    {
      float _scale;
      float _dx;
      float _dy;
      float _powx;
      float _powy;
    };

    struct transform_interpolation
    {
      transform _start;
      transform _end;
    };

  private:

    float _hue;
    ggo::image _bkgd_image;
    std::vector<point> _points;
    std::array<transform_interpolation, 4> _transforms_x;
    std::array<transform_interpolation, 4> _transforms_y;
  };
}

#endif
