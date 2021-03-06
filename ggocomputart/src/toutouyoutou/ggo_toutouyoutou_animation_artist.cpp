#include "ggo_toutouyoutou_animation_artist.h"
#include <kernel/ggo_kernel.h>
#include <2d/fill/ggo_fill.h>
#include <2d/fill/ggo_fill.h>

//////////////////////////////////////////////////////////////
void ggo::toutouyoutou_animation_artist::particle_emitter::create_particles(std::vector<ggo::toutouyoutou_animation_artist::particle> & particles)
{
  constexpr int particles_count = 15;
  
  if (_counter >= 0 && _counter < 1000)
  {
    for (int i = 0; i < particles_count; ++i)
    {
      ggo::toutouyoutou_animation_artist::particle particle;
      float x = _x;
      float y = ggo::map(static_cast<float>(i), 0.f, static_cast<float>(particles_count - 1), static_cast<float>(_y_inf), static_cast<float>(_y_sup)) * view_height;
      particle._cur_pos = particle._prv_pos = ggo::pos2_f(x, y);
      particle._speed = _speed;
      particle._prv_temperature = _temperature;
      particle._cur_temperature = _temperature;
      
      particles.push_back(particle);
    }
  }

  ++_counter;
}

//////////////////////////////////////////////////////////////
ggo::toutouyoutou_animation_artist::toutouyoutou_animation_artist(
  int width, int height, int line_byte_step,
  ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order,
  ggo::ratio fps)
:
animation_artist(width, height, line_byte_step, pixel_type, memory_lines_order),
_grid(ggo::round_to<int>(view_height / influence_radius), ggo::round_to<int>(view_height / influence_radius)), // Grid size is the same as the discard radius.
_background({ width, height }, line_byte_step)
{
  _sub_steps_count = static_cast<int>(std::ceil(to<float>(250 / fps)));
  _delta_time = 1.f / to<float>(fps * _sub_steps_count);

  _rest_density = ggo::rand<float>(100.f, 200.f); // 150
  _stiffness = ggo::rand<float>(0.005f, 0.015f); // 0.01
  _near_stiffness = ggo::rand<float>(0.05f, 0.15f); // 0.1
  _surface_tension = ggo::rand<float>(0.0003f, 0.0005f); // 0.0004
  _linear_viscocity = ggo::rand<float>(0.3f, 0.7f); // 0.5
  _quadratic_viscocity = ggo::rand<float>(0.08f, 0.12f); // 0.1
  _particle_mass = ggo::rand<float>(0.3f, 0.7f); // 0.5
  _gravity = ggo::rand<float>(5.f, 15.f); // 9.8

  _hue1 = ggo::rand<float>();
  _hue2 = _hue1 + ggo::rand<float>(0.1f, 0.2f);
  _sat1 = ggo::rand<float>();
  _sat2 = _sat1 + ggo::rand<float>(0.1f, 0.2f);
  _val1 = ggo::rand<float>();
  _val2 = _val1 + ggo::rand<float>(0.1f, 0.2f);

  _emitter1._x = 0;
  _emitter1._y_inf = ggo::rand<float>(0.3f, 0.7f);
  _emitter1._y_sup = _emitter1._y_inf + 0.12f;
  _emitter1._speed = ggo::vec2_f::from_angle(ggo::rand<float>(0, ggo::pi<float>() / 4)) * ggo::rand<float>(5, 15);
  _emitter1._temperature = 0;

  _emitter2._x = get_view_width();
  _emitter2._y_inf = ggo::rand<float>(0.3f, 0.7f);
  _emitter2._y_sup = _emitter2._y_inf + 0.12f;
  _emitter2._speed = ggo::vec2_f::from_angle(ggo::rand<float>(3 * ggo::pi<float>() / 4, ggo::pi<float>())) * ggo::rand<float>(5, 15);
  _emitter2._temperature = 1;

  if (ggo::rand<bool>())
  {
    _emitter1._counter = 0;
    _emitter2._counter = ggo::rand<int>(-200, -100);
  }
  else
  {
    _emitter1._counter = ggo::rand<int>(-200, -100);
    _emitter2._counter = 0;
  }
}

