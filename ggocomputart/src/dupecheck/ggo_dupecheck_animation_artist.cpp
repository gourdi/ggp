#include "ggo_dupecheck_animation_artist.h"
#include <kernel/math/signal_processing/ggo_dct.h>
#include <kernel/math/ggo_harmonic_curve.h>
#include <2d/paint/ggo_paint.h>
#include <2d/fill/ggo_fill.h>
#include <2d/paint/ggo_brush.h>
#include <2d/paint/ggo_blend.h>
#include <animation/paths/ggo_velocity_path.h>
#include <animation/paths/ggo_spiral_path.h>

//////////////////////////////////////////////////////////////
ggo::dupecheck_animation_artist::dupecheck_animation_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order)
:
animation_artist_abc(width, height, line_byte_step, pixel_type, memory_lines_order)
{
  _colors[0] = ggo::rgb_32f(ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>());
  _colors[1] = ggo::rgb_32f(ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>());
  _colors[2] = ggo::rgb_32f(ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>());
  _colors[3] = ggo::rgb_32f(ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>());

  // Push background colors, depending of bitmap or animation rendering.
  _bkgd_colors.clear();
  for (int i = 0; i < 4; ++i)
  {
    ggo::dupecheck_animation_artist::background_color bkgd_color;

    bkgd_color._val = ggo::rand<float>(0.1f, 0.15f);
    bkgd_color._radius = ggo::rand<float>(0.1f, 0.5f);
    bkgd_color._y = ggo::rand<float>();
    bkgd_color._angle = ggo::rand<float>(0, 2 * ggo::pi<float>());
    bkgd_color._dangle = ggo::rand<float>(0.01f, 0.02f);
    bkgd_color._var = ggo::rand<float>(60, 90);

    _bkgd_colors.push_back(bkgd_color);
  }

  _points[0].x() = ggo::rand<float>() * width;
  _points[0].y() = ggo::rand<float>() * height;
  _points[1].x() = ggo::rand<float>() * width;
  _points[1].y() = ggo::rand<float>() * height;
  _points[2].x() = ggo::rand<float>() * width;
  _points[2].y() = ggo::rand<float>() * height;
  _points[3].x() = ggo::rand<float>() * width;
  _points[3].y() = ggo::rand<float>() * height;
}

//////////////////////////////////////////////////////////////
void ggo::dupecheck_animation_artist::render_frame(void * buffer, int frame_index, float time_step, bool & finished)
{
  const int counter_max = 400;

  // Check if the artist is finished.
  finished = false;
  if ((frame_index > counter_max) && (_animator.is_empty() == true))
  {
    finished = true;
    return;
  }

  // Background.
  for (auto & bkgd_color : _bkgd_colors)
  {
    bkgd_color._angle += bkgd_color._dangle;
  }

  // Create new anim item.
  ggo::pos2_f pos = get_position(frame_index);
  ggo::rgb_8u color = get_color(frame_index);
  if ((frame_index < counter_max) && ((frame_index % 4) == 0))
  {
    ggo::path_abc * path = nullptr;

    switch (ggo::rand<int>(0, 2))
    {
    case 0:
      break;
    case 1:
      path = new ggo::velocity_path(ggo::rand<float>(0.001f, 0.01f) * min_size(), ggo::rand<float>(0, 2 * ggo::pi<float>()));
      break;
    case 2:
      path = new ggo::spiral_path(ggo::rand<float>(-ggo::pi<float>() / 32, ggo::pi<float>() / 32), ggo::rand<float>(0.001f, 0.01f) * min_size());
      break;
    }

    ggo::animate_abc * animate = nullptr;
    switch (ggo::rand<int>(0, 2))
    {
    case 0:
      animate = new ggo::dupecheck_animation_artist::animate1(pos, path, color, min_size());
      break;
    case 1:
      animate = new ggo::dupecheck_animation_artist::animate2(pos, path, color, min_size());
      break;
    case 2:
      animate = new ggo::dupecheck_animation_artist::animate3(pos, path, color, min_size());
      break;
    default:
      GGO_FAIL();
      break;
    }

    _animator.add_animate(animate);
  }

  // Update anim items.
  _animator.update();

  // Background.
  if (buffer != nullptr)
  {
    uint8_t * ptr = static_cast<uint8_t *>(buffer);

    for (int y = 0; y < height(); ++y)
    {
      float v_f = 1;
      for (const auto & bkgd_color : _bkgd_colors)
      {
        float pos = bkgd_color._y + bkgd_color._radius * std::cos(bkgd_color._angle);
        float dpos = pos - y / float(height());
        v_f -= bkgd_color._val * std::exp(-(dpos * dpos) * bkgd_color._var);
      }
      uint8_t v = std::max(0, ggo::round_to<int>(255 * v_f));

      for (int x = 0; x < width(); ++x)
      {
        *ptr++ = v;
        *ptr++ = v;
        *ptr++ = v;
      }
    }

    // Render anim items.
    _animator.render(ggo::image(buffer, size(), pixel_type(), memory_lines_order(), line_byte_step()));
  }
}

