#ifndef __GGO_ARTIST_ABC__
#define __GGO_ARTIST_ABC__

#include <kernel/ggo_kernel.h>
#include <kernel/ggo_ease.h>
#include <kernel/ggo_rect_int.h>
#include <kernel/ggo_size.h>
#include <kernel/math/shapes_2d/ggo_shapes2d.h>
#include <string>
#include <memory>

namespace ggo
{
  class artist
  {
  public:

    artist(int width, int height);

    // Static methods.
    static ggo::pos2_f  map_fill(const ggo::pos2_f & point, float inf, float sup, int render_width, int render_height);

    static ggo::pos2_f  get_random_point(int render_width, int render_height);
    static ggo::pos2_f  get_random_point(float margin, int render_width, int render_height);
    static ggo::pos2_f	get_random_point(float margin_left, float margin_right, float margin_top, float margin_bottom, int render_width, int render_height);

    static ggo::pos2_f  horz_mirror(const ggo::pos2_f & p, int render_height);
    static ggo::pos2_f  vert_mirror(const ggo::pos2_f & p, int render_width);

    // Accessors.
    [[nodiscard]] int         width() const { return _width; }
    [[nodiscard]] int         height() const { return _height; }
    [[nodiscard]] ggo::size   size() const { return { _width, _height }; }
    [[nodiscard]] int         max_size() const { return std::max(_width, _height); }
    [[nodiscard]] int         min_size() const { return std::min(_width, _height); }
    [[nodiscard]] int         pixels_count() const { return _width * _height; }

    [[nodiscard]] ggo::pos2_f get_random_point() const { return get_random_point(_width, _height); }
    [[nodiscard]] ggo::pos2_f get_random_point(float margin) const { return get_random_point(margin, _width, _height); }
    [[nodiscard]] ggo::pos2_f get_random_point(float margin_left, float margin_right, float margin_top, float margin_bottom) const {
      return get_random_point(margin_left, margin_right, margin_top, margin_bottom, _width, _height);
    }

    // Mapping onto static methods.
    [[nodiscard]] ggo::pos2_f	 map_fill(const ggo::pos2_f & point, float inf, float sup) const;

    [[nodiscard]] float        map_fit(float value, float inf, float sup) const;
    [[nodiscard]] ggo::pos2_f  map_fit(const ggo::pos2_f & point, float inf, float sup) const;
    [[nodiscard]] ggo::rect_f	 map_fit(ggo::rect_f rect, float inf, float sup) const;
    [[nodiscard]] ggo::disc_f	 map_fit(ggo::disc_f disc, float inf, float sup) const;

    [[nodiscard]] ggo::disc_d	 map_fit(ggo::disc_d disc, double inf, double sup) const;

    [[nodiscard]] ggo::pos2_f  center() const;

    [[nodiscard]] ggo::pos2_f  horz_mirror(const ggo::pos2_f & p) const { return horz_mirror(p, _height); }
    [[nodiscard]] ggo::pos2_f  vert_mirror(const ggo::pos2_f & p) const { return vert_mirror(p, _width); }

  private:

    const int	_width;
    const int	_height;
  };
}

#endif
