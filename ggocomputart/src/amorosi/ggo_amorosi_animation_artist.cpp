#include "ggo_amorosi_animation_artist.h"
#include <2d/paint/ggo_color_triangle.h>
#include <2d/paint/ggo_paint.h>
#include <2d/fill/ggo_fill.h>

//////////////////////////////////////////////////////////////
void ggo::amorosi_animation_artist::random_angle_interpolator::get_random_data(float & data, float & dt)
{
  data = ggo::rand<float>(0, 2 * ggo::pi<float>());
  dt = ggo::rand<float>(1, 2);
}

//////////////////////////////////////////////////////////////
void ggo::amorosi_animation_artist::random_width_interpolator::get_random_data(float & data, float & dt)
{
  data = ggo::rand<float>(0.001f, 0.025f);
  dt = ggo::rand<float>(2, 5);
}

//////////////////////////////////////////////////////////////
ggo::amorosi_animation_artist::curve::curve(int width, int height, const ggo::rgb_32f & color, ggo::ratio substeps_per_frame)
:
_lines_count(ggo::rand<int>(3, 10)),
_color(color),
_width(width),
_height(height),
_substeps_processing(substeps_per_frame)
{
  const int min_size = std::min(width, height);

  _speed = ggo::rand<float>(0.002f, 0.004f) * min_size;

  _prv_pos.x() = ggo::rand<float>(0.2f, 0.8f) * width;
  _prv_pos.y() = ggo::rand<float>(0.2f, 0.8f) * height;
  _prv_subangle = _subangle_interpolator.update(0.005f);
  _prv_width = _width_interpolator.update(0.05f) * min_size;

  _substeps_dead = ggo::rand(200, 500);
}

//////////////////////////////////////////////////////////////
bool ggo::amorosi_animation_artist::curve::is_dead() const
{
  return _triangles.empty(); 
}

//////////////////////////////////////////////////////////////
void ggo::amorosi_animation_artist::curve::update()
{
  const float opacity_dec = 0.975f;
  const int substeps = 5;
  const int min_size = std::min(_width, _height);

  _substeps_processing.call([&]
  {
    // Make already created triangle more transparants.
    for (auto & triangle : _triangles)
    {
      triangle[0]._opacity *= opacity_dec;
      triangle[1]._opacity *= opacity_dec;
      triangle[2]._opacity *= opacity_dec;
    }
     
    // Create new triangles.
    if (_substeps_processing._substeps_count < _substeps_dead)
    {
      float width = _width_interpolator.update(0.05f) * min_size;
      float padding = 0.5f * width;
      float prv_padding = 0.5f * _prv_width;
        
      float angle = _angle_interpolator.update(0.05f);
      ggo::vec2_f speed = _speed * ggo::vec2_f::from_angle(angle);
      ggo::pos2_f pos = _prv_pos + speed;
      
      float subangle = _subangle_interpolator.update(0.005f);
      ggo::vec2_f lateral = ggo::vec2_f::from_angle(subangle);
      ggo::vec2_f prv_lateral = ggo::vec2_f::from_angle(_prv_subangle);

      float offset = 0.5f * (_lines_count * width + (_lines_count - 1) * padding);
      float prv_offset = 0.5f * (_lines_count * _prv_width + (_lines_count - 1) * prv_padding);
      
      const float opacity_sup = 1;
      const float opacity_inf = opacity_sup * opacity_dec;

      for (int line = 0; line < _lines_count; ++line)
      {
        ggo::pos2_f p1 = _prv_pos + prv_offset * prv_lateral;
        ggo::pos2_f p2 = _prv_pos + (prv_offset + _prv_width) * prv_lateral;
        ggo::pos2_f p3 = pos + (offset + width) * lateral;
        ggo::pos2_f p4 = pos + offset * lateral;
        
        std::array<opacity_point, 3> triangle1;
        triangle1[0] = {p3, opacity_sup};
        triangle1[1] = {p4, opacity_sup};
        triangle1[2] = {p2, opacity_inf};
        _triangles.push_back(triangle1);
        
        std::array<opacity_point, 3> triangle2;
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
  });
  
  // Remove dead sprite triangle.
  ggo::remove_if(_triangles, [](std::array<opacity_point, 3> & triangle)
  {
    return triangle[0]._opacity <= 0.005 && triangle[1]._opacity <= 0.005 && triangle[2]._opacity <= 0.005;
  });
}

//////////////////////////////////////////////////////////////
void ggo::amorosi_animation_artist::curve::paint(void * buffer, int line_byte_step) const
{
  ggo::scene2d<ggo::rgb_8u> triangles;
  
  for (const auto & triangle : _triangles)
  {
    ggo::triangle2d_f shape(triangle[0]._pos, triangle[1]._pos, triangle[2]._pos);

    triangles.make<alpha_color_triangle<ggo::rgb_8u, float>>(shape,
      ggo::rgba_32f(_color.r(), _color.g(), _color.b(), triangle[0]._opacity),
      ggo::rgba_32f(_color.r(), _color.g(), _color.b(), triangle[1]._opacity),
      ggo::rgba_32f(_color.r(), _color.g(), _color.b(), triangle[2]._opacity));
  }

  ggo::image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up> img(buffer, { _width, _height }, line_byte_step);
  ggo::paint<ggo::sampling_4x4>(img, triangles);
}

//////////////////////////////////////////////////////////////
ggo::amorosi_animation_artist::amorosi_animation_artist(
  int width, int height, int line_byte_step,
  ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order,
  ggo::ratio fps)
:
animation_artist(width, height, line_byte_step, pixel_type, memory_lines_order),
_substeps_per_frame(125 / fps),
_fps(fps)
{
  _hue = ggo::rand<float>();
}

//////////////////////////////////////////////////////////////
ggo::rgb_32f ggo::amorosi_animation_artist::get_color() const
{
  float rnd = ggo::rand<float>();
  if (rnd < 0.5)
  {
    return from_hsv<ggo::rgb_32f>(_hue, 1, 1);
  }
  else
  {
    return ggo::white<ggo::rgb_32f>();
  }
}

//////////////////////////////////////////////////////////////
void ggo::amorosi_animation_artist::render_frame(void * buffer, bool & finished)
{
  constexpr int duration = 8;

  if (_elapsed_time == 0)
  {
    _curves.push_back(std::unique_ptr<curve>(new curve(width(), height(), get_color(), _substeps_per_frame)));
  }

  finished = false;
  if (_elapsed_time >= duration && _curves.empty())
  {
    finished = true;
    return;
  }

  if (ggo::rand<float>() < 0.05 && _elapsed_time < duration)
  {
    _curves.push_back(std::unique_ptr<curve>(new curve(width(), height(), get_color(), _substeps_per_frame)));
  }

  for (auto & curve : _curves)
  {
    curve->update();
  }

  ggo::remove_if(_curves, [](const std::unique_ptr<curve> & curve)
  {
    return curve->is_dead();
  });

  ggo::fill_black(ggo::image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up>(buffer, size(), line_byte_step()));

  if (buffer != nullptr)
  {
    for (const auto & curve : _curves)
    {
      curve->paint(buffer, line_byte_step());
    }
  }

  _hue += 0.001f;
  _elapsed_time += 1 / _fps;
}