////////////////////////////////////////////////////////////// 
void ggo::toutouyoutou_animation_artist::render_frame(void * buffer, bool & finished)
{
  for (int step = 0; step < _sub_steps_count; ++step)
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

  update_grid();

  if (_frame_index == 0)
  {
    ggo::fill_gaussian(_background, static_cast<float>(min_size()), ggo::white_8u(), { 0x80_u8, 0x80_u8, 0x80_u8 });
  }

  if (buffer != nullptr)
  {
    memcpy(buffer, _background.data(), height() * line_byte_step());
    paint_flow(buffer);
  }
}

//////////////////////////////////////////////////////////////
void ggo::toutouyoutou_animation_artist::calculate_pressure()
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
          if (hypot > influence_radius * influence_radius || hypot < 0.00000001)
          {
              continue;
          }
          
          float dist = std::sqrt(hypot);
          float influence = 1 - dist / influence_radius;
          GGO_ASSERT(influence >= 0 && influence <= 1);
          float influence_pow2 = influence * influence;
          float influence_pow3 = influence * influence_pow2;
          float influence_pow4 = influence * influence_pow3;
          
          density += _particle_mass * influence_pow3 * norm;
          near_density += _particle_mass * influence_pow4 * near_norm;
  
          ggo::toutouyoutou_animation_artist::particle_neighbour neighbour;
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
void ggo::toutouyoutou_animation_artist::calculate_relaxed_positions()
{
  float delta_time_pow2 = ggo::square(_delta_time);

  for (auto & particle : _particles)
  {
    particle._relaxed_pos = particle._cur_pos;
  
    for (const auto & neighbour : particle._neighbours)
    {
      const auto & particle2 = *neighbour._particle;
      float influence = neighbour._influence;
      float influence_pow2 = influence * influence;
      float influence_pow3 = influence * influence_pow2;
    
      ggo::vec2_f dpos(particle2._cur_pos - particle._cur_pos);
      
      // Relax.
      float d = delta_time_pow2 * ((particle._near_pressure + particle2._near_pressure) * influence_pow3 * near_norm +
                                   (particle._pressure + particle2._pressure) * influence_pow2 * norm) / 2;
      particle._relaxed_pos -= d * dpos / (neighbour._dist * _particle_mass);
 
      // Surface tension.
      particle._relaxed_pos += (_surface_tension / _particle_mass) * _particle_mass * influence_pow2 * norm * dpos;
 
      // Viscosity.
      ggo::vec2_f dspeed = particle._speed - particle2._speed;
      float u = dspeed.x() * dpos.x() + dspeed.y() * dpos.y();
      if (u > 0)
      {
        u /= neighbour._dist;
 
        float I = _delta_time * influence * (_linear_viscocity * u + _quadratic_viscocity * u * u) / 2;
        
        particle._relaxed_pos -= I * dpos * _delta_time;
      }
    }
  }
}

//////////////////////////////////////////////////////////////
void ggo::toutouyoutou_animation_artist::move_to_relaxed_positions()
{
  for (auto & particle : _particles)
  {
    particle._cur_pos = particle._relaxed_pos;
    particle._speed = (particle._cur_pos - particle._prv_pos) / _delta_time;
  }
}

//////////////////////////////////////////////////////////////
void ggo::toutouyoutou_animation_artist::advance()
{
  for (auto & particle : _particles)
  {
    particle._prv_pos = particle._cur_pos;
    particle._cur_pos += _delta_time * particle._speed;
  }
}

//////////////////////////////////////////////////////////////
void ggo::toutouyoutou_animation_artist::update_grid()
{
  // Clear grid.
  for (auto & particles : _grid)
  {
    particles.clear();
  }   
  
  // Add particles to grid.
  for (auto & particle : _particles)
  {
    particle._grid_x = ggo::round_to<int>(particle._cur_pos.x() / influence_radius);
    particle._grid_x = ggo::clamp(particle._grid_x, 1, _grid.width() - 2);
    
    particle._grid_y = ggo::round_to<int>(particle._cur_pos.y() / influence_radius);
    particle._grid_y = ggo::clamp(particle._grid_y, 1, _grid.height() - 2);
    
    _grid(particle._grid_x, particle._grid_y).push_back(&particle);
  }
}

