#include "ggo_toutouyoutou_animation_artist.h"
#include <ggo_kernel.h>
#include <ggo_fill.h>
#include <ggo_paint.h>
#include <ggo_morphology.h>

//////////////////////////////////////////////////////////////
void ggo_toutouyoutou_anim_artist::ggo_particle_emitter::create_particles(std::vector<ggo_toutouyoutou_anim_artist::ggo_particle> & particles)
{
  constexpr int PARTICLES_COUNT = 15;
  
  if (_counter >= 0 && _counter < 1000)
  {
    for (int i = 0; i < PARTICLES_COUNT; ++i)
    {
      ggo_particle particle;
      float x = _x;
      float y = ggo::map(static_cast<float>(i), 0.f, static_cast<float>(PARTICLES_COUNT - 1), static_cast<float>(_y_inf), static_cast<float>(_y_sup)) * VIEW_HEIGHT;
      particle._cur_pos = particle._prv_pos = ggo::point2d_float(x, y);
      particle._speed = _speed;
      particle._prv_temperature = _temperature;
      particle._cur_temperature = _temperature;
      
      particles.push_back(particle);
    }
  }

  ++_counter;
}

//////////////////////////////////////////////////////////////
ggo_toutouyoutou_anim_artist::ggo_toutouyoutou_anim_artist(int render_width, int render_height)
:
ggo_static_background_animation_artist_abc(render_width, render_height),
_grid(ggo::to<int>(VIEW_HEIGHT / INFLUENCE_RADIUS), ggo::to<int>(VIEW_HEIGHT / INFLUENCE_RADIUS)) // Grid size is the same as the discard radius.
{
}

//////////////////////////////////////////////////////////////
void ggo_toutouyoutou_anim_artist::init_sub()
{
  _particles.clear();

  _rest_density = ggo::rand_float(100.f, 200.f); // 150
  _stiffness = ggo::rand_float(0.005f, 0.015f); // 0.01
  _near_stiffness = ggo::rand_float(0.05f, 0.15f); // 0.1
  _surface_tension = ggo::rand_float(0.0003f, 0.0005f); // 0.0004
  _linear_viscocity = ggo::rand_float(0.3f, 0.7f); // 0.5
  _quadratic_viscocity = ggo::rand_float(0.08f, 0.12f); // 0.1
  _particle_mass = ggo::rand_float(0.3f, 0.7f); // 0.5
  _gravity = ggo::rand_float(5.f, 15.f); // 9.8
  
  _hue1 = ggo::rand_float();
  _hue2 = _hue1 + ggo::rand_float(0.1f, 0.2f);
  _sat1 = ggo::rand_float();
  _sat2 = _sat1 + ggo::rand_float(0.1f, 0.2f);
  _val1 = ggo::rand_float();
  _val2 = _val1 + ggo::rand_float(0.1f, 0.2f);
  
  _emitter1._x = 0;
  _emitter1._y_inf = ggo::rand_float(0.3f, 0.7f);
  _emitter1._y_sup = _emitter1._y_inf + 0.12f;
  _emitter1._speed = ggo::vector2d_float::from_polar(ggo::rand_float(0, ggo::PI<float>() / 4), ggo::rand_float(5, 15));
  _emitter1._temperature = 0;
  
  _emitter2._x = get_view_width();
  _emitter2._y_inf = ggo::rand_float(0.3f, 0.7f);
  _emitter2._y_sup = _emitter2._y_inf + 0.12f;
  _emitter2._speed = ggo::vector2d_float::from_polar(ggo::rand_float(3 * ggo::PI<float>() / 4, ggo::PI<float>()), ggo::rand_float(5, 15));
  _emitter2._temperature = 1;
  
  if (ggo::rand_bool())
  {
    _emitter1._counter = 0;
    _emitter2._counter = ggo::rand_int(-200, -100);
  }
  else
  {
    _emitter1._counter = ggo::rand_int(-200, -100);
    _emitter2._counter = 0;
  }
}

////////////////////////////////////////////////////////////// 
void ggo_toutouyoutou_anim_artist::init_bkgd_buffer(uint8_t * bkgd_buffer)
{
  ggo::rgb_image_data_uint8 image_data(bkgd_buffer, get_render_width(), get_render_height());
  
  ggo::fill_gaussian(image_data, static_cast<float>(get_render_min_size()), ggo::color::WHITE, ggo::color(0.5f, 0.5f, 0.5f));
}

