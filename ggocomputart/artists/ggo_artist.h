#ifndef __GGO_ARTIST_ABC__
#define __GGO_ARTIST_ABC__

#include <ggo_kernel.h>
#include <ggo_buffer_access.h>
#include <ggo_shapes2d.h>
#include <ggo_ease.h>
#include <ggo_color.h>
#include <string>
#include <memory>

namespace ggo
{
  class artist
  {
  public:

    artist(int render_width, int render_height) : _render_width(render_width), _render_height(render_height) {}

    static ggo::pos2f	map_fill(const ggo::pos2f & point, float inf, float sup, int render_width, int render_height);
           
    static float      map_fit(float value, float inf, float sup, int render_width, int render_height);
    static ggo::pos2f	map_fit(const ggo::pos2f & point, float inf, float sup, int render_width, int render_height);
    static void	      map_fit(ggo::rect_float & rect, float inf, float sup, int render_width, int render_height);
    static void	      map_fit(ggo::disc_float & disc, float inf, float sup, int render_width, int render_height);

    static ggo::pos2f get_random_point(int render_width, int render_height);
    static ggo::pos2f get_random_point(float margin, int render_width, int render_height);
    static ggo::pos2f	get_random_point(float margin_left, float margin_right, float margin_top, float margin_bottom, int render_width, int render_height);

    static ggo::pos2f horz_mirror(const ggo::pos2f & p, int render_height);
    static ggo::pos2f vert_mirror(const ggo::pos2f & p, int render_width);

    int get_render_width() const { return _render_width; }
    int get_render_height() const { return _render_height; }
    int get_render_max_size() const { return std::max(_render_width, _render_height); }
    int get_render_min_size() const { return std::min(_render_width, _render_height); }
    int get_pixels_count() const { return _render_width * _render_height; }

    ggo::pos2f get_random_point() const { return get_random_point(_render_width, _render_height); }
    ggo::pos2f get_random_point(float margin) const { return get_random_point(margin, _render_width, _render_height); }
    ggo::pos2f get_random_point(float margin_left, float margin_right, float margin_top, float margin_bottom) const {
      return get_random_point(margin_left, margin_right, margin_top, margin_bottom, _render_width, _render_height);
    }

    ggo::pos2f	map_fill(const ggo::pos2f & point, float inf, float sup) const;

    float       map_fit(float value, float inf, float sup) const;
    ggo::pos2f	map_fit(const ggo::pos2f & point, float inf, float sup) const;
    void        map_fit(ggo::rect_float & rect, float inf, float sup) const;
    void        map_fit(ggo::disc_float & disc, float inf, float sup) const;

    ggo::pos2f get_center() const;

    ggo::pos2f horz_mirror(const ggo::pos2f & p) const { return horz_mirror(p, _render_height); }
    ggo::pos2f vert_mirror(const ggo::pos2f & p) const { return vert_mirror(p, _render_width); }

    template <typename pixel_func_t>
    void for_each_pixel(pixel_func_t pixel_func) const
    {
      for (int y = 0; y < get_render_height(); ++y)
      {
        for (int x = 0; x < get_render_width(); ++x)
        {
          pixel_func(x, y);
        }
      }
    }

    template <typename data_t>
    const data_t & get2d(const data_t * buffer, int x, int y) const
    {
      return ggo::get2d(buffer, x, y, get_render_height(), sizeof(data_t) * get_render_width());
    }

    template <typename data_t>
    const data_t & get2d_loop(const data_t * buffer, int x, int y) const
    {
      return ggo::get2d_loop(buffer, x, y, get_render_width(), get_render_height(), sizeof(data_t) * get_render_width());
    }

    template <typename data_t>
    void set2d(data_t * buffer, int x, int y, const data_t & v) const
    {
      return ggo::set2d(buffer, x, y, get_render_height(), sizeof(data_t) * get_render_width(), v);
    }

  private:

    int	_render_width;
    int	_render_height;
  };
}

#endif
