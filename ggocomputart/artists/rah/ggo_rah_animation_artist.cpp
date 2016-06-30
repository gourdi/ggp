#include "ggo_rah_animation_artist.h"
#include <ggo_fill.h>
#include <ggo_paint.h>
#include <ggo_pixel_rect.h>
#include <ggo_blur_pixel_sampler.h>

namespace
{
  const float NEAR = 1;
  const float FAR = 15;
  const int FOG_COUNT = 32;
}

//////////////////////////////////////////////////////////////
// FOG

//////////////////////////////////////////////////////////////
ggo_rah_animation_artist::ggo_fog::ggo_fog(int width, int height)
:
_position_interpolator(width, height)
{
}

//////////////////////////////////////////////////////////////
void ggo_rah_animation_artist::ggo_fog::update(int min_size)
{
  _position = _position_interpolator.update(1);
}

//////////////////////////////////////////////////////////////
void ggo_rah_animation_artist::ggo_fog::paint(uint8_t * buffer, int width, int height, float focus_dist) const
{
  float alpha = 0.04f;
  float sigma = ggo::square(0.25f * std::min(width, height));
  
  for (int y = 0; y < height; ++y)
  {
    float dy2 = ggo::square(y - _position.get<1>());
    
    for (int x = 0; x < width; ++x)
    {
      float dx2 = ggo::square(x - _position.get<0>());
      float value = 1 - std::exp(-(dx2 + dy2) / sigma);
      
      buffer[0] = ggo::to<int>(alpha * value * 255 + (1 - alpha) * buffer[0]);
      buffer[1] = ggo::to<int>(alpha * value * 255 + (1 - alpha) * buffer[1]);
      buffer[2] = ggo::to<int>(alpha * value * 255 + (1 - alpha) * buffer[2]);
      
      buffer += 3;
    }
  }
}

//////////////////////////////////////////////////////////////
bool ggo_rah_animation_artist::ggo_fog::is_alive(int width, int height, float focus_dist) const
{
  return true;
}

//////////////////////////////////////////////////////////////
// PARTICLE

//////////////////////////////////////////////////////////////
ggo_rah_animation_artist::ggo_particle::ggo_particle(int render_width, int render_height, float focus_dist)
{
  int min_size = std::min(render_width, render_height);

  _angle = ggo::rand_float(0,  2 *ggo::PI<float>());
  _dist = ggo::map<float>(std::sqrt(ggo::rand_float()), 0, 1, NEAR, FAR);
  float total_radius = disc_radius(min_size) + blur_radius(min_size, focus_dist);
  _pos.get<0>() = -total_radius;
  _pos.get<1>() = ggo::rand_float(0, static_cast<float>(render_height));
  _color = ggo::color::from_hsv(ggo::rand_float(), 1, 1);
}

//////////////////////////////////////////////////////////////
float ggo_rah_animation_artist::ggo_particle::blur_radius(int min_size, float focus_dist) const
{
  return 1 / std::sqrt(2.f) + 0.0005f * min_size * std::abs(_dist - focus_dist);
}

//////////////////////////////////////////////////////////////
float ggo_rah_animation_artist::ggo_particle::disc_radius(int min_size) const
{
  return 0.1f * min_size / _dist;
}

//////////////////////////////////////////////////////////////
void ggo_rah_animation_artist::ggo_particle::update(int min_size)
{
  _pos.get<0>() += 0.025f * min_size / _dist;
  _pos.get<1>() += _vertical_offset_interpolator.update(1) * min_size / _dist;
  _angle += 0.1f;
}

//////////////////////////////////////////////////////////////
void ggo_rah_animation_artist::ggo_particle::paint(uint8_t * buffer, int width, int height, float focus_dist) const
{
  int min_size = std::min(width, height);
  
  auto borders = std::make_shared<ggo::multi_shape_float>();
  auto shapes = std::make_shared<ggo::multi_shape_float>();
  fill_multi_shapes(*borders, *shapes, min_size);
  
  std::vector<ggo::rgb_layer> layers;
  layers.emplace_back(borders, ggo::color::BLACK);
  layers.emplace_back(shapes, _color);
  
  ggo::paint(buffer, width, height, layers, ggo::blur_pixel_sampler(blur_radius(min_size, focus_dist), 7));
}