//////////////////////////////////////////////////////////////
bool ggo_toutouyoutou_anim_artist::render_next_frame_bkgd(uint8_t * buffer, int frame_index)
{
  if (frame_index > 1000)
  {
      return false;
  }
  
  for (int step = 0; step < SUB_STEPS_COUNT; ++step)
  {
    _emitter1.create_particles(_particles);
    _emitter2.create_particles(_particles);
  
    apply_body_forces();
    advance();
    update_grid();
    calculate_pressure();
    calculate_relaxed_positions();
    move_to_relaxed_positions();
    resolve_collisions();
    
    apply_temperature();
  }

  if (buffer != nullptr)
  {
    update_grid();
    paint_flow(buffer);
  }

  return true;
}

//////////////////////////////////////////////////////////////
void ggo_toutouyoutou_anim_artist::calculate_pressure()
{
  for (auto & particle1 : _particles)
  {
    float density = 0;
    float near_density = 0;
    
    particle1._neighbours.clear();
    
    for (int x = particle1._grid_x - 1; x <= particle1._grid_x + 1; ++x)
    {
      for (int y = particle1._grid_y - 1; y <= particle1._grid_y + 1; ++y)
      {
        for (const auto * particle2 : _grid(x, y))
        {
          if (&particle1 == particle2)
          {
              continue;
          }
          
          float hypot = ggo::hypot(particle1._cur_pos, particle2->_cur_pos);
          if (hypot > INFLUENCE_RADIUS * INFLUENCE_RADIUS || hypot < 0.00000001)
          {
              continue;
          }
          
          float dist = std::sqrt(hypot);
          float influence = 1 - dist / INFLUENCE_RADIUS;
          GGO_ASSERT(influence >= 0 && influence <= 1);
          float influence_pow2 = influence * influence;
          float influence_pow3 = influence * influence_pow2;
          float influence_pow4 = influence * influence_pow3;
          
          density += _particle_mass * influence_pow3 * NORM;
          near_density += _particle_mass * influence_pow4 * NEAR_NORM;
  
          ggo_particle_neighbour neighbour;
          neighbour._particle = particle2;
          neighbour._dist = dist;
          neighbour._influence = influence;
          
          particle1._neighbours.push_back(neighbour);
        }
      }
    }
 
    particle1._pressure = _stiffness * (density - _particle_mass * _rest_density);
    particle1._near_pressure = _near_stiffness * near_density;
  }
}

//////////////////////////////////////////////////////////////
void ggo_toutouyoutou_anim_artist::calculate_relaxed_positions()
{
  for (auto & particle : _particles)
  {
    particle._relaxed_pos = particle._cur_pos;
  
    for (const auto & neighbour : particle._neighbours)
    {
      const ggo_particle & particle2 = *neighbour._particle;
      float influence = neighbour._influence;
      float influence_pow2 = influence * influence;
      float influence_pow3 = influence * influence_pow2;
    
      ggo::vector2d_float dpos(particle2._cur_pos - particle._cur_pos);
      
      // Relax.
      float d = DELTA_TIME_POW2 * ((particle._near_pressure + particle2._near_pressure) * influence_pow3 * NEAR_NORM +
                                   (particle._pressure + particle2._pressure) * influence_pow2 * NORM) / 2;
      particle._relaxed_pos -= d * dpos / (neighbour._dist * _particle_mass);
 
      // Surface tension.
      particle._relaxed_pos += (_surface_tension / _particle_mass) * _particle_mass * influence_pow2 * NORM * dpos;
 
      // Viscosity.
      ggo::vector2d_float dspeed = particle._speed - particle2._speed;
      float u = dspeed.x() * dpos.x() + dspeed.y() * dpos.y();
      if (u > 0)
      {
        u /= neighbour._dist;
 
        float I = DELTA_TIME * influence * (_linear_viscocity * u + _quadratic_viscocity * u * u) / 2;
        
        particle._relaxed_pos -= I * dpos * DELTA_TIME;
      }
    }
  }
}

//////////////////////////////////////////////////////////////
void ggo_toutouyoutou_anim_artist::move_to_relaxed_positions()
{
  for (auto & particle : _particles)
  {
    particle._cur_pos = particle._relaxed_pos;
    particle._speed = (particle._cur_pos - particle._prv_pos) / DELTA_TIME;
  }
}

