#ifndef __GGO_ALPHA_ANIM_ARTIST__
#define __GGO_ALPHA_ANIM_ARTIST__

#include "ggo_animation_artist_abc.h"
#include <memory>
#include <2d/ggo_image.h>

namespace ggo
{
  class alpha_animation_artist : public animation_artist_abc
  {
  public:

          alpha_animation_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);

    void  render_frame(void * buffer, int frame_index, float time_step, bool & finished) override;

    int		get_items_count() const { return static_cast<int>(_items.size()); }

  private:

    class line
    {
    public:

            line(const ggo::pos2_f & center, float angle, float inner_radius, float outter_radius, const ggo::rgb_8u & color);

      bool	update(int width, int height);
      void	draw(image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up> & img) const;

      float get_start_factor() const;

    private:

      int				    _counter = 0;
      ggo::pos2_f   _center;
      float			    _angle = 0.f;
      float			    _angle_offset = 0.f;
      float			    _inner_radius = 0.f;
      float			    _outter_radius = 0.f;
      ggo::rgb_8u   _color;
      ggo::pos2_f   _p1;
      ggo::pos2_f   _p2;
      float 		    _opacity = 0.f;
      float			    _blur = 0.f;
    };

    class item
    {
    public:

                        item(const ggo::pos2_f & center, float inner_radius, float outter_radius, float hue, float sat, float val);

      bool						  update(int width, int height);
      void						  draw(image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up> & img) const;
      const ggo::pos2_f get_center() const { return _center; };

    private:

      std::vector<line> _lines;
      ggo::pos2_f        _center;
    };

    class oscillo
    {
    public:

            oscillo(float y, float dy);

      float	y() const { return _y; };
      bool	update();
      void	draw(image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up> & img) const;

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
    int							      _remaining_counter = 0;
    int							      _creation_counter = 0;
    ggo::rgb_8u  					_bkgd_color1;
    ggo::rgb_8u  					_bkgd_color2;
    ggo::rgb_8u  					_bkgd_color3;
    ggo::rgb_8u  					_bkgd_color4;
  };
}

#endif