//////////////////////////////////////////////////////////////
bool ggo_rah_animation_artist::ggo_particle::is_alive(int width, int height, float focus_dist) const
{
  int min_size = std::min(width, height);

  float delta = disc_radius(min_size) + blur_radius(min_size, focus_dist);
  int left    = ggo::to<int>(_pos.get<0>() - delta);
  int right   = ggo::to<int>(_pos.get<0>() + delta);
  int bottom  = ggo::to<int>(_pos.get<1>() - delta);
  int top     = ggo::to<int>(_pos.get<1>() + delta);
  ggo::pixel_rect pixel_rect = ggo::pixel_rect::from_left_right_bottom_top(left, right, bottom, top);
  
  return pixel_rect.left() < width;
}

//////////////////////////////////////////////////////////////
// PARTICLE 1

//////////////////////////////////////////////////////////////
ggo_rah_animation_artist::ggo_particle1::ggo_particle1(int render_width, int render_height, float focus_dist)
:
ggo_particle(render_width, render_height, focus_dist),
_polygon(std::make_shared<ggo::polygon2d_float>())
{    
  int points_count = ggo::rand_int(3, 6);
  _radius_interpolators.resize(points_count);
  for (int i = 0; i < points_count; ++i)
  {
    _polygon->add_point(0, 0);
  }
}

//////////////////////////////////////////////////////////////
void ggo_rah_animation_artist::ggo_particle1::update(int min_size)
{
  ggo_particle::update(min_size);

  for (int i = 0; i < _radius_interpolators.get_count(); ++i)
  {
    float radius = _radius_interpolators(i).update(1) * disc_radius(min_size);
    float angle = _angle + i * 2 * ggo::PI<float>() / _radius_interpolators.get_count();
    _polygon->get_point(i) = _pos + ggo::from_polar(angle, radius);
  }
}

//////////////////////////////////////////////////////////////
void ggo_rah_animation_artist::ggo_particle1::fill_multi_shapes(ggo::multi_shape_float & borders, ggo::multi_shape_float & shapes, int min_size) const
{
  float border_size = 0.0025f * min_size / _dist;

  shapes.add_shape(_polygon);
  
  for(int i = 0; i < _polygon->get_points_count(); ++i)
  {
    const auto & p1 = _polygon->get_point(i);
    const auto & p2 = _polygon->get_point((i + 1) % _polygon->get_points_count());
    borders.add_shape(std::make_shared<ggo::extended_segment_float>(p1, p2, border_size));
  }
}

//////////////////////////////////////////////////////////////
// PARTICLE 2

//////////////////////////////////////////////////////////////
ggo_rah_animation_artist::ggo_particle2::ggo_particle2(int render_width, int render_height, float focus_dist)
:
ggo_particle(render_width, render_height, focus_dist),
_point_count(ggo::rand_int(5, 8))
{
}

//////////////////////////////////////////////////////////////
void ggo_rah_animation_artist::ggo_particle2::fill_multi_shapes(ggo::multi_shape_float & borders, ggo::multi_shape_float & shapes, int min_size) const
{
  float border_size = 0.0025f * min_size / _dist;
  float segment_size = 0.01f * min_size / _dist;
  
  float radius = disc_radius(min_size);

  for(int i = 0; i < _point_count; ++i)
  {
    float angle = _angle + i * 2 * ggo::PI<float>() / _point_count;
    ggo::pos2f p1 = ggo::from_polar(angle, radius / 2);
    ggo::pos2f p2 = ggo::from_polar(angle, radius - border_size - segment_size);
    
    p1 += _pos;
    p2 += _pos;
  
    borders.add_shape(std::make_shared<ggo::extended_segment_float>(p1, p2, segment_size + border_size));
    shapes.add_shape(std::make_shared<ggo::extended_segment_float>(p1, p2, segment_size));
  }
}


