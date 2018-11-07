#ifndef __GGO_WAKENDA_REALTIME_ARTIST__
#define __GGO_WAKENDA_REALTIME_ARTIST__

#include <ggo_realtime_artist_abc.h>
#include <2d/ggo_image.h>

namespace ggo
{
  class wakenda_realtime_artist : public realtime_artist_abc
  {
  public:

    wakenda_realtime_artist(int width, int height, int line_step, ggo::image_format format);

  private:

    void preprocess_frame(int frame_index) override;
    void render_tile(void * buffer, int frame_index, const ggo::rect_int & clipping) override;
    bool finished(int frame_index) const override;

    template <ggo::image_format format> void  copy_bkgd(void * buffer, const ggo::rect_int & clipping) const;
    template <ggo::image_format format> void  process_bkgd(void * buffer, const ggo::rect_int & clipping) const;
    template <ggo::image_format format> void  paint_points(void * buffer, const ggo::rect_int & clipping) const;

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

    struct paint_point
    {
      paint_point(const ggo::pos2f & p, const ggo::rgb_8u & color, float opacity) : _pos(p), _color(color), _opacity(opacity) {}

      const ggo::pos2f _pos;
      const ggo::rgb_8u _color;
      const float _opacity;
    };

  private:

    float _hue;
    ggo::image _bkgd_image;
    std::vector<point> _points;
    std::array<transform_interpolation, 4> _transforms_x;
    std::array<transform_interpolation, 4> _transforms_y;
    std::vector<paint_point> _paint_points;
  };
}

#endif