//////////////////////////////////////////////////////////////
void ggo_toutouyoutou_anim_artist::advance()
{
  for (auto & particle : _particles)
  {
    particle._prv_pos = particle._cur_pos;
    particle._cur_pos += DELTA_TIME * particle._speed;
  }
}

//////////////////////////////////////////////////////////////
void ggo_toutouyoutou_anim_artist::update_grid()
{
  // Clear grid.
  for (auto & particles : _grid)
  {
    particles.clear();
  }   
  
  // Add particles to grid.
  for (auto & particle : _particles)
  {
    particle._grid_x = ggo::to<int>(particle._cur_pos.x() / INFLUENCE_RADIUS);
    particle._grid_x = ggo::clamp(particle._grid_x, 1, _grid.get_size_x() - 2);
    
    particle._grid_y = ggo::to<int>(particle._cur_pos.y() / INFLUENCE_RADIUS);
    particle._grid_y = ggo::clamp(particle._grid_y, 1, _grid.get_size_y() - 2);
    
    _grid(particle._grid_x, particle._grid_y).push_back(&particle);
  }
}

//////////////////////////////////////////////////////////////
void ggo_toutouyoutou_anim_artist::apply_body_forces()
{
  for (auto & particle : _particles)
  {
    particle._speed.y() -= _gravity * DELTA_TIME;
  }
}

//////////////////////////////////////////////////////////////
void ggo_toutouyoutou_anim_artist::resolve_collisions()
{
  for (auto & particle : _particles)
  {
    // Left wall.
    {
      float dist = particle._cur_pos.x();
      if (dist < PARTICLE_RADIUS)
      {
        dist = std::max<float>(0, dist);
        particle._speed.x() += (PARTICLE_RADIUS - dist) / DELTA_TIME;
      }
    }
    // Right wall.
    {
      float dist = get_view_width() - particle._cur_pos.x();
      if (dist < PARTICLE_RADIUS)
      {
        dist = std::max<float>(0, dist);
        particle._speed.x() -= (PARTICLE_RADIUS - dist) / DELTA_TIME;
      }
    }
    // Bottom wall.
    {
      float dist = particle._cur_pos.y();
      if (dist < PARTICLE_RADIUS)
      {
        dist = std::max<float>(0, dist);
        particle._speed.y() += (PARTICLE_RADIUS - dist) / DELTA_TIME;
      }
    }
  }
}

//////////////////////////////////////////////////////////////
void ggo_toutouyoutou_anim_artist::apply_temperature()
{
  for (auto & particle : _particles)
  {
    particle._prv_temperature = particle._cur_temperature;
  }

  for (auto & particle : _particles)
  {
    float neighbour_temperature = 0;
    float normalization = 0;
  
    for (const auto & neighbour : particle._neighbours)
    {
      if (&particle == neighbour._particle)
      {
        continue;
      }
          
      float hypot = ggo::hypot(particle._cur_pos, neighbour._particle->_cur_pos);
      if (hypot > INFLUENCE_RADIUS * INFLUENCE_RADIUS)
      {
        continue;
      }
          
      float dist = std::sqrt(hypot);
      float influence = 1 - dist / INFLUENCE_RADIUS;
          
      neighbour_temperature += influence * neighbour._particle->_prv_temperature;
      normalization += influence;
    }
      
    if (normalization > 0)
    {
      neighbour_temperature /= normalization;
      particle._cur_temperature = 0.9f * particle._cur_temperature + 0.1f * neighbour_temperature;
    }
  }
}

