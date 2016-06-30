#ifndef __GGO_DUPECHECK_ANIMATION_ARTIST__
#define __GGO_DUPECHECK_ANIMATION_ARTIST__

#include "ggo_animation_artist_abc.h"
#include <ggo_animator.h>
#include <ggo_animate_abc.h>

class ggo_dupecheck_animation_artist : public ggo_animation_artist_abc
{
public:

              ggo_dupecheck_animation_artist(int render_width, int render_height);

	void				init_sub() override;
	bool				render_next_frame_sub(uint8_t * buffer, int frame_index) override;

private:
	
	ggo::pos2f	get_position(int frame_index);
	ggo::color	get_color(int frame_index);
	
private:

	class ggo_dupecheck_animate_abc : public ggo_path_animate_abc
	{
	protected:
		
		float		    _opacity;
		ggo::color	_color;
		float		    _radius;
		float		    _dradius;
		
                      ggo_dupecheck_animate_abc(ggo::pos2f pos, ggo_path_abc * path, const ggo::color & color, int render_min_size);
                bool	update(uint8_t * output_buffer, uint8_t * bkgd_buffer, int width, int height, int counter, const ggo::pos2f & pos) override;
        virtual	void  update(uint8_t * buffer, int width, int height, const ggo::pos2f & pos) = 0;
	};

	class ggo_animate1 : public ggo_dupecheck_animate_abc
	{
	public:

            ggo_animate1(const ggo::pos2f & pos, ggo_path_abc * path, const ggo::color & color, int render_min_size);
		void    update(uint8_t * buffer, int width, int height, const ggo::pos2f & pos) override;

	private:

		float	_width;
		float	_distance;
		int	  _count;
	};
	
	class ggo_animate2 : public ggo_dupecheck_animate_abc
	{
	public:

          ggo_animate2(const ggo::pos2f & pos, ggo_path_abc * path, const ggo::color & color, int render_min_size);
		void	update(uint8_t * buffer, int width, int height, const ggo::pos2f & pos) override;

	private:

		float	_width;
	};
	
	class ggo_animate3 : public ggo_dupecheck_animate_abc
	{
	public:

          ggo_animate3(const ggo::pos2f & pos, ggo_path_abc * path, const ggo::color & color, int render_min_size);
     void	update(uint8_t * buffer, int width, int height, const ggo::pos2f & pos) override;

	private:

		int		_shapes_count;
		int		_vertices_count;
		float	_angle;
		float	_dangle;
		float	_angle_shape;
		float	_dangle_shape;
		float	_shape_size;
	};
	
	struct ggo_background_color
	{
		float _y;
		float	_val;
		float	_radius;
		float	_angle;
		float	_dangle;
		float	_var;
	};

	std::vector<ggo_background_color>	_bkgd_colors;
	ggo::color							          _colors[4];
	ggo::pos2f 					              _points[4];
	ggo_animator						          _animator;
};  

#endif

