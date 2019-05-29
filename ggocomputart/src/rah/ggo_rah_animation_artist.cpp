#include "ggo_rah_animation_artist.h"
#include <2d/fill/ggo_fill.h>
#include <2d/paint/ggo_blur_paint.h>

namespace
{
  const float near = 1;
  const float far = 15;
}

//////////////////////////////////////////////////////////////
// PARTICLE

//////////////////////////////////////////////////////////////
ggo::rah_animation_artist::particle::particle(int width, int height, float focus_dist)
{
  int min_size = std::min(width, height);

  _angle = ggo::rand<float>(0,  2 *ggo::pi<float>());
  _dist = ggo::map<float>(std::sqrt(ggo::rand<float>()), 0, 1, near, far);
  float total_radius = disc_radius(min_size) + blur_radius(min_size, focus_dist);
  _pos.x() = -total_radius;
  _pos.y() = ggo::rand<float>(0, static_cast<float>(height));
  _color = ggo::from_hsv<ggo::rgb_8u>(ggo::rand<float>(), 1, 1);
}

//////////////////////////////////////////////////////////////
float ggo::rah_animation_artist::particle::blur_radius(int min_size, float focus_dist) const
{
  return 1 / std::sqrt(2.f) + 0.0005f * min_size * std::abs(_dist - focus_dist);
}

//////////////////////////////////////////////////////////////
float ggo::rah_animation_artist::particle::disc_radius(int min_size) const
{
  return 0.1f * min_size / _dist;
}

//////////////////////////////////////////////////////////////
void ggo::rah_animation_artist::particle::update(int min_size)
{
  _pos.x() += 0.05f * min_size / _dist;
  _pos.y() += _vertical_offset_interpolator.update(1) * min_size / _dist;
  _angle += 0.1f;
}

//////////////////////////////////////////////////////////////
void ggo::rah_animation_artist::particle::paint(void * buffer, int width, int height, float focus_dist) const
{
  int min_size = std::min(width, height);
  
  ggo::multi_shape_f backgrounds;
  ggo::multi_shape_f bodies;
  fill_multi_shapes(backgrounds, bodies, min_size);

  // Paint background first.
  auto paint_border_pixel_func = [&](int x, int y, int num, int den)
  {
    ggo::rgb_8u pixel = ggo::read_pixel<ggo::rgb_8u_yu>(buffer, x, y, height, 3 * width);

    uint8_t r = ggo::round_div((den - num) * pixel.r(), den);
    uint8_t g = ggo::round_div((den - num) * pixel.g(), den);
    uint8_t b = ggo::round_div((den - num) * pixel.b(), den);

    ggo::write_pixel<ggo::rgb_8u_yu>(buffer, x, y, height, 3 * width, ggo::rgb_8u(r, g, b));
  };

  ggo::paint_blur_shape(backgrounds, width, height, blur_radius(min_size, focus_dist), 0.25f, paint_border_pixel_func);

  // Paint bodies.
  auto paint_body_pixel_func = [&](int x, int y, int num, int den)
  {
    ggo::rgb_8u pixel = ggo::read_pixel<ggo::rgb_8u_yu>(buffer, x, y, height, 3 * width);

    uint8_t r = ggo::round_div(num * _color.r() + (den - num) * pixel.r(), den);
    uint8_t g = ggo::round_div(num * _color.g() + (den - num) * pixel.g(), den);
    uint8_t b = ggo::round_div(num * _color.b() + (den - num) * pixel.b(), den);

    ggo::write_pixel<ggo::rgb_8u_yu>(buffer, x, y, height, 3 * width, ggo::rgb_8u(r, g, b));
  };

  ggo::paint_blur_shape(bodies, width, height, blur_radius(min_size, focus_dist), 0.25f, paint_body_pixel_func);
}

//////////////////////////////////////////////////////////////
bool ggo::rah_animation_artist::particle::is_alive(int width, int height, float focus_dist) const
{
  int min_size = std::min(width, height);

  float delta = disc_radius(min_size) + blur_radius(min_size, focus_dist);
  int left    = ggo::round_to<int>(_pos.x() - delta);
  int right   = ggo::round_to<int>(_pos.x() + delta);
  int bottom  = ggo::round_to<int>(_pos.y() - delta);
  int top     = ggo::round_to<int>(_pos.y() + delta);
  ggo::rect_int pixel_rect = ggo::rect_int::from_left_right_bottom_top(left, right, bottom, top);
  
  return pixel_rect.left() < width;
}

//////////////////////////////////////////////////////////////
// PARTICLE 1

//////////////////////////////////////////////////////////////
ggo::rah_animation_artist::particle1::particle1(int width, int height, float focus_dist)
:
particle(width, height, focus_dist),
_polygon(std::make_shared<ggo::polygon2d_f>())
{    
  int points_count = ggo::rand<int>(3, 6);
  _radius_interpolators = ggo::array<ggo_radius_interpolator, 1>(points_count);
  for (int i = 0; i < points_count; ++i)
  {
    _polygon->add_point(0, 0);
  }
}

