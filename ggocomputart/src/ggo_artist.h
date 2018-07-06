#ifndef __GGO_ARTIST_ABC__
#define __GGO_ARTIST_ABC__

#include <kernel/ggo_kernel.h>
#include <kernel/ggo_ease.h>
#include <kernel/ggo_rect_int.h>
#include <kernel/math/shapes_2d/ggo_shapes2d.h>
#include <2d/ggo_image_format.h>
#include <2d/ggo_color.h>
#include <string>
#include <memory>

namespace ggo
{
  class artist
  {
  public:

    artist(int width, int height, int line_step, image_format format) : _width(width), _height(height), _line_step(line_step), _format(format) {}

    // Static methods.
    static ggo::pos2f	map_fill(const ggo::pos2f & point, float inf, float sup, int render_width, int render_height);

    static ggo::pos2f get_random_point(int render_width, int render_height);
    static ggo::pos2f get_random_point(float margin, int render_width, int render_height);
    static ggo::pos2f	get_random_point(float margin_left, float margin_right, float margin_top, float margin_bottom, int render_width, int render_height);

    static ggo::pos2f horz_mirror(const ggo::pos2f & p, int render_height);
    static ggo::pos2f vert_mirror(const ggo::pos2f & p, int render_width);

    // Accessors.
    int                       width() const { return _width; }
    int                       height() const { return _height; }
    ggo::size                 size() const { return { _width, _height }; }
    int                       max_size() const { return std::max(_width, _height); }
    int                       min_size() const { return std::min(_width, _height); }
    int                       pixels_count() const { return _width * _height; }
    int                       line_step() const { return _line_step; }
    ggo::image_format         format() const { return _format; }


    ggo::pos2f                get_random_point() const { return get_random_point(_width, _height); }
    ggo::pos2f                get_random_point(float margin) const { return get_random_point(margin, _width, _height); }
    ggo::pos2f                get_random_point(float margin_left, float margin_right, float margin_top, float margin_bottom) const {
      return get_random_point(margin_left, margin_right, margin_top, margin_bottom, _width, _height);
    }

    // Mapping onto static methods.
    ggo::pos2f	              map_fill(const ggo::pos2f & point, float inf, float sup) const;

    float                     map_fit(float value, float inf, float sup) const;
    ggo::pos2f                map_fit(const ggo::pos2f & point, float inf, float sup) const;
    void	                    map_fit(ggo::rect_float & rect, float inf, float sup) const;
    void	                    map_fit(ggo::disc_float & disc, float inf, float sup) const;

    void	                    map_fit(ggo::disc_double & disc, double inf, double sup) const;

    ggo::pos2f                get_center() const;

    ggo::pos2f                horz_mirror(const ggo::pos2f & p) const { return horz_mirror(p, _height); }
    ggo::pos2f                vert_mirror(const ggo::pos2f & p) const { return vert_mirror(p, _width); }

  private:

    const int	_width;
    const int	_height;
    const int _line_step;
    const ggo::image_format _format;
  };
}

#endif