//////////////////////////////////////////////////////////////
void ggo::toutouyoutou_animation_artist::apply_body_forces()
{
  for (auto & particle : _particles)
  {
    particle._speed.y() -= _gravity * _delta_time;
  }
}

//////////////////////////////////////////////////////////////
void ggo::toutouyoutou_animation_artist::resolve_collisions()
{
  for (auto & particle : _particles)
  {
    // Left wall.
    {
      float dist = particle._cur_pos.x();
      if (dist < particle_radius)
      {
        dist = std::max<float>(0, dist);
        particle._speed.x() += (particle_radius - dist) / _delta_time;
      }
    }
    // Right wall.
    {
      float dist = get_view_width() - particle._cur_pos.x();
      if (dist < particle_radius)
      {
        dist = std::max<float>(0, dist);
        particle._speed.x() -= (particle_radius - dist) / _delta_time;
      }
    }
    // Bottom wall.
    {
      float dist = particle._cur_pos.y();
      if (dist < particle_radius)
      {
        dist = std::max<float>(0, dist);
        particle._speed.y() += (particle_radius - dist) / _delta_time;
      }
    }
  }
}

//////////////////////////////////////////////////////////////
void ggo::toutouyoutou_animation_artist::apply_temperature()
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
      if (hypot > influence_radius * influence_radius)
      {
        continue;
      }
          
      float dist = std::sqrt(hypot);
      float influence = 1 - dist / influence_radius;
          
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
void ggo::toutouyoutou_animation_artist::paint_flow(void * buffer) const
{
  constexpr float potential_threshold = 0.8f;
  
  ggo::array_8u sample_buffer(width() * height());

  uint8_t * ptr = sample_buffer.data();
  for (int render_y = 0; render_y < height(); ++render_y)
  {
    for (int render_x = 0; render_x < width(); ++render_x)
    {
      ptr[0] = 0;
      if (get_potiental(render_x - 3 / 8.f, render_y - 3 / 8.f) > potential_threshold) { ptr[0] += 1; }
      if (get_potiental(render_x - 3 / 8.f, render_y + 3 / 8.f) > potential_threshold) { ptr[0] += 1; }
      if (get_potiental(render_x + 3 / 8.f, render_y - 3 / 8.f) > potential_threshold) { ptr[0] += 1; }
      if (get_potiental(render_x + 3 / 8.f, render_y + 3 / 8.f) > potential_threshold) { ptr[0] += 1; }
      
      if (ptr[0] != 0)
      {
        if (ptr[0] < 4)
        {
          GGO_ASSERT(ptr[0] > 0 && ptr[0] < 4);
        
          if (get_potiental(render_x - 3 / 8.f, render_y - 1 / 8.f) > potential_threshold) { ptr[0] += 1; }
          if (get_potiental(render_x - 3 / 8.f, render_y + 1 / 8.f) > potential_threshold) { ptr[0] += 1; }
          
          if (get_potiental(render_x - 1 / 8.f, render_y - 3 / 8.f) > potential_threshold) { ptr[0] += 1; }
          if (get_potiental(render_x - 1 / 8.f, render_y - 1 / 8.f) > potential_threshold) { ptr[0] += 1; }
          if (get_potiental(render_x - 1 / 8.f, render_y + 1 / 8.f) > potential_threshold) { ptr[0] += 1; }
          if (get_potiental(render_x - 1 / 8.f, render_y + 3 / 8.f) > potential_threshold) { ptr[0] += 1; }
          
          if (get_potiental(render_x + 1 / 8.f, render_y - 3 / 8.f) > potential_threshold) { ptr[0] += 1; }
          if (get_potiental(render_x + 1 / 8.f, render_y - 1 / 8.f) > potential_threshold) { ptr[0] += 1; }
          if (get_potiental(render_x + 1 / 8.f, render_y + 1 / 8.f) > potential_threshold) { ptr[0] += 1; }
          if (get_potiental(render_x + 1 / 8.f, render_y + 3 / 8.f) > potential_threshold) { ptr[0] += 1; }
          
          if (get_potiental(render_x + 3 / 8.f, render_y - 1 / 8.f) > potential_threshold) { ptr[0] += 1; }
          if (get_potiental(render_x + 3 / 8.f, render_y + 1 / 8.f) > potential_threshold) { ptr[0] += 1; }
        }
        else
        {
            ptr[0] = 16;
        }
        
        // Compute opacity.
        /*float potential = get_potiental(render_x, render_y);
        float potential_mapped =  0.5f + std::atan((potential - potential_threshold) / 10) / GGO_pi; // Between 0.5 and 1
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
  //ggo::array_uint8 border_buffer(width() * height());
  //float radius = 0.0025f * get_min_size();
  //ggo::dilate_circle_kernel(sample_buffer, border_buffer, width(), height(), radius);
  
  //for (int i = 0; i < border_buffer.get_size(); ++i)
  //{   
  //  if (border_buffer[i] > 0)
  //  {
  //    buffer[3 * i + 0] = ((16 - border_buffer[i]) * buffer[3 * i + 0] + 8) / 16;
  //    buffer[3 * i + 1] = ((16 - border_buffer[i]) * buffer[3 * i + 1] + 8) / 16;
  //    buffer[3 * i + 2] = ((16 - border_buffer[i]) * buffer[3 * i + 2] + 8) / 16;
  //  }
  //}
  
  // Inside.
  ggo::image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up> img(buffer, size(), line_byte_step());
  uint8_t * ptr_sample = sample_buffer.data();
  for (int render_y = 0; render_y < height(); ++render_y)
  {
    for (int render_x = 0; render_x < width(); ++render_x)
    {
      if (ptr_sample[0] > 0)
      {
        // Compute opacity.
        float potential = get_potiental(static_cast<float>(render_x), static_cast<float>(render_y));
        float potential_mapped =  0.5f + std::atan((potential - potential_threshold) / 10) / ggo::pi<float>(); // Between 0.5 and 1
        float opacity = ptr_sample[0] * potential_mapped / 16.f;
        
        // Compute color.
        float temperature = get_temperature(static_cast<float>(render_x), static_cast<float>(render_y));
        GGO_ASSERT(temperature >= 0 && temperature <= 1);
        float hue = temperature * _hue1 + (1 - temperature) * _hue2;
        float sat = temperature * _sat1 + (1 - temperature) * _sat2;
        float val = temperature * _val1 + (1 - temperature) * _val2;
        const ggo::rgb_8u color(ggo::from_hsv<ggo::rgb_8u>(hue, sat, val));
        
        img.write_pixel(render_x, render_y, color);
      }
      
      ptr_sample += 1;
    }
  }

#if 0
  for (const auto & particle : _particles)
  {
    ggo::pos2_f center = particle._cur_pos;
    center *= height() / view_height;
    
    ggo_disc_float disc(center, 0.001 * height());
    disc.center() = horz_mirror(disc.center());

    ggo_paint_shape_rgb(buffer, width(), height(), disc, ggo::color::RED);
  }
#endif
}

//////////////////////////////////////////////////////////////
float ggo::toutouyoutou_animation_artist::get_potiental(float render_x, float render_y) const
{
  float view_x = render_x * view_height / height();
  float view_y = view_height - render_y * view_height / height();
  
  int grid_x = ggo::clamp(ggo::round_to<int>(view_x / influence_radius), 1, _grid.width() - 2);
  int grid_y = ggo::clamp(ggo::round_to<int>(view_y / influence_radius), 1, _grid.height() - 2);

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
        if (hypot <= influence_radius * influence_radius)
        {
          float dist = std::sqrt(hypot);
          potential += 1 - dist / influence_radius;
        }
      }
    }
  }
  
  return potential;
}

//////////////////////////////////////////////////////////////
float ggo::toutouyoutou_animation_artist::get_temperature(float render_x, float render_y) const
{
  float view_x = render_x * view_height / height();
  float view_y = view_height - render_y * view_height / height();
  
  int grid_x = ggo::clamp(ggo::round_to<int>(view_x / influence_radius), 1, _grid.width() - 2);
  int grid_y = ggo::clamp(ggo::round_to<int>(view_y / influence_radius), 1, _grid.height() - 2);

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
        if (hypot <= influence_radius * influence_radius)
        {
          float dist = std::sqrt(hypot);
          float influence = 1 - dist / influence_radius;
          
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
float ggo::toutouyoutou_animation_artist::get_view_width() const
{
  return width() * view_height / static_cast<float>(height());
}
