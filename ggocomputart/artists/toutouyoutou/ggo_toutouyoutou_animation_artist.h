#ifndef __GGO_TOUTOUYOUTOU_ANIM_ARTIST__
#define __GGO_TOUTOUYOUTOU_ANIM_ARTIST__

// References:
// http://imdoingitwrong.wordpress.com/2010/12/14/why-my-fluids-dont-flow/
// http://cg.informatik.uni-freiburg.de/publications/2007_SCA_SPH.pdf

#include <ggo_animation_artist_abc.h>

namespace ggo
{
  class toutouyoutou_animation_artist : public animation_artist_abc
  {
  public:

          toutouyoutou_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt);

    void  init() override;
    bool  prepare_frame() override;
    void  process_frame(void * buffer, const ggo::rect_int & clipping) override;

  private:

    struct particle;

    struct particle_neighbour
    {
      const particle *  _particle;
      float             _dist;
      float             _influence;
    };

    struct particle
    {
      ggo::pos2f	                    _cur_pos;
      ggo::pos2f	                    _prv_pos;
      ggo::pos2f	                    _relaxed_pos;
      ggo::vec2f	                    _speed;
      float                           _pressure;
      float                           _near_pressure;
      int                             _grid_x;
      int                             _grid_y;
      float                           _prv_temperature;
      float                           _cur_temperature;
      std::vector<particle_neighbour> _neighbours;
    };

    struct particle_emitter
    {
      float      _x;
      float      _y_inf;
      float      _y_sup;
      ggo::vec2f _speed;
      int        _counter;
      float      _temperature;

      void  create_particles(std::vector<particle> & particles);
    };

  private:

    void  apply_body_forces();
    void  advance();
    void  calculate_pressure();
    void  calculate_relaxed_positions();
    void  move_to_relaxed_positions();
    void  update_grid();
    void  resolve_collisions();

    void  apply_temperature();

    void  paint_flow(void * buffer) const;
    float get_potiental(float render_x, float render_y) const;
    float get_temperature(float render_x, float render_y) const;
    float get_view_width() const;

  private:

    constexpr static int sub_steps_count = 10;
    constexpr static int frame_rate = 25;
    constexpr static float delta_time = 1.f / (frame_rate * sub_steps_count);
    constexpr static float delta_time_pow2 = delta_time * delta_time;
    constexpr static float particle_radius = 0.05f;
    constexpr static float influence_radius = 6 * particle_radius;
    constexpr static float norm = 20 / (2 * ggo::pi<float>() * influence_radius * influence_radius);
    constexpr static float near_norm = 30 / (2 * ggo::pi<float>() * influence_radius * influence_radius);
    constexpr static float view_height = 20;

    int                                           _frame_index;
    std::unique_ptr<uint8_t>                      _background;
    std::vector<particle>                         _particles;
    ggo::array<std::vector<const particle *>, 2>  _grid;
    particle_emitter                              _emitter1;
    particle_emitter                              _emitter2;
    float                                         _rest_density;
    float                                         _stiffness;
    float                                         _near_stiffness;
    float                                         _surface_tension;
    float                                         _linear_viscocity;
    float                                         _quadratic_viscocity;
    float                                         _particle_mass;
    float                                         _gravity;
    float                                         _hue1;
    float                                         _hue2;
    float                                         _sat1;
    float                                         _sat2;
    float                                         _val1;
    float                                         _val2;
  };
}

#endif