//////////////////////////////////////////////////////////////
// PARTICLE 3

//////////////////////////////////////////////////////////////
ggo_rah_animation_artist::ggo_particle3::ggo_particle3(int render_width, int render_height, float focus_dist)
:
ggo_particle(render_width, render_height, focus_dist)
{
}

//////////////////////////////////////////////////////////////
void ggo_rah_animation_artist::ggo_particle3::update(int min_size)
{
  ggo_particle::update(min_size);

  _width = _width_interpolator.update(1);
  _height = _height_interpolator.update(1);
}

//////////////////////////////////////////////////////////////
void ggo_rah_animation_artist::ggo_particle3::fill_multi_shapes(ggo::multi_shape_float & borders, ggo::multi_shape_float & shapes, int min_size) const
{
  float radius = disc_radius(min_size);
  
  float border_size = 0.0025f * min_size / _dist;
  float segment_size = 0.01f * min_size / _dist;
  
  for (int i = 0; i < 32; ++i)
  {
    float angle1 = + i * ggo::PI<float>() / 16;
    float angle2 = + (i + 1) * ggo::PI<float>() / 16;
    
    float x1 = std::cos(angle1) * _width;
    float y1 = std::sin(angle1) * _height;
    float x2 = std::cos(angle2) * _width;
    float y2 = std::sin(angle2) * _height;
    
    x1 = ggo::map<float>(x1, -1, 1, -radius + (border_size + segment_size) / 2, radius - (border_size + segment_size) / 2);
    y1 = ggo::map<float>(y1, -1, 1, -radius + (border_size + segment_size) / 2, radius - (border_size + segment_size) / 2);
    x2 = ggo::map<float>(x2, -1, 1, -radius + (border_size + segment_size) / 2, radius - (border_size + segment_size) / 2);
    y2 = ggo::map<float>(y2, -1, 1, -radius + (border_size + segment_size) / 2, radius - (border_size + segment_size) / 2);
  
    ggo::pos2f p1(x1, y1);
    ggo::pos2f p2(x2, y2);
    
    p1 = ggo::rotate(p1, _angle);
    p2 = ggo::rotate(p2, _angle);
    
    p1 += _pos;
    p2 += _pos;
  
    borders.add_shape(std::make_shared<ggo::extended_segment_float>(p1, p2, segment_size + border_size));
    shapes.add_shape(std::make_shared<ggo::extended_segment_float>(p1, p2, segment_size));
  }
}

//////////////////////////////////////////////////////////////
ggo_rah_animation_artist::ggo_particle4::ggo_particle4(int render_width, int render_height, float focus_dist)
:
ggo_particle(render_width, render_height, focus_dist)
{
}

//////////////////////////////////////////////////////////////
void ggo_rah_animation_artist::ggo_particle4::fill_multi_shapes(ggo::multi_shape_float & borders, ggo::multi_shape_float & shapes, int min_size) const
{
  float border_size = 0.0025f * min_size / _dist;
  float segment_size = 0.02f * min_size / _dist;
  
  float radius = disc_radius(min_size) - segment_size - border_size;

  {
    auto disc1 = std::make_shared<ggo::disc_float>(_pos, radius + 0.5f * segment_size);
    auto disc2 = std::make_shared<ggo::disc_float>(_pos, radius - 0.5f * segment_size);

    auto opened_disc = std::make_shared<ggo::multi_shape<float, ggo::boolean_mode::DIFFERENCE>>();
    opened_disc->add_shapes(disc1, disc2);

    shapes.add_shape(opened_disc);
  }
  
  {
    auto disc1 = std::make_shared<ggo::disc_float>(_pos, radius + 0.5f * segment_size + border_size);
    auto disc2 = std::make_shared<ggo::disc_float>(_pos, radius - 0.5f * segment_size - border_size);

    auto opened_disc = std::make_shared<ggo::multi_shape<float, ggo::boolean_mode::DIFFERENCE>>();
    opened_disc->add_shapes(disc1, disc2);

    borders.add_shape(opened_disc);
  }

  for (int i = 0; i < 8; ++i)
  {
    float delta = radius + segment_size / 2;
    float angle = _angle + i * ggo::PI<float>() / 4;
    ggo::vec2f offset(delta * std::cos(angle), delta * std::sin(angle));
  
    auto disc = std::make_shared<ggo::disc_float>();
    disc->center() = _pos + offset;
    disc->radius() = segment_size;
    shapes.add_shape(disc);
    
    auto disc_border = std::make_shared<ggo::disc_float>();
    disc_border->center() = _pos + offset;
    disc_border->radius() = segment_size + border_size;
    borders.add_shape(disc_border);
  }
}