//////////////////////////////////////////////////////////////
void ggo_toutouyoutou_anim_artist::paint_flow(uint8_t * buffer) const
{
  constexpr float POTENTIAL_THRESHOLD = 0.8f;
  
  ggo::array_uint8 sample_buffer(get_render_width() * get_render_height());

  uint8_t * ptr = sample_buffer;
  for (int render_y = 0; render_y < get_render_height(); ++render_y)
  {
    for (int render_x = 0; render_x < get_render_width(); ++render_x)
    {
      ptr[0] = 0;
      if (get_potiental(render_x - 3 / 8.f, render_y - 3 / 8.f) > POTENTIAL_THRESHOLD) { ptr[0] += 1; }
      if (get_potiental(render_x - 3 / 8.f, render_y + 3 / 8.f) > POTENTIAL_THRESHOLD) { ptr[0] += 1; }
      if (get_potiental(render_x + 3 / 8.f, render_y - 3 / 8.f) > POTENTIAL_THRESHOLD) { ptr[0] += 1; }
      if (get_potiental(render_x + 3 / 8.f, render_y + 3 / 8.f) > POTENTIAL_THRESHOLD) { ptr[0] += 1; }
      
      if (ptr[0] != 0)
      {
        if (ptr[0] < 4)
        {
          GGO_ASSERT(ptr[0] > 0 && ptr[0] < 4);
        
          if (get_potiental(render_x - 3 / 8.f, render_y - 1 / 8.f) > POTENTIAL_THRESHOLD) { ptr[0] += 1; }
          if (get_potiental(render_x - 3 / 8.f, render_y + 1 / 8.f) > POTENTIAL_THRESHOLD) { ptr[0] += 1; }
          
          if (get_potiental(render_x - 1 / 8.f, render_y - 3 / 8.f) > POTENTIAL_THRESHOLD) { ptr[0] += 1; }
          if (get_potiental(render_x - 1 / 8.f, render_y - 1 / 8.f) > POTENTIAL_THRESHOLD) { ptr[0] += 1; }
          if (get_potiental(render_x - 1 / 8.f, render_y + 1 / 8.f) > POTENTIAL_THRESHOLD) { ptr[0] += 1; }
          if (get_potiental(render_x - 1 / 8.f, render_y + 3 / 8.f) > POTENTIAL_THRESHOLD) { ptr[0] += 1; }
          
          if (get_potiental(render_x + 1 / 8.f, render_y - 3 / 8.f) > POTENTIAL_THRESHOLD) { ptr[0] += 1; }
          if (get_potiental(render_x + 1 / 8.f, render_y - 1 / 8.f) > POTENTIAL_THRESHOLD) { ptr[0] += 1; }
          if (get_potiental(render_x + 1 / 8.f, render_y + 1 / 8.f) > POTENTIAL_THRESHOLD) { ptr[0] += 1; }
          if (get_potiental(render_x + 1 / 8.f, render_y + 3 / 8.f) > POTENTIAL_THRESHOLD) { ptr[0] += 1; }
          
          if (get_potiental(render_x + 3 / 8.f, render_y - 1 / 8.f) > POTENTIAL_THRESHOLD) { ptr[0] += 1; }
          if (get_potiental(render_x + 3 / 8.f, render_y + 1 / 8.f) > POTENTIAL_THRESHOLD) { ptr[0] += 1; }
        }
        else
        {
            ptr[0] = 16;
        }
        
        // Compute opacity.
        /*float potential = get_potiental(render_x, render_y);
        float potential_mapped =  0.5f + std::atan((potential - POTENTIAL_THRESHOLD) / 10) / GGO_PI; // Between 0.5 and 1
        float opacity = count * potential_mapped / 16.f;
        
        // Compute color.
        float temperature = get_temperature(render_x, render_y);
        float hue = _hue_offset + _hue_temperature_scale * temperature;
        ggo::color color(ggo::color::from_hsv(hue, 1, 1));
        
        ggo::paint_pixel_alpha_rgb_uint8(ptr, opacity, color);*/
      }
      
      ptr++;
    }
  }
  
  // Border.
  ggo::array_uint8 border_buffer(get_render_width() * get_render_height());
  float radius = 0.0025f * get_render_min_size();
  ggo::dilate_circle_kernel(sample_buffer, border_buffer, get_render_width(), get_render_height(), radius);
  
  for (int i = 0; i < border_buffer.get_size(); ++i)
  {   
    if (border_buffer[i] > 0)
    {
      buffer[3 * i + 0] = ((16 - border_buffer[i]) * buffer[3 * i + 0] + 8) / 16;
      buffer[3 * i + 1] = ((16 - border_buffer[i]) * buffer[3 * i + 1] + 8) / 16;
      buffer[3 * i + 2] = ((16 - border_buffer[i]) * buffer[3 * i + 2] + 8) / 16;
    }
  }
  
  // Inside.
  uint8_t * ptr_sample = sample_buffer;
  for (int render_y = 0; render_y < get_render_height(); ++render_y)
  {
    for (int render_x = 0; render_x < get_render_width(); ++render_x)
    {
      if (ptr_sample[0] > 0)
      {
        // Compute opacity.
        float potential = get_potiental(static_cast<float>(render_x), static_cast<float>(render_y));
        float potential_mapped =  0.5f + std::atan((potential - POTENTIAL_THRESHOLD) / 10) / ggo::PI<float>(); // Between 0.5 and 1
        float opacity = ptr_sample[0] * potential_mapped / 16.f;
        
        // Compute color.
        float temperature = get_temperature(static_cast<float>(render_x), static_cast<float>(render_y));
        GGO_ASSERT(temperature >= 0 && temperature <= 1);
        float hue = temperature * _hue1 + (1 - temperature) * _hue2;
        float sat = temperature * _sat1 + (1 - temperature) * _sat2;
        float val = temperature * _val1 + (1 - temperature) * _val2;
        ggo::color color(ggo::color::from_hsv(hue, sat, val));
        
        ggo::paint_pixel(buffer, get_render_width(), get_render_height(), render_x, render_y, color);
      }
      
      ptr_sample += 1;
    }
  }

#if 0
  for (const auto & particle : _particles)
  {
    ggo::point2d_float center = particle._cur_pos;
    center *= get_render_height() / VIEW_HEIGHT;
    
    ggo_disc_float disc(center, 0.001 * get_render_height());
    disc.center() = horz_mirror(disc.center());

    ggo_paint_shape_rgb(buffer, get_render_width(), get_render_height(), disc, ggo::color::RED);
  }
#endif
}

