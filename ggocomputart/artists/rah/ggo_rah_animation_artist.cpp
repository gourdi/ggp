#include "ggo_rah_animation_artist.h"
#include <ggo_buffer_fill.h>
#include <ggo_multi_shape_paint.h>
#include <ggo_blur_paint.h>

namespace
{
  const float near = 1;
  const float far = 15;
  const int fog_count = 32;
}

//////////////////////////////////////////////////////////////
// FOG

//////////////////////////////////////////////////////////////
ggo::rah_animation_artist::fog::fog(int width, int height)
:
_position_interpolator(width, height)
{
}

//////////////////////////////////////////////////////////////
void ggo::rah_animation_artist::fog::update(int min_size)
{
  _position = _position_interpolator.update(1);
}

//////////////////////////////////////////////////////////////
void ggo::rah_animation_artist::fog::paint(void * buffer, int width, int height, float focus_dist) const
{
  float alpha = 0.04f;
  float sigma = ggo::square(0.25f * std::min(width, height));

  uint8_t * ptr = static_cast<uint8_t *>(buffer);
  
  for (int y = 0; y < height; ++y)
  {
    float dy2 = ggo::square(y - _position.y());
    
    for (int x = 0; x < width; ++x)
    {
      float dx2 = ggo::square(x - _position.x());
      float value = 1 - std::exp(-(dx2 + dy2) / sigma);
      
      ptr[0] = ggo::to<int>(alpha * value * 255 + (1 - alpha) * ptr[0]);
      ptr[1] = ggo::to<int>(alpha * value * 255 + (1 - alpha) * ptr[1]);
      ptr[2] = ggo::to<int>(alpha * value * 255 + (1 - alpha) * ptr[2]);
      
      ptr += 3;
    }
  }
}

//////////////////////////////////////////////////////////////
bool ggo::rah_animation_artist::fog::is_alive(int width, int height, float focus_dist) const
{
  return true;
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
  _color = ggo::from_hsv<ggo::color_8u>(ggo::rand<float>(), 1, 1);
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
  _pos.x() += 0.025f * min_size / _dist;
  _pos.y() += _vertical_offset_interpolator.update(1) * min_size / _dist;
  _angle += 0.1f;
}

//////////////////////////////////////////////////////////////
void ggo::rah_animation_artist::particle::paint(void * buffer, int width, int height, float focus_dist) const
{
  int min_size = std::min(width, height);
  
  ggo::multi_shape_float borders;
  ggo::multi_shape_float bodies;
  fill_multi_shapes(borders, bodies, min_size);

  // Paint bodies.
  auto paint_body_pixel_func = [&](int x, int y, int num, int den)
  {
    ggo::color_8u pixel = ggo::read_pixel<ggo::rgb_8u_yu>(buffer, x, y, height, 3 * width);

    uint8_t r = ggo::round_div(num * _color.r() + (den - num) * pixel.r(), den);
    uint8_t g = ggo::round_div(num * _color.g() + (den - num) * pixel.g(), den);
    uint8_t b = ggo::round_div(num * _color.b() + (den - num) * pixel.b(), den);

    ggo::write_pixel<ggo::rgb_8u_yu>(buffer, x, y, height, 3 * width, ggo::color_8u(r, g, b));
  };

  ggo::paint_blur_shape<ggo::blur_samples_type::disc_52_samples>(
    bodies, width, height, blur_radius(min_size, focus_dist), paint_body_pixel_func);

  // Paint borders.
  auto paint_border_pixel_func = [&](int x, int y, int num, int den)
  {
    ggo::color_8u pixel = ggo::read_pixel<ggo::rgb_8u_yu>(buffer, x, y, height, 3 * width);

    uint8_t r = ggo::round_div((den - num) * pixel.r(), den);
    uint8_t g = ggo::round_div((den - num) * pixel.g(), den);
    uint8_t b = ggo::round_div((den - num) * pixel.b(), den);

    ggo::write_pixel<ggo::rgb_8u_yu>(buffer, x, y, height, 3 * width, ggo::color_8u(r, g, b)); 
  };

  ggo::paint_blur_shape<ggo::blur_samples_type::disc_52_samples>(
    borders, width, height, blur_radius(min_size, focus_dist), paint_border_pixel_func);
}