//////////////////////////////////////////////////////////////
void ggo::rah_animation_artist::particle1::update(int min_size)
{
  particle::update(min_size);

  for (int i = 0; i < _radius_interpolators.count(); ++i)
  {
    float radius = _radius_interpolators(i).update(1) * disc_radius(min_size);
    float angle = _angle + i * 2 * ggo::pi<float>() / _radius_interpolators.count();
    _polygon->get_point(i) = _pos + radius * ggo::vec2_f::from_angle(angle);
  }
}

//////////////////////////////////////////////////////////////
void ggo::rah_animation_artist::particle1::fill_multi_shapes(ggo::multi_shape_f & backgrounds, ggo::multi_shape_f & bodies, int min_size) const
{
  float border_size = 0.0025f * min_size / _dist;

  bodies.add_shape(_polygon);
  
  for(int i = 0; i < _polygon->get_points_count(); ++i)
  {
    const auto & p1 = _polygon->get_point(i);
    const auto & p2 = _polygon->get_point((i + 1) % _polygon->get_points_count());
    backgrounds.add_shape(std::make_shared<ggo::capsule_f>(p1, p2, border_size));
  }
}

//////////////////////////////////////////////////////////////
// PARTICLE 2

//////////////////////////////////////////////////////////////
ggo::rah_animation_artist::particle2::particle2(int width, int height, float focus_dist)
:
particle(width, height, focus_dist),
_point_count(ggo::rand<int>(5, 8))
{
}

//////////////////////////////////////////////////////////////
void ggo::rah_animation_artist::particle2::fill_multi_shapes(ggo::multi_shape_f & backgrounds, ggo::multi_shape_f & bodies, int min_size) const
{
  float border_size = 0.0025f * min_size / _dist;
  float segment_size = 0.01f * min_size / _dist;
  
  float radius = disc_radius(min_size);

  for (int i = 0; i < _point_count; ++i)
  {
    float angle = _angle + i * 2 * ggo::pi<float>() / _point_count;
    ggo::pos2_f p1 = ggo::vec2_f::from_angle(angle) * (radius / 2);
    ggo::pos2_f p2 = ggo::vec2_f::from_angle(angle) * (radius - border_size - segment_size);
    
    p1 += _pos;
    p2 += _pos;
  
    backgrounds.add_shape(std::make_shared<ggo::capsule_f>(p1, p2, segment_size + border_size));
    bodies.add_shape(std::make_shared<ggo::capsule_f>(p1, p2, segment_size));
  }
}

//////////////////////////////////////////////////////////////
// PARTICLE 3

//////////////////////////////////////////////////////////////
ggo::rah_animation_artist::particle3::particle3(int width, int height, float focus_dist)
:
particle(width, height, focus_dist)
{
  _width = _width_interpolator.update(1);
  _height = _height_interpolator.update(1);
}

//////////////////////////////////////////////////////////////
void ggo::rah_animation_artist::particle3::update(int min_size)
{
  particle::update(min_size);

  _width = _width_interpolator.update(1);
  _height = _height_interpolator.update(1);
}

//////////////////////////////////////////////////////////////
void ggo::rah_animation_artist::particle3::fill_multi_shapes(ggo::multi_shape_f & backgrounds, ggo::multi_shape_f & bodies, int min_size) const
{
  float radius = disc_radius(min_size);
  
  float border_size = 0.0025f * min_size / _dist;
  float segment_size = 0.01f * min_size / _dist;
  
  for (int i = 0; i < 32; ++i)
  {
    float angle1 = + i * ggo::pi<float>() / 16;
    float angle2 = + (i + 1) * ggo::pi<float>() / 16;
    
    float x1 = std::cos(angle1) * _width;
    float y1 = std::sin(angle1) * _height;
    float x2 = std::cos(angle2) * _width;
    float y2 = std::sin(angle2) * _height;
    
    x1 = ggo::map<float>(x1, -1, 1, -radius + (border_size + segment_size) / 2, radius - (border_size + segment_size) / 2);
    y1 = ggo::map<float>(y1, -1, 1, -radius + (border_size + segment_size) / 2, radius - (border_size + segment_size) / 2);
    x2 = ggo::map<float>(x2, -1, 1, -radius + (border_size + segment_size) / 2, radius - (border_size + segment_size) / 2);
    y2 = ggo::map<float>(y2, -1, 1, -radius + (border_size + segment_size) / 2, radius - (border_size + segment_size) / 2);
  
    ggo::pos2_f p1(x1, y1);
    ggo::pos2_f p2(x2, y2);
    
    p1 = ggo::rotate(p1, _angle);
    p2 = ggo::rotate(p2, _angle);
    
    p1 += _pos;
    p2 += _pos;
  
    backgrounds.add_shape(std::make_shared<ggo::capsule_f>(p1, p2, segment_size + border_size));
    bodies.add_shape(std::make_shared<ggo::capsule_f>(p1, p2, segment_size));
  }
}