//////////////////////////////////////////////////////////////
float ggo_toutouyoutou_anim_artist::get_potiental(float render_x, float render_y) const
{
  float view_x = render_x * VIEW_HEIGHT / get_render_height();
  float view_y = VIEW_HEIGHT - render_y * VIEW_HEIGHT / get_render_height();
  
  int grid_x = ggo::clamp(ggo::to<int>(view_x / INFLUENCE_RADIUS), 1, _grid.get_size_x() - 2);
  int grid_y = ggo::clamp(ggo::to<int>(view_y / INFLUENCE_RADIUS), 1, _grid.get_size_y() - 2);

  float potential = 0;
  
  for (int x = grid_x - 1; x <= grid_x + 1; ++x)
  {
    for (int y = grid_y - 1; y <= grid_y + 1; ++y)
    {
      for (const auto * particle : _grid(x, y))
      {
        float dx = particle->_cur_pos.x() - view_x;
        float dy = particle->_cur_pos.y() - view_y;
        float hypot = dx * dx + dy * dy;
        if (hypot <= INFLUENCE_RADIUS * INFLUENCE_RADIUS)
        {
          float dist = std::sqrt(hypot);
          potential += 1 - dist / INFLUENCE_RADIUS;
        }
      }
    }
  }
  
  return potential;
}

//////////////////////////////////////////////////////////////
float ggo_toutouyoutou_anim_artist::get_temperature(float render_x, float render_y) const
{
  float view_x = render_x * VIEW_HEIGHT / get_render_height();
  float view_y = VIEW_HEIGHT - render_y * VIEW_HEIGHT / get_render_height();
  
  int grid_x = ggo::clamp(ggo::to<int>(view_x / INFLUENCE_RADIUS), 1, _grid.get_size_x() - 2);
  int grid_y = ggo::clamp(ggo::to<int>(view_y / INFLUENCE_RADIUS), 1, _grid.get_size_y() - 2);

  float temperature = 0;
  float normalization = 0;
  
  for (int x = grid_x - 1; x <= grid_x + 1; ++x)
  {
    for (int y = grid_y - 1; y <= grid_y + 1; ++y)
    {
      for (const auto * particle : _grid(x, y))
      {
        float dx = particle->_cur_pos.x() - view_x;
        float dy = particle->_cur_pos.y() - view_y;
        float hypot = dx * dx + dy * dy;
        if (hypot <= INFLUENCE_RADIUS * INFLUENCE_RADIUS)
        {
          float dist = std::sqrt(hypot);
          float influence = 1 - dist / INFLUENCE_RADIUS;
          
          temperature += influence * particle->_cur_temperature;
          normalization += influence;
        }
      }
    }
  }
  
  if (normalization > 0)
  {
    temperature /= normalization;
  }
  
  return temperature;
}

//////////////////////////////////////////////////////////////
float ggo_toutouyoutou_anim_artist::get_view_width() const
{
  return get_render_width() * VIEW_HEIGHT / static_cast<float>(get_render_height());
}