//////////////////////////////////////////////////////////////
bool ggo::rah_animation_artist::particle::is_alive(int width, int height, float focus_dist) const
{
  int min_size = std::min(width, height);

  float delta = disc_radius(min_size) + blur_radius(min_size, focus_dist);
  int left    = ggo::to<int>(_pos.x() - delta);
  int right   = ggo::to<int>(_pos.x() + delta);
  int bottom  = ggo::to<int>(_pos.y() - delta);
  int top     = ggo::to<int>(_pos.y() + delta);
  ggo::pixel_rect pixel_rect = ggo::pixel_rect::from_left_right_bottom_top(left, right, bottom, top);
  
  return pixel_rect.left() < width;
}

//////////////////////////////////////////////////////////////
// PARTICLE 1

//////////////////////////////////////////////////////////////
ggo::rah_animation_artist::particle1::particle1(int width, int height, float focus_dist)
:
particle(width, height, focus_dist),
_polygon(std::make_shared<ggo::polygon2d_float>())
{    
  int points_count = ggo::rand<int>(3, 6);
  _radius_interpolators.resize(points_count);
  for (int i = 0; i < points_count; ++i)
  {
    _polygon->add_point(0, 0);
  }
}

//////////////////////////////////////////////////////////////
void ggo::rah_animation_artist::particle1::update(int min_size)
{
  particle::update(min_size);

  for (int i = 0; i < _radius_interpolators.get_count(); ++i)
  {
    float radius = _radius_interpolators(i).update(1) * disc_radius(min_size);
    float angle = _angle + i * 2 * ggo::pi<float>() / _radius_interpolators.get_count();
    _polygon->get_point(i) = _pos + ggo::from_polar(angle, radius);
  }
}

//////////////////////////////////////////////////////////////
void ggo::rah_animation_artist::particle1::fill_multi_shapes(ggo::multi_shape_float & borders, ggo::multi_shape_float & bodies, int min_size) const
{
  float border_size = 0.0025f * min_size / _dist;

  bodies.add_shape(_polygon);
  
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
ggo::rah_animation_artist::particle2::particle2(int width, int height, float focus_dist)
:
particle(width, height, focus_dist),
_point_count(ggo::rand<int>(5, 8))
{
}

//////////////////////////////////////////////////////////////
void ggo::rah_animation_artist::particle2::fill_multi_shapes(ggo::multi_shape_float & borders, ggo::multi_shape_float & bodies, int min_size) const
{
  float border_size = 0.0025f * min_size / _dist;
  float segment_size = 0.01f * min_size / _dist;
  
  float radius = disc_radius(min_size);

  for(int i = 0; i < _point_count; ++i)
  {
    float angle = _angle + i * 2 * ggo::pi<float>() / _point_count;
    ggo::pos2f p1 = ggo::from_polar(angle, radius / 2);
    ggo::pos2f p2 = ggo::from_polar(angle, radius - border_size - segment_size);
    
    p1 += _pos;
    p2 += _pos;
  
    borders.add_shape(std::make_shared<ggo::extended_segment_float>(p1, p2, segment_size + border_size));
    bodies.add_shape(std::make_shared<ggo::extended_segment_float>(p1, p2, segment_size));
  }
}


//////////////////////////////////////////////////////////////
// PARTICLE 3

//////////////////////////////////////////////////////////////
ggo::rah_animation_artist::particle3::particle3(int width, int height, float focus_dist)
:
particle(width, height, focus_dist)
{
}

//////////////////////////////////////////////////////////////
void ggo::rah_animation_artist::particle3::update(int min_size)
{
  particle::update(min_size);

  _width = _width_interpolator.update(1);
  _height = _height_interpolator.update(1);
}

//////////////////////////////////////////////////////////////
void ggo::rah_animation_artist::particle3::fill_multi_shapes(ggo::multi_shape_float & borders, ggo::multi_shape_float & bodies, int min_size) const
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
  
    ggo::pos2f p1(x1, y1);
    ggo::pos2f p2(x2, y2);
    
    p1 = ggo::rotate(p1, _angle);
    p2 = ggo::rotate(p2, _angle);
    
    p1 += _pos;
    p2 += _pos;
  
    borders.add_shape(std::make_shared<ggo::extended_segment_float>(p1, p2, segment_size + border_size));
    bodies.add_shape(std::make_shared<ggo::extended_segment_float>(p1, p2, segment_size));
  }
}