//////////////////////////////////////////////////////////////
ggo::pos2_f ggo::dupecheck_animation_artist::get_position(int frame_index)
{
	const float scale = 0.015f;
	
	// Compute current position.
	float t = std::fmod(scale * frame_index, 1.f);
	
	ggo::pos2_f p;
	p  = t*t*t*(-1.f*_points[0] +  3.f*_points[1] + -3.f*_points[2] + _points[3]);
	p +=   t*t*( 3.f*_points[0] + -6.f*_points[1] +  3.f*_points[2]);
	p +=     t*(-3.f*_points[0]                   +  3.f*_points[2]);
	p +=             _points[0] +  4.f*_points[1] +      _points[2];
	p /= 6.f;		
	
	// Update spline points if needed.
	float t_next = std::fmod(scale * (frame_index + 1), 1.f);
	if (std::fmod(t, 1.f) > std::fmod(t_next, 1.f)) 
	{
		_points[0] = _points[1];
		_points[1] = _points[2];
		_points[2] = _points[3];
		_points[3].x() = ggo::rand<float>() * width();
		_points[3].y() = ggo::rand<float>() * height();
	}
	
	return p;
}

//////////////////////////////////////////////////////////////
ggo::rgb_8u ggo::dupecheck_animation_artist::get_color(int frame_index)
{
	const float scale = 0.01f;
	
	// Compute current position.
	float t = std::fmod(scale * frame_index, 1.f);
	
	ggo::rgb_32f c;
	c  = t*t*t*(-1.f*_colors[0] +  3.f*_colors[1] + -3.f*_colors[2] + _colors[3]);
	c +=   t*t*( 3.f*_colors[0] + -6.f*_colors[1] +  3.f*_colors[2]);
	c +=     t*(-3.f*_colors[0]                   +  3.f*_colors[2]);
	c +=             _colors[0] +  4.f*_colors[1] +      _colors[2];
	c /= 6.f;		
	
	// Update spline points if needed.
	float t_next = std::fmod(scale * (frame_index + 1), 1.f);
	if (std::fmod(t, 1.f) > std::fmod(t_next, 1.f)) 
	{
		_colors[0] = _colors[1];
		_colors[1] = _colors[2];
		_colors[2] = _colors[3];
		_colors[3] = ggo::rgb_32f(ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>());
	}
	
	c.r() = ggo::clamp<float>(c.r() + ggo::rand<float>(-0.1f, 0.1f), 0, 1);
	c.g() = ggo::clamp<float>(c.g() + ggo::rand<float>(-0.1f, 0.1f), 0, 1);
	c.b() = ggo::clamp<float>(c.b() + ggo::rand<float>(-0.1f, 0.1f), 0, 1);

	return ggo::convert_color_to<ggo::rgb_8u>(c);
}

//////////////////////////////////////////////////////////////
ggo::dupecheck_animation_artist::dupecheck_animate_abc::dupecheck_animate_abc(ggo::pos2_f pos, ggo::path_abc * path, const ggo::rgb_8u & color, int render_min_size)
:
ggo::path_animate_abc(pos, path)
{
	_color = color;
	_opacity = 1;
	_radius = 0;
	_dradius = ggo::rand<float>(0.0005f, 0.005f) * render_min_size;
}

//////////////////////////////////////////////////////////////
bool ggo::dupecheck_animation_artist::dupecheck_animate_abc::update(int frame_index, const ggo::pos2_f & pos)
{
	if (frame_index > 30) 
	{
		_opacity -= 0.1f;
	}
	
	if (_opacity < 0)
	{
		return false;
	}

	_radius += _dradius;
    
  update();
	
	return true;
}

//////////////////////////////////////////////////////////////
ggo::dupecheck_animation_artist::animate1::animate1(const ggo::pos2_f & pos, ggo::path_abc * path, const ggo::rgb_8u & color, int render_min_size)
:
dupecheck_animate_abc(pos, path, color, render_min_size)
{
	_count = ggo::rand<int>(1, 3);
	_width = ggo::rand<float>(0.002f, 0.02f) * render_min_size;
	_distance = _width * ggo::rand<float>(1.1f, 2);
	_opacity = 1;
}

//////////////////////////////////////////////////////////////
void ggo::dupecheck_animation_artist::animate1::render(ggo::image & img, const ggo::rect_int & clipping, int frame_index, const ggo::pos2_f & pos) const
{
  ggo::image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up> view(img.data(), img.size(), img.line_byte_step());

  for (int i = 0; i < _count; ++i)
  {
    float radius = _radius - i * _distance;
    if (radius > 0)
    {
      auto disc1 = std::make_shared<ggo::disc_f>(pos, radius + 0.5f * _width);
      auto disc2 = std::make_shared<ggo::disc_f>(pos, radius - 0.5f * _width);

      ggo::multi_shape<float, ggo::boolean_mode::DIFFERENCE> opened_disc;
      opened_disc.add_shape(disc1);
      opened_disc.add_shape(disc2);

      ggo::paint<ggo::sampling_4x4>(view, opened_disc, _color, _opacity);
    }
  }
}