//////////////////////////////////////////////////////////////
// ARTIST

//////////////////////////////////////////////////////////////
ggo_rah_animation_artist::ggo_rah_animation_artist(int render_width, int render_height)
:
ggo_animation_artist_abc(render_width, render_height)
{
  _focus_dist_interpolator._near = NEAR;
  _focus_dist_interpolator._far = FAR;

  _sort_func = [](const ggo_rah_item_ptr & item1, const ggo_rah_item_ptr & item2)
  {
    return item1->_dist > item2->_dist;
  };
}

//////////////////////////////////////////////////////////////
void ggo_rah_animation_artist::init_sub()
{
  _items.clear();
  
  // Create fog planes.
  for (int i = 0; i < FOG_COUNT; ++i)
  {
    auto fog = std::make_shared<ggo_fog>(get_render_width(), get_render_height());
    fog->_dist = ggo::map(static_cast<float>(i + 1), 0.f, 31.f, NEAR, FAR);
    
    _items.push_back(fog);
  }

  std::sort(_items.begin(), _items.end(), _sort_func);
}

//////////////////////////////////////////////////////////////
bool ggo_rah_animation_artist::render_next_frame_sub(uint8_t * buffer, int frame_index)
{
  float focus_dist = _focus_dist_interpolator.update(1);
  
  // Paint background.
  if (buffer != nullptr)
  {    
    ggo::fill_solid_rgb(buffer, get_render_width() * get_render_height(), ggo::color::WHITE);
  }

  // Update and paint items (far from near).
  for (auto & item : _items)
  {
    item->update(get_render_min_size());
  }

  ggo::remove_if(_items, [&](const ggo_rah_item_ptr & item)
  {
    return item->is_alive(get_render_width(), get_render_height(), focus_dist) == false;
  });

  if (buffer != nullptr)
  {
    for (const auto & item : _items)
    {
      item->paint(buffer, get_render_width(), get_render_height(), focus_dist);
    }
  }

  std::cout << _items.size() << std::endl;
  
  // Create new particles.
  if (frame_index < 1000)
  {
    int create_count = std::min(frame_index / 5, 12);
    for (int i = 0; i < create_count; ++i)
    {
      // Sorted insertion.
      auto particle = create_particle(focus_dist, get_render_width(), get_render_height());
      _items.insert(std::upper_bound(_items.begin(), _items.end(), particle, _sort_func), particle);
    }

    return true;
  }
  else
  {
    // Check there is only fog items left.
    return ggo::find_if(_items, [](const ggo_rah_item_ptr & item_ptr) { return item_ptr->is_fog() == false; });
  }
}

//////////////////////////////////////////////////////////////
std::shared_ptr<ggo_rah_animation_artist::ggo_particle> ggo_rah_animation_artist::create_particle(float focus_dist,
                                                                                                  int render_width,
                                                                                                  int render_height)
{
  std::shared_ptr<ggo_particle> particle;
  
  switch (ggo::rand_int(0, 3))
  {
  case 0:
    particle = std::make_shared<ggo_particle1>(render_width, render_height, focus_dist);
    break;
  case 1:
    particle = std::make_shared<ggo_particle2>(render_width, render_height, focus_dist);
    break;
  case 2:
    particle = std::make_shared<ggo_particle3>(render_width, render_height, focus_dist);
    break;
  case 3:
    particle = std::make_shared<ggo_particle4>(render_width, render_height, focus_dist);
    break;
  }
  
  return particle;
}

