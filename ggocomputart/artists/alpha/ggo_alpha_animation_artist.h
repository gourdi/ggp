#ifndef __GGO_ALPHA_ANIM_ARTIST__
#define __GGO_ALPHA_ANIM_ARTIST__

#include "ggo_animation_artist_abc.h"
#include <memory>

namespace ggo
{
  class alpha_animation_artist : public animation_artist_abc
  {
  public:

          alpha_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt);

    void	init() override;
    bool  update() override;
    void  render_frame(void * buffer, const ggo::rect_int & clipping) override;

    int		get_items_count() const { return static_cast<int>(_items.size()); }

  private:

    class line
    {
    public:

            line(const ggo::pos2f & center, float angle, float inner_radius, float outter_radius, const ggo::color_8u & color);

      bool	update(int width, int height);
      void	draw(void * buffer, int width, int height) const;

      float get_start_factor() const;

    private:

      int				    _counter;
      ggo::pos2f    _center;
      float			    _angle;
      float			    _angle_offset;
      float			    _inner_radius;
      float			    _outter_radius;
      ggo::color_8u _color;
      ggo::pos2f    _p1;
      ggo::pos2f    _p2;
      float 		    _opacity;
      float			    _blur;
    };

    class item
    {
    public:

                        item(const ggo::pos2f & center, float inner_radius, float outter_radius, float hue, float sat, float val);

      bool						  update(int width, int height);
      void						  draw(void * buffer, int width, int height) const;
      const ggo::pos2f  get_center() const { return _center; };

    private:

      std::vector<line> _lines;
      ggo::pos2f        _center;
    };

    class oscillo
    {
    public:

            oscillo(float y, float dy);

      float	y() const { return _y; };
      bool	update();
      void	draw(void * buffer, int width, int height) const;

    private:

      float	_y;
      float	_dy;
      float _opacity;
      int		_threshold;
      int		_counter;
    };

  private:

    void	add_new_item();

  private:

    std::vector<item>		  _items;
    std::vector<oscillo>  _oscillos;
    int							      _remaining_counter;
    int							      _creation_counter;
    ggo::color_8u					_bkgd_color1;
    ggo::color_8u					_bkgd_color2;
    ggo::color_8u					_bkgd_color3;
    ggo::color_8u					_bkgd_color4;
  };
}

#endif
