#include "ggo_lagaude_realtime_artist.h"
#include <kernel/math/ggo_coordinates_conversions.h>
#include <2d/paint/ggo_paint.h>
#include <2d/fill/ggo_fill.h>
#include <2d/paint/ggo_brush.h>
#include <2d/paint/ggo_blend.h>

//////////////////////////////////////////////////////////////
ggo::lagaude_realtime_artist::lagaude_realtime_artist(int width, int height, int line_step, ggo::image_format format)
:
fixed_frames_count_realtime_artist_abc(width, height, line_step, format)
{
  for (int i = 0; i < 100; ++i)
  {
    ggo::lagaude_realtime_artist::bkgd_disc bkgd_disc;
    bkgd_disc._pos.x() = ggo::rand<float>(-1, 3);
    bkgd_disc._pos.y() = ggo::rand<float>(-1, 3);
    bkgd_disc._vel = 0.001f * ggo::vec2_f::from_angle(ggo::rand<float>(0, 2 * ggo::pi<float>()));
    bkgd_disc._radius = 0.75;

    _bkgd_discs.push_back(bkgd_disc);
  }

  for (int i = 0; i < 1000; ++i)
  {
    float scale = ggo::rand<float>(0.5, 1);
    ggo::pos2_f pos(ggo::rand<float>(-2, 2), ggo::rand<float>(-0.1f, 1.1f));

    auto * path = new ggo::lagaude_realtime_artist::sinuoid_path();
    path->_amplitude = ggo::rand<float>(0.01f, 0.02f);
    path->_dx = ggo::rand<float>(0.001f, 0.002f);
    path->_frequency = ggo::rand<float>(0.02f, 0.05f);

    auto * dust = new ggo::lagaude_realtime_artist::dust(pos, path, scale);
    dust->_radius = ggo::rand<float>(0.004f, 0.008f);
    dust->_width = dust->_radius * ggo::rand<float>(0.1f, 0.5f);
    dust->_val = ggo::rand<float>(0, 0.5f);

    insert_scale_animator(dust);
  }

  _hue = ggo::rand<float>();
}

//////////////////////////////////////////////////////////////
void ggo::lagaude_realtime_artist::preprocess_frame(int frame_index, uint32_t cursor_events, ggo::pos2_i cursor_pos)
{
  // Update background.
  for (auto & bkgd_disc : _bkgd_discs)
  {
    bkgd_disc._pos += bkgd_disc._vel;
  }

  // Add seed.
  if ((frame_index % 10 == 0) && (frame_index < 350))
  {
    float scale = ggo::rand<float>(0.5, 1);
    ggo::pos2_f pos(ggo::rand<float>(), ggo::rand<float>());
    ggo::path_abc * path = new ggo::velocity_path(scale * ggo::rand<float>(0.002f, 0.005f), ggo::rand<float>(0, 2 * ggo::pi<float>()));

    insert_scale_animator(new ggo::lagaude_realtime_artist::seed(pos, path, scale, _hue));
  }

  _animator.update();
}

//////////////////////////////////////////////////////////////
void ggo::lagaude_realtime_artist::render_tile(void * buffer, int frame_index, const ggo::rect_int & clipping)
{
  // Render background.
  switch (format())
  {
  case ggo::rgb_8u_yu:
    ggo::fill_solid<ggo::rgb_8u_yu>(buffer, width(), height(), line_step(), ggo::white_8u(), clipping);
    break;
  case ggo::bgrx_8u_yd:
    ggo::fill_solid<ggo::bgrx_8u_yd>(buffer, width(), height(), line_step(), ggo::white_8u(), clipping);
    break;
  }

  for (auto & bkgd_disc : _bkgd_discs)
  {
    float x = width() * bkgd_disc._pos.x();
    float y = height() * bkgd_disc._pos.y();
    float radius = min_size() * bkgd_disc._radius;

    switch (format())
    {
    case ggo::rgb_8u_yu:
      ggo::paint<ggo::rgb_8u_yu, ggo::sampling_4x4>(
        buffer, width(), height(), line_step(),
        ggo::disc_f({ x, y }, radius), ggo::black_brush_8u(), ggo::alpha_blender_rgb8u(0.1f), clipping);
      break;
    case ggo::bgrx_8u_yd:
      ggo::paint<ggo::bgrx_8u_yd, ggo::sampling_4x4>(
        buffer, width(), height(), line_step(),
        ggo::disc_f({ x, y }, radius), ggo::black_brush_8u(), ggo::alpha_blender_rgb8u(0.1f), clipping);
      break;
    }
  }

  _animator.render(buffer, width(), height(), line_step(), format(), clipping);
}