//////////////////////////////////////////////////////////////
ggo::dupecheck_animation_artist::animate2::animate2(const ggo::pos2_f & pos, ggo::path_abc * path, const ggo::rgb_8u & color, int render_min_size)
:
dupecheck_animate_abc(pos, path, color, render_min_size)
{
	_width = ggo::rand<float>(0.001f, 0.01f) * render_min_size;
}

//////////////////////////////////////////////////////////////
void ggo::dupecheck_animation_artist::animate2::render(ggo::image & img, const ggo::rect_int & clipping, int frame_index, const ggo::pos2_f & pos) const
{
  ggo::image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up> view(img.data(), img.size(), img.line_byte_step());

  const int samples = 120;

  ggo::harmonic_curve<float> harmonic_curve;

  harmonic_curve.set_harmonic(ggo::rand<int>(8, 16), ggo::rand<float>(0.05f, 0.1f) * _radius, ggo::rand<float>(0, 2 * ggo::pi<float>()));
  harmonic_curve.set_harmonic(ggo::rand<int>(8, 16), ggo::rand<float>(0.05f, 0.1f) * _radius, ggo::rand<float>(0, 2 * ggo::pi<float>()));
  harmonic_curve.set_harmonic(ggo::rand<int>(8, 16), ggo::rand<float>(0.05f, 0.1f) * _radius, ggo::rand<float>(0, 2 * ggo::pi<float>()));
  harmonic_curve.set_harmonic(ggo::rand<int>(8, 16), ggo::rand<float>(0.05f, 0.1f) * _radius, ggo::rand<float>(0, 2 * ggo::pi<float>()));

  ggo::multi_shape_f multi_shape;

  for (int i = 0; i <= samples; ++i)
  {
    float angle1 = i * 2 * ggo::pi<float>() / samples;
    float angle2 = (i + 1) * 2 * ggo::pi<float>() / samples;
    float radius1 = _radius + harmonic_curve.evaluate(angle1);
    float radius2 = _radius + harmonic_curve.evaluate(angle2);

    ggo::pos2_f p1 = pos + radius1 * ggo::vec2_f::from_angle(angle1);
    ggo::pos2_f p2 = pos + radius2 * ggo::vec2_f::from_angle(angle2);

    auto segment = std::make_shared<ggo::capsule_f>(p1, p2, _width);

    multi_shape.add_shape(segment);
  }

  ggo::paint<ggo::sampling_4x4>(view, multi_shape, _color, _opacity);
}

//////////////////////////////////////////////////////////////
ggo::dupecheck_animation_artist::animate3::animate3(const ggo::pos2_f & pos, ggo::path_abc * path, const ggo::rgb_8u & color, int render_min_size)
:
dupecheck_animate_abc(pos, path, color, render_min_size)
{
	_shapes_count = ggo::rand<int>(5, 20);
	_vertices_count = ggo::rand<int>(3, 6);
	_angle = ggo::rand<float>(0, 2 * ggo::pi<float>());
	_dangle = ggo::rand<float>(-ggo::pi<float>() / 16, ggo::pi<float>() / 16);
	_angle_shape = ggo::rand<float>(0, 2 * ggo::pi<float>());
	_dangle_shape = ggo::rand<float>(-ggo::pi<float>() / 16, ggo::pi<float>() / 16);
	_shape_size = ggo::rand<float>(0.003f, 0.03f) * render_min_size;
}

//////////////////////////////////////////////////////////////
void ggo::dupecheck_animation_artist::animate3::update()
{
  _angle += _dangle;
  _angle_shape += _dangle_shape;
}

//////////////////////////////////////////////////////////////
void ggo::dupecheck_animation_artist::animate3::render(ggo::image & img, const ggo::rect_int & clipping, int frame_index, const ggo::pos2_f & pos) const
{
  ggo::image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up> view(img.data(), img.size(), img.line_byte_step());

  for (int i = 0; i < _shapes_count; ++i)
  {
    float angle_shape = _angle + i * 2 * ggo::pi<float>() / _shapes_count;
    ggo::pos2_f shape_pos = pos + _radius * ggo::vec2_f::from_angle(angle_shape);

    ggo::polygon2d_f shape;

    for (int j = 0; j < _vertices_count; ++j)
    {
      float angle_vertex = _angle_shape + j * 2 * ggo::pi<float>() / _vertices_count;
      ggo::pos2_f vertex = shape_pos + _shape_size * ggo::vec2_f::from_angle(angle_vertex);

      shape.add_point(vertex);
    }

    ggo::paint<ggo::sampling_4x4>(view, shape, _color, _opacity);
  }
}