//////////////////////////////////////////////////////////////
ggo::rah_animation_artist::particle4::particle4(int width, int height, float focus_dist)
:
particle(width, height, focus_dist)
{
}

//////////////////////////////////////////////////////////////
void ggo::rah_animation_artist::particle4::fill_multi_shapes(ggo::multi_shape_float & borders, ggo::multi_shape_float & bodies, int min_size) const
{
  float border_size = 0.0025f * min_size / _dist;
  float segment_size = 0.02f * min_size / _dist;
  
  float radius = disc_radius(min_size) - segment_size - border_size;

  {
    auto disc1 = std::make_shared<ggo::disc_float>(_pos, radius + 0.5f * segment_size);
    auto disc2 = std::make_shared<ggo::disc_float>(_pos, radius - 0.5f * segment_size);

    auto opened_disc = std::make_shared<ggo::multi_shape<float, ggo::boolean_mode::DIFFERENCE>>();
    opened_disc->add_shapes(disc1, disc2);

    bodies.add_shape(opened_disc);
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
    float angle = _angle + i * ggo::pi<float>() / 4;
    ggo::vec2f offset(delta * std::cos(angle), delta * std::sin(angle));
  
    auto disc = std::make_shared<ggo::disc_float>();
    disc->center() = _pos + offset;
    disc->radius() = segment_size;
    bodies.add_shape(disc);
    
    auto disc_border = std::make_shared<ggo::disc_float>();
    disc_border->center() = _pos + offset;
    disc_border->radius() = segment_size + border_size;
    borders.add_shape(disc_border);
  }
}

//////////////////////////////////////////////////////////////
// ARTIST

//////////////////////////////////////////////////////////////
ggo::rah_animation_artist::rah_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt)
:
animation_artist_abc(width, height, line_step, pbf, rt)
{
  _focus_dist_interpolator._near = near;
  _focus_dist_interpolator._far = far;

  _sort_func = [](const rah_item_ptr & item1, const rah_item_ptr & item2)
  {
    return item1->_dist > item2->_dist;
  };
}


//////////////////////////////////////////////////////////////
void ggo::rah_animation_artist::init()
{
  _frame_index = -1;

  _items.clear();

  // Create fog planes.
  for (int i = 0; i < fog_count; ++i)
  {
    auto fog_item = std::make_shared<fog>(get_width(), get_height());
    fog_item->_dist = ggo::map(static_cast<float>(i + 1), 0.f, 31.f, near, far);

    _items.push_back(fog_item);
  }

  std::sort(_items.begin(), _items.end(), _sort_func);
}

//////////////////////////////////////////////////////////////
bool ggo::rah_animation_artist::update()
{
  ++_frame_index;

  _focus_dist = _focus_dist_interpolator.update(1);

  // Update items (far from near).
  for (auto & item : _items)
  {
    item->update(get_min_size());
  }

  ggo::remove_if(_items, [&](const rah_item_ptr & item)
  {
    return item->is_alive(get_width(), get_height(), _focus_dist) == false;
  });

  // Create new particles.
  if (_frame_index < 1000)
  {
    int create_count = std::min(_frame_index / 5, 12);
    for (int i = 0; i < create_count; ++i)
    {
      // Sorted insertion.
      auto particle = create_particle(_focus_dist, get_width(), get_height());
      _items.insert(std::upper_bound(_items.begin(), _items.end(), particle, _sort_func), particle);
    }

    return true;
  }
  else
  {
    // Check there is only fog items left.
    return ggo::find_if(_items, [](const rah_item_ptr & item_ptr) { return item_ptr->is_fog() == false; });
  }

  return false;
}

//////////////////////////////////////////////////////////////
void ggo::rah_animation_artist::render_frame(void * buffer, const ggo::pixel_rect & clipping)
{
  // Paint background.
  if (buffer != nullptr)
  {    
    ggo::fill_solid<ggo::rgb_8u_yu>(buffer, get_width(), get_height(), get_line_step(), ggo::white_8u(), clipping);
  }

  // Paint items (far from near).
  if (buffer != nullptr)
  {
    for (const auto & item : _items)
    {
      item->paint(buffer, get_width(), get_height(), _focus_dist);
    }
  }

  std::cout << _items.size() << std::endl;
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

