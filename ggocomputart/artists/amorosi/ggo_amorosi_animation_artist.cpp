#include "ggo_amorosi_animation_artist.h"
#include <ggo_fill.h>
#include <ggo_paint.h>
#include <ggo_triangle_interpolation_brush.h>
#include <ggo_shape_sampling_coef.h>

//////////////////////////////////////////////////////////////
void ggo_amorosi_animation_artist::ggo_random_angle_interpolator::get_random_data(float & data, float & dt)
{
  data = ggo::rand_float(0, 2 * ggo::PI<float>());
  dt = ggo::rand_float(1, 2);
}

//////////////////////////////////////////////////////////////
void ggo_amorosi_animation_artist::ggo_random_width_interpolator::get_random_data(float & data, float & dt)
{
  data = ggo::rand_float(0.001f, 0.025f);
  dt = ggo::rand_float(2, 5);
}

//////////////////////////////////////////////////////////////
ggo_amorosi_animation_artist::ggo_curve::ggo_curve(int render_width, int render_height, const ggo::color & color)
:
ggo_artist_abc(render_width, render_height),
_lines_count(ggo::rand_int(3, 10)),
_color(color)
{
  _counter = ggo::rand_int(100, 200);
  _speed = ggo::rand_float(0.002f, 0.004f) * get_render_min_size();

  _prv_pos.x() = ggo::rand_float(0.2f, 0.8f) * render_width;
  _prv_pos.y() = ggo::rand_float(0.2f, 0.8f) * render_height;
  _prv_subangle = _subangle_interpolator.update(0.005f);
  _prv_width = _width_interpolator.update(0.05f) * get_render_min_size();
}

//////////////////////////////////////////////////////////////
bool ggo_amorosi_animation_artist::ggo_curve::is_dead() const
{
  return _triangles.empty(); 
}

//////////////////////////////////////////////////////////////
void ggo_amorosi_animation_artist::ggo_curve::update()
{
  const float OPACITY_DEC = 0.975f;
  const int SUBSTEPS = 5;

  --_counter;
    
  if (_counter <= 25)
  {
    _speed *= 0.9f;
  }
    
  for (int i = 0; i < SUBSTEPS; ++i)
  {
    // Make already created triangle more transparants.
    for (auto & triangle : _triangles)
    {
      triangle[0]._opacity *= OPACITY_DEC;
      triangle[1]._opacity *= OPACITY_DEC;
      triangle[2]._opacity *= OPACITY_DEC;
    }
     
    // Create new triangles.
    if (_counter > 0)
    {
      float width = _width_interpolator.update(0.05f) * get_render_min_size();
      float padding = 0.5f * width;
      float prv_padding = 0.5f * _prv_width;
        
      float angle = _angle_interpolator.update(0.05f);
      ggo::vector2d_float speed = ggo::vector2d_float::from_polar(angle, _speed);
      ggo::point2d_float pos = _prv_pos + speed;
      
      float subangle = _subangle_interpolator.update(0.005f);
      ggo::vector2d_float lateral = ggo::vector2d_float::from_polar(subangle, 1);
      ggo::vector2d_float prv_lateral = ggo::vector2d_float::from_polar(_prv_subangle, 1);

      float offset = 0.5f * (_lines_count * width + (_lines_count - 1) * padding);
      float prv_offset = 0.5f * (_lines_count * _prv_width + (_lines_count - 1) * prv_padding);
      
      const float opacity_sup = 1;
      const float opacity_inf = opacity_sup * OPACITY_DEC;

      for (int line = 0; line < _lines_count; ++line)
      {
        ggo::point2d_float p1 = _prv_pos + prv_offset * prv_lateral;
        ggo::point2d_float p2 = _prv_pos + (prv_offset + _prv_width) * prv_lateral;
        ggo::point2d_float p3 = pos + (offset + width) * lateral;
        ggo::point2d_float p4 = pos + offset * lateral;
        
        std::array<ggo_opacity_point, 3> triangle1;
        triangle1[0] = {p3, opacity_sup};
        triangle1[1] = {p4, opacity_sup};
        triangle1[2] = {p2, opacity_inf};
        _triangles.push_back(triangle1);
        
        std::array<ggo_opacity_point, 3> triangle2;
        triangle2[0] = {p1, opacity_inf};
        triangle2[1] = {p2, opacity_inf};
        triangle2[2] = {p4, opacity_sup};
        _triangles.push_back(triangle2);
          
        offset += width + padding;
        prv_offset += _prv_width + prv_padding;
      }

      _prv_subangle = subangle;
      _prv_pos = pos;
      _prv_width = width;
    }
  }
  
  // Remove dead sprite triangle.
  ggo::remove_if(_triangles, [](std::array<ggo_opacity_point, 3> & triangle)
  {
    return triangle[0]._opacity <= 0.005 && triangle[1]._opacity <= 0.005 && triangle[2]._opacity <= 0.005;
  });
}

