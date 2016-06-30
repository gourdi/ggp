#ifndef __GGO_LAGAUDE_ANIMATION_ARTIST__
#define __GGO_LAGAUDE_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include <ggo_animator.h>
#include <ggo_animate_abc.h>
#include <ggo_linear_path.h>
#include <ggo_array.h>
#include <ggo_random_interpolator_abc.h>

class ggo_lagaude_animation_artist : public ggo_animation_artist_abc
{
public:
	
        ggo_lagaude_animation_artist(int render_width, int render_height);

	void	init_sub() override;
	bool	render_next_frame_sub(uint8_t * buffer, int frame_index) override;
	
private:
	
	//////////////////////////////////////////////////////////////
	class ggo_particle : public ggo_path_animate_abc
	{
	public:
		
          ggo_particle(const ggo::pos2f & pos, ggo_linear_path * path);
		
    bool  update(uint8_t * output_buffer, uint8_t * bkgd_buffer, int width, int height, int counter, const ggo::pos2f & pos) override;
		
	public:
		
		float		    _angle;
		float		    _dangle;
		float		    _radius;
		float		    _dradius;
		float		    _opacity;
		ggo::color	_color;
	};

	//////////////////////////////////////////////////////////////
	class ggo_angle_generator : public ggo::random_interpolator_abc<float, float>
	{
		void	get_random_data(float & data, float & dt) override;
	};
	
	//////////////////////////////////////////////////////////////
	class ggo_scale_animate_abc : public ggo_path_animate_abc
	{
	public:
  
					ggo_scale_animate_abc(const ggo::pos2f & pos, ggo_path_abc * path, float scale) : ggo_path_animate_abc(pos, path), _scale(scale) {};

		float	get_scale() const { return _scale; };
				
	protected:
		
		float	_scale;
	};
	
	//////////////////////////////////////////////////////////////
	class ggo_seed : public ggo_scale_animate_abc
	{
	public:
		
          ggo_seed(const ggo::pos2f & pos, ggo_path_abc * path, float scale, float hue);
                        
    bool	update(uint8_t * output_buffer, uint8_t * bkgd_buffer, int width, int height, int counter, const ggo::pos2f & pos) override;
    
	private:

		ggo::array<ggo_angle_generator, 1>  _angle_generators;
		ggo_animator					              _particles_animator;
		int								                  _life;
		ggo::color					                _color;
		float							                  _dangle;
	};

	//////////////////////////////////////////////////////////////
	class ggo_dust : public ggo_scale_animate_abc
	{
	public:
		
          ggo_dust(const ggo::pos2f & pos, ggo_path_abc * path, float scale) : ggo_scale_animate_abc(pos, path, scale) {};
		
    bool	update(uint8_t * output_buffer, uint8_t * bkgd_buffer, int width, int height, int counter, const ggo::pos2f & pos) override;
		
	public:

		float	_val;
		float	_radius;
		float	_width;
	};
	
	//////////////////////////////////////////////////////////////
	class ggo_sinuoid_path : public ggo_path_abc
	{
	public:

		ggo::pos2f	get_pos(int counter) override { return ggo::pos2f(counter * _dx, _amplitude * std::sin(counter * _frequency)); };
		
	public:

		float	_dx;
		float	_frequency;
		float	_amplitude;
	};
	
	//////////////////////////////////////////////////////////////
	struct ggo_bkgd_disc
	{
		ggo::pos2f	_pos;
		ggo::vec2f	_vel;
		float			  _radius;
	};
	
private:
	
	void	insert_scale_animator(ggo_scale_animate_abc * scale_animator);

private:

	ggo_animator				        _animator;
	std::vector<ggo_bkgd_disc>  _bkgd_discs;
	float						            _hue;
};

#endif
