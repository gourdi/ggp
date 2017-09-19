#ifndef __GGO_ARTIST_ABC__
#define __GGO_ARTIST_ABC__

#include <ggo_kernel.h>
#include <ggo_pixel_buffer.h>
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

    artist(int width, int height) : _width(width), _height(height) {}

    // Static methods.
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

    // Accessors.
    int                       get_width() const { return _width; }
    int                       get_height() const { return _height; }
    int                       get_max_size() const { return std::max(_width, _height); }
    int                       get_min_size() const { return std::min(_width, _height); }
    int                       get_pixels_count() const { return _width * _height; }

    ggo::pos2f                get_random_point() const { return get_random_point(_width, _height); }
    ggo::pos2f                get_random_point(float margin) const { return get_random_point(margin, _width, _height); }
    ggo::pos2f                get_random_point(float margin_left, float margin_right, float margin_top, float margin_bottom) const {
      return get_random_point(margin_left, margin_right, margin_top, margin_bottom, _width, _height);
    }

    // Mapping onto static methods.
    ggo::pos2f	              map_fill(const ggo::pos2f & point, float inf, float sup) const;

    float                     map_fit(float value, float inf, float sup) const;
    ggo::pos2f	              map_fit(const ggo::pos2f & point, float inf, float sup) const;
    void                      map_fit(ggo::rect_float & rect, float inf, float sup) const;
    void                      map_fit(ggo::disc_float & disc, float inf, float sup) const;

    ggo::pos2f                get_center() const;

    ggo::pos2f                horz_mirror(const ggo::pos2f & p) const { return horz_mirror(p, _height); }
    ggo::pos2f                vert_mirror(const ggo::pos2f & p) const { return vert_mirror(p, _width); }

    // Template functions.
    template <typename pixel_func_t>
    void for_each_pixel(pixel_func_t pixel_func) const
    {
      for (int y = 0; y < get_height(); ++y)
      {
        for (int x = 0; x < get_width(); ++x)
        {
          pixel_func(x, y);
        }
      }
    }

    template <typename data_t>
    const data_t & get2d(const data_t * buffer, int x, int y) const
    {
      return buffer[y * get_height() + x];
    }

    template <typename data_t>
    const data_t & get2d_loop(const data_t * buffer, int x, int y) const
    {
      return ggo::get2d_loop<ggo::direction::up>(buffer, x, y, get_width(), get_height(), sizeof(data_t) * get_width());
    }

    template <typename data_t>
    void set2d(data_t * buffer, int x, int y, const data_t & v) const
    {
      buffer[y * get_height() + x] = v;
    }

  private:

    const int	_width;
    const int	_height;
  };
}

#endif