//////////////////////////////////////////////////////////////
void ggo_amorosi_animation_artist::ggo_curve::paint(uint8_t * buffer) const
{
  std::vector<ggo::rgb_layer> layers;
  
  for (const auto & triangle : _triangles)
  {
    auto triangle_shape = std::make_shared<ggo::triangle2d_float>(triangle[0]._pos, triangle[1]._pos, triangle[2]._pos);

    auto color_brush = std::make_shared<ggo::rgb_solid_brush>(_color);
    
    auto opacity_brush = std::make_shared<ggo::opacity_triangle_interpolation_brush>(triangle[0]._pos, triangle[0]._opacity,
                                                                                     triangle[1]._pos, triangle[1]._opacity, 
                                                                                     triangle[2]._pos, triangle[2]._opacity);

    layers.emplace_back(triangle_shape, color_brush, opacity_brush);
  }
  
  ggo::paint(make_image_buffer(buffer), layers);
}

//////////////////////////////////////////////////////////////
ggo_amorosi_animation_artist::ggo_amorosi_animation_artist(int render_width, int render_height)
:
ggo_static_background_animation_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo_amorosi_animation_artist::init_sub()
{
  _hue = ggo::rand_float();
  _curves.push_back(std::make_shared<ggo_curve>(get_render_width(), get_render_height(), get_color()));
}

//////////////////////////////////////////////////////////////
void ggo_amorosi_animation_artist::init_bkgd_buffer(uint8_t * buffer)
{
  ggo::fill_solid_rgb(buffer, get_render_width() * get_render_height(), ggo::color::BLACK);
}

//////////////////////////////////////////////////////////////
ggo::color ggo_amorosi_animation_artist::get_color() const
{
  float rnd = ggo::rand_float();
  if (rnd < 0.5)
  {
    return ggo::color::from_hsv(_hue, 1, 1);
  }
  else
  {
    return ggo::color::WHITE;
  }
}

//////////////////////////////////////////////////////////////
bool ggo_amorosi_animation_artist::render_next_frame_bkgd(uint8_t * buffer, int frame_index)
{
  const int FRAMES_COUNT = 300;

  if (frame_index > FRAMES_COUNT && _curves.empty())
  {
    return false;
  }

  if (ggo::rand_float() < 0.05 && frame_index < FRAMES_COUNT)
  {
    _curves.push_back(std::make_shared<ggo_curve>(get_render_width(), get_render_height(), get_color()));
  }

  for (auto & curve : _curves)
  {
    curve->update();
  }

  ggo::remove_if(_curves, [](const std::shared_ptr<ggo_curve> curve)
  {
    return curve->is_dead();
  });

  if (buffer != nullptr)
  {
    for (const auto & curve : _curves)
    {
      curve->paint(buffer);
    }
  }
  
  _hue += 0.001f;
    
  return true;
}