//////////////////////////////////////////////////////////////
void ggo::lagaude_realtime_artist::insert_scale_animator(ggo::lagaude_realtime_artist::scale_animate_abc * scale_animator)
{
	// From smaller to largest scale.
	int i = 0;
	for (i = 0; i < _animator.get_animates_count(); ++i)
	{
		const auto * current = static_cast<const ggo::lagaude_realtime_artist::scale_animate_abc *>(_animator.get_animate(i));
		if (current->get_scale() < scale_animator->get_scale())
		{
			break;
		}
	}
		
	if (i >= _animator.get_animates_count())
	{
		_animator.add_animate(scale_animator);
	}
	else 
	{
		_animator.insert_animate(scale_animator, i);
	}
}

//////////////////////////////////////////////////////////////
void ggo::lagaude_realtime_artist::angle_generator::get_random_data(float & data, float & dt)
{
	data = ggo::rand<float>(-ggo::pi<float>() / 4, ggo::pi<float>() / 4);
	dt = ggo::rand<float>(3, 15);
}

//////////////////////////////////////////////////////////////
ggo::lagaude_realtime_artist::seed::seed(const ggo::pos2_f & pos, ggo::path_abc * path, float scale, float hue)
:
scale_animate_abc(pos, path, scale)
{
	_angle_generators = ggo::array<angle_generator, 1>(ggo::rand<int>(3, 5));
	_life = ggo::rand<int>(80, 120);
	_dangle = ggo::rand<float>(-0.1f, 0.1f);
	_color = ggo::from_hsv<ggo::rgb_8u>(hue, ggo::rand<float>(0.5f, 1), ggo::rand<float>(0.5f, 1));
}

//////////////////////////////////////////////////////////////
bool ggo::lagaude_realtime_artist::seed::update(int frame_index, const ggo::pos2_f & pos)
{
  // Create the particles if not dead.
  float opacity = 1;
  if (frame_index > _life)
  {
    opacity = 1 - 0.15f * (frame_index - _life);
  }

  if (opacity > 0)
  {
    for (int i = 0; i < _angle_generators.count(); ++i)
    {
      float angle = _angle_generators(i).update(1) + 2 * ggo::pi<float>() * i / _angle_generators.count();

      auto * particle = new ggo::lagaude_realtime_artist::particle(pos + 0.02f * _scale * ggo::vec2_f::from_angle(angle), new ggo::velocity_path(0.02f * _scale, angle));
      particle->_angle = angle;
      particle->_dangle = _dangle;
      particle->_radius = 0.025f * _scale;
      particle->_dradius = 0.0025f;
      particle->_opacity = opacity;
      particle->_color = _color;

      _particles_animator.add_animate(particle);
    }
  }

  _particles_animator.update();

  return !_particles_animator.is_empty();
}

//////////////////////////////////////////////////////////////
void ggo::lagaude_realtime_artist::seed::render(void * buffer, int width, int height, int line_step, ggo::image_format format,
  const ggo::rect_int & clipping, int frame_index, const ggo::pos2_f & pos) const
{
	_particles_animator.render(buffer, width, height, line_step, format, clipping);
}

//////////////////////////////////////////////////////////////
ggo::lagaude_realtime_artist::particle::particle(const ggo::pos2_f & pos, ggo::velocity_path * path)
:
ggo::path_animate_abc(pos, path)
{
}

