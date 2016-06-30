#ifndef __GGO_TOUTOUYOUTOU_ANIM_ARTIST__
#define __GGO_TOUTOUYOUTOU_ANIM_ARTIST__

// References:
// http://imdoingitwrong.wordpress.com/2010/12/14/why-my-fluids-dont-flow/
// http://cg.informatik.uni-freiburg.de/publications/2007_SCA_SPH.pdf

#include "ggo_animation_artist_abc.h"

class ggo_toutouyoutou_anim_artist : public ggo_static_background_animation_artist_abc
{
public:

	ggo_toutouyoutou_anim_artist(int render_width, int render_height);
            
private:

  struct ggo_particle_neighbour;

	struct ggo_particle
	{
    ggo::pos2f	                        _cur_pos;
    ggo::pos2f	                        _prv_pos;
    ggo::pos2f	                        _relaxed_pos;
    ggo::vec2f	                        _speed;
    float                               _pressure;
    float                               _near_pressure;
    int                                 _grid_x;
    int                                 _grid_y;
    float                               _prv_temperature;
    float                               _cur_temperature;
    std::vector<ggo_particle_neighbour> _neighbours;
	};
    
    struct ggo_particle_neighbour
    {
      const ggo_particle *    _particle;
      float                   _dist;
      float                   _influence;
    };
    
    struct ggo_particle_emitter
    {
      float      _x;
      float      _y_inf;
      float      _y_sup;
      ggo::vec2f _speed;
      int        _counter;
      float      _temperature;
      
      void  create_particles(std::vector<ggo_particle> & particles);
    };

private:

    void	init_sub() override;
    void	init_bkgd_buffer(uint8_t * bkgd_buffer) override;
    bool	render_next_frame_bkgd(uint8_t * buffer, int frame_index) override;
            
    void  apply_body_forces();
    void  advance();
    void  calculate_pressure();
    void  calculate_relaxed_positions();
    void  move_to_relaxed_positions();
    void  update_grid();
    void  resolve_collisions();
    
    void  apply_temperature();
        
    void  paint_flow(uint8_t * buffer) const;
    float get_potiental(float render_x, float render_y) const;
    float get_temperature(float render_x, float render_y) const;
    float get_view_width() const;
    
private:

    constexpr static int SUB_STEPS_COUNT = 10;
    constexpr static int FRAME_RATE = 25;
    constexpr static float DELTA_TIME = 1.f / (FRAME_RATE * SUB_STEPS_COUNT);
    constexpr static float DELTA_TIME_POW2 = DELTA_TIME * DELTA_TIME;
    constexpr static float PARTICLE_RADIUS = 0.05f;
    constexpr static float INFLUENCE_RADIUS = 6 * PARTICLE_RADIUS;
    constexpr static float NORM = 20 / (2 * ggo::PI<float>() * INFLUENCE_RADIUS * INFLUENCE_RADIUS);
    constexpr static float NEAR_NORM = 30 / (2 * ggo::PI<float>() * INFLUENCE_RADIUS * INFLUENCE_RADIUS);
    constexpr static float VIEW_HEIGHT = 20;

    std::vector<ggo_particle>                         _particles;
    ggo::array<std::vector<const ggo_particle *>, 2>  _grid;
    ggo_particle_emitter                              _emitter1;
    ggo_particle_emitter                              _emitter2;
    float                                             _rest_density;
    float                                             _stiffness;
    float                                             _near_stiffness;
    float                                             _surface_tension;
    float                                             _linear_viscocity;
    float                                             _quadratic_viscocity;
    float                                             _particle_mass;
    float                                             _gravity;
    float                                             _hue1;
    float                                             _hue2;
    float                                             _sat1;
    float                                             _sat2;
    float                                             _val1;
    float                                             _val2;
};

#endif
