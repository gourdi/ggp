#ifndef __GGO_ALPHA_ANIM_ARTIST__
#define __GGO_ALPHA_ANIM_ARTIST__

#include "ggo_animation_artist_abc.h"
#include <memory>

class ggo_alpha_anim_artist : public ggo_animation_artist_abc
{
public:

        ggo_alpha_anim_artist(int render_width, int render_height);
	
	void	init_sub() override;
	bool	render_next_frame_sub(uint8_t * buffer, int frame_index) override;
	
	int		get_items_count() const { return static_cast<int>(_items.size()); }
	
private:

	class ggo_line
	{		
	public:

          ggo_line(const ggo::pos2f & center, float angle, float inner_radius, float outter_radius, const ggo::color & color);
		
		bool	update(int width, int height);
		void	draw(uint8_t * buffer, int width, int height) const;
        
    float get_start_factor() const;

	private:
		
		int				  _counter;
		ggo::pos2f  _center;
		float			  _angle;
		float			  _angle_offset;
		float			  _inner_radius;
		float			  _outter_radius;
		ggo::color  _color;
    ggo::pos2f  _p1;
    ggo::pos2f  _p2;
		float			  _opacity;
		float			  _blur;
	};
	
	class ggo_item
	{
	public:
		
                      ggo_item(const ggo::pos2f & center, float inner_radius, float outter_radius, float hue, float sat, float val);
									
		bool						  update(int width, int height);
		void						  draw(uint8_t * buffer, int width, int height) const;
		const ggo::pos2f  get_center() const { return _center; };
		
	private:
		
		std::vector<ggo_line>	_lines;
		ggo::pos2f		        _center;
	};
	
	class ggo_oscillo
	{
	public:
		
          ggo_oscillo(float y, float dy);
		
		float	y() const { return _y; };
		bool	update();
		void	draw(uint8_t * buffer, int width, int height) const;
		
	private:
		
		float	_y;
		float	_dy;
		float	_opacity;
		int		_threshold;
		int		_counter;
	};

private:
	
	void	add_new_item();
	
private:

	std::vector<ggo_item>		  _items;
	std::vector<ggo_oscillo>  _oscillos;
	int							          _remaining_counter;
	int							          _creation_counter;
	ggo::color					      _bkgd_color1;
	ggo::color					      _bkgd_color2;
	ggo::color					      _bkgd_color3;
	ggo::color					      _bkgd_color4;
};

#endif