//////////////////////////////////////////////////////////////
// PARTICLE 4

//////////////////////////////////////////////////////////////
ggo::rah_animation_artist::particle4::particle4(int width, int height, float focus_dist)
:
particle(width, height, focus_dist)
{
}

//////////////////////////////////////////////////////////////
void ggo::rah_animation_artist::particle4::fill_multi_shapes(ggo::multi_shape_f & backgrounds, ggo::multi_shape_f & bodies, int min_size) const
{
  float border_size = 0.0025f * min_size / _dist;
  float segment_size = 0.02f * min_size / _dist;
  
  float radius = disc_radius(min_size) - segment_size - border_size;

  {
    auto disc1 = std::make_shared<ggo::disc_f>(_pos, radius + 0.5f * segment_size);
    auto disc2 = std::make_shared<ggo::disc_f>(_pos, radius - 0.5f * segment_size);

    auto opened_disc = std::make_shared<ggo::multi_shape<float, ggo::boolean_mode::DIFFERENCE>>();
    opened_disc->add_shapes(disc1, disc2);

    bodies.add_shape(opened_disc);
  }
  
  {
    auto disc1 = std::make_shared<ggo::disc_f>(_pos, radius + 0.5f * segment_size + border_size);
    auto disc2 = std::make_shared<ggo::disc_f>(_pos, radius - 0.5f * segment_size - border_size);

    auto opened_disc = std::make_shared<ggo::multi_shape<float, ggo::boolean_mode::DIFFERENCE>>();
    opened_disc->add_shapes(disc1, disc2);

    backgrounds.add_shape(opened_disc);
  }

  for (int i = 0; i < 8; ++i)
  {
    float delta = radius + segment_size / 2;
    float angle = _angle + i * ggo::pi<float>() / 4;
    ggo::vec2_f offset(delta * std::cos(angle), delta * std::sin(angle));
  
    auto disc = std::make_shared<ggo::disc_f>();
    disc->center() = _pos + offset;
    disc->radius() = segment_size;
    bodies.add_shape(disc);
    
    auto disc_border = std::make_shared<ggo::disc_f>();
    disc_border->center() = _pos + offset;
    disc_border->radius() = segment_size + border_size;
    backgrounds.add_shape(disc_border);
  }
}

//////////////////////////////////////////////////////////////
// ARTIST

//////////////////////////////////////////////////////////////
ggo::rah_animation_artist::rah_animation_artist(int width, int height, int line_step, ggo::image_format format)
:
fixed_frames_count_animation_artist_abc(width, height, line_step, format, 500)
{
  _focus_dist_interpolator._near = near;
  _focus_dist_interpolator._far = far;
}

//////////////////////////////////////////////////////////////
void ggo::rah_animation_artist::insert_particle(std::shared_ptr<particle> particle)
{
  auto insert_it = std::upper_bound(_particles.begin(), _particles.end(), particle, [](const auto & particle1, const auto & particle2) { return particle1->_dist > particle2->_dist; });

  _particles.insert(insert_it, particle);
}

//////////////////////////////////////////////////////////////
void ggo::rah_animation_artist::render_frame(void * buffer, int frame_index, float time_step)
{
  _focus_dist = _focus_dist_interpolator.update(1);

  // Update items (far from near).
  for (auto & particle : _particles)
  {
    particle->update(min_size());
  }

  ggo::remove_if(_particles, [&](const auto & particle)
  {
    return particle->is_alive(width(), height(), _focus_dist) == false;
  });

  // Create new particles.
  int create_count = std::min(frame_index / 5, 20);
  for (int i = 0; i < create_count; ++i)
  {
    // Sorted insertion.
    auto particle = create_particle(_focus_dist, width(), height());
    insert_particle(particle);
  }

  // Paint background.
  if (buffer != nullptr)
  {    
    ggo::fill_solid<ggo::rgb_8u_yu>(buffer, width(), height(), line_step(), ggo::white_8u());
  }

  // Paint items (far from near).
  if (buffer != nullptr)
  {
    for (const auto & particle : _particles)
    {
      particle->paint(buffer, width(), height(), _focus_dist);
    }
  }

  std::cout << _particles.size() << std::endl;
}

//////////////////////////////////////////////////////////////
std::shared_ptr<ggo::rah_animation_artist::particle> ggo::rah_animation_artist::create_particle(float focus_dist,
                                                                                                int width,
                                                                                                int height)
{
  std::shared_ptr<particle> particle;
  
  switch (ggo::rand<int>(0, 3))
  {
  case 0:
    particle = std::make_shared<particle1>(width, height, focus_dist);
    break;
  case 1:
    particle = std::make_shared<particle2>(width, height, focus_dist);
    break;
  case 2:
    particle = std::make_shared<particle3>(width, height, focus_dist);
    break;
  case 3:
    particle = std::make_shared<particle4>(width, height, focus_dist);
    break;
  }
  
  return particle;
}