//////////////////////////////////////////////////////////////
bool ggo::lagaude_realtime_artist::particle::update(int frame_index, const ggo::pos2_f & pos)
{
  _angle -= _dangle;
  _radius -= _dradius;

  return _radius >= 0;
}


//////////////////////////////////////////////////////////////
void ggo::lagaude_realtime_artist::particle::render(void * buffer, int width, int height, int line_step, ggo::image_format format,
  const ggo::rect_int & clipping, int frame_index, const ggo::pos2_f & pos) const
{
	ggo::pos2_f p1 = ggo::map_fit(_radius * ggo::vec2_f::from_angle(_angle), 0.f, 1.f, width, height);
	ggo::pos2_f p2 = ggo::map_fit(_radius * ggo::vec2_f::from_angle(_angle + 2 * ggo::pi<float>() / 3), 0.f, 1.f, width, height);
	ggo::pos2_f p3 = ggo::map_fit(_radius * ggo::vec2_f::from_angle(_angle + 4 * ggo::pi<float>() / 3), 0.f, 1.f, width, height);
    
  ggo::vec2_f disp(pos.x() * width, pos.y() * height);
  p1 += disp;
  p2 += disp;
  p3 += disp;    
	
  ggo::multi_shape_f multi_shape;
  
  float size = 0.003f * std::min(width, height);
  multi_shape.add_shape(std::make_shared<ggo::capsule_f>(p1, p2, size));
  multi_shape.add_shape(std::make_shared<ggo::capsule_f>(p2, p3, size));
  multi_shape.add_shape(std::make_shared<ggo::capsule_f>(p3, p1, size));

  switch (format)
  {
  case ggo::rgb_8u_yu:
    ggo::paint<ggo::rgb_8u_yu, ggo::sampling_4x4>(
      buffer, width, height, line_step, multi_shape, ggo::make_solid_brush(_color), ggo::alpha_blender_rgb8u(_opacity), clipping);
    break;
  case ggo::bgrx_8u_yd:
    ggo::paint<ggo::bgrx_8u_yd, ggo::sampling_4x4>(
      buffer, width, height, line_step, multi_shape, ggo::make_solid_brush(_color), ggo::alpha_blender_rgb8u(_opacity), clipping);
    break;
  }
}

//////////////////////////////////////////////////////////////
bool ggo::lagaude_realtime_artist::dust::update(int frame_index, const ggo::pos2_f & pos)
{
  if (pos.x() > 1.1f)
  {
    return false;
  }

  return true;
}

//////////////////////////////////////////////////////////////
void ggo::lagaude_realtime_artist::dust::render(void * buffer, int width, int height, int line_step, ggo::image_format format,
  const ggo::rect_int & clipping, int frame_index, const ggo::pos2_f & pos) const
{
  ggo::pos2_f center(width * pos.x(), height * pos.y());
  float disc_radius = std::min(width, height) * _radius;
  float disc_width = std::min(width, height) * _width;

  auto disc1 = std::make_shared<ggo::disc_f>(center, disc_radius + 0.5f * disc_width);
  auto disc2 = std::make_shared<ggo::disc_f>(center, disc_radius - 0.5f * disc_width);

  ggo::multi_shape<float, ggo::boolean_mode::DIFFERENCE> opened_disc;
  opened_disc.add_shapes(disc1, disc2);

  switch (format)
  {
  case ggo::rgb_8u_yu:
    ggo::paint<ggo::rgb_8u_yu, ggo::sampling_4x4>(
      buffer, width, height, line_step, opened_disc, ggo::rgb_8u(ggo::round_to<uint8_t>(255.f * _val)), clipping);
    break;
  case ggo::bgrx_8u_yd:
    ggo::paint<ggo::bgrx_8u_yd, ggo::sampling_4x4>(
      buffer, width, height, line_step, opened_disc, ggo::rgb_8u(ggo::round_to<uint8_t>(255.f * _val)), clipping);
    break;
  }
}

