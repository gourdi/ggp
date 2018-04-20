#include "ggo_lagaude_animation_artist.h"
#include <kernel/math/ggo_coordinates_conversions.h>
#include <2d/paint/ggo_paint.h>
#include <2d/fill/ggo_fill.h>
#include <2d/paint/ggo_brush.h>
#include <2d/paint/ggo_blend.h>

//////////////////////////////////////////////////////////////
ggo::lagaude_animation_artist::lagaude_animation_artist(int width, int height, int line_step, ggo::image_format format, rendering_type rt)
:
animation_artist_abc(width, height, line_step, format, rt)
{
	
}

//////////////////////////////////////////////////////////////
void ggo::lagaude_animation_artist::init_animation()
{
  _frame_index = -1;

  _bkgd_discs.clear();
  for (int i = 0; i < 100; ++i)
  {
    ggo::lagaude_animation_artist::bkgd_disc bkgd_disc;
    bkgd_disc._pos.x() = ggo::rand<float>(-1, 3);
    bkgd_disc._pos.y() = ggo::rand<float>(-1, 3);
    bkgd_disc._vel = ggo::from_polar(ggo::rand<float>(0, 2 * ggo::pi<float>()), 0.001f);
    bkgd_disc._radius = 0.75;

    _bkgd_discs.push_back(bkgd_disc);
  }

  _animator.clear();
  for (int i = 0; i < 1000; ++i)
  {
    float scale = ggo::rand<float>(0.5, 1);
    ggo::pos2f pos(ggo::rand<float>(-2, 2), ggo::rand<float>(-0.1f, 1.1f));

    ggo::lagaude_animation_artist::sinuoid_path * path = new ggo::lagaude_animation_artist::sinuoid_path();
    path->_amplitude = ggo::rand<float>(0.01f, 0.02f);
    path->_dx = ggo::rand<float>(0.001f, 0.002f);
    path->_frequency = ggo::rand<float>(0.02f, 0.05f);

    ggo::lagaude_animation_artist::dust * dust = new ggo::lagaude_animation_artist::dust(pos, path, scale);
    dust->_radius = ggo::rand<float>(0.004f, 0.008f);
    dust->_width = dust->_radius * ggo::rand<float>(0.1f, 0.5f);
    dust->_val = ggo::rand<float>(0, 0.5f);

    insert_scale_animator(dust);
  }

  _hue = ggo::rand<float>();
}

//////////////////////////////////////////////////////////////
bool ggo::lagaude_animation_artist::prepare_frame()
{
  ++_frame_index;

  if (_frame_index > 450)
  {
    return false;
  }

  // Update background.
  for (auto & bkgd_disc : _bkgd_discs)
  {
    bkgd_disc._pos += bkgd_disc._vel;
  }

  // Add seed.
  if ((_frame_index % 10 == 0) && (_frame_index < 350))
  {
    float scale = ggo::rand<float>(0.5, 1);
    ggo::pos2f pos(ggo::rand<float>(), ggo::rand<float>());
    ggo::path_abc * path = new ggo::velocity_path(scale * ggo::rand<float>(0.002f, 0.005f), ggo::rand<float>(0, 2 * ggo::pi<float>()));

    insert_scale_animator(new ggo::lagaude_animation_artist::seed(pos, path, scale, _hue));
  }

  _animator.update();

  return true;
}

//////////////////////////////////////////////////////////////
void ggo::lagaude_animation_artist::render_frame(void * buffer, const ggo::rect_int & clipping)
{
  // Render background.
  ggo::fill_solid<ggo::rgb_8u_yu>(buffer, get_width(), get_height(), get_line_step(), ggo::white_8u(), clipping);

  for (auto & bkgd_disc : _bkgd_discs)
  {
    float x = get_width() * bkgd_disc._pos.x();
    float y = get_height() * bkgd_disc._pos.y();
    float radius = get_min_size() * bkgd_disc._radius;

    ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(
      buffer, get_width(), get_height(), get_line_step(),
      ggo::disc_float(x, y, radius), ggo::black_brush_8u(), ggo::alpha_blender_rgb8u(0.1f));
  }

  _animator.render(buffer, get_width(), get_height(), get_line_step(), get_format());
}

//////////////////////////////////////////////////////////////
void ggo::lagaude_animation_artist::insert_scale_animator(ggo::lagaude_animation_artist::scale_animate_abc * scale_animator)
{
	// From smaller to largest scale.
	int i = 0;
	for (i = 0; i < _animator.get_animates_count(); ++i)
	{
		const auto * current = static_cast<const ggo::lagaude_animation_artist::scale_animate_abc *>(_animator.get_animate(i));
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
void ggo::lagaude_animation_artist::angle_generator::get_random_data(float & data, float & dt)
{
	data = ggo::rand<float>(-ggo::pi<float>() / 4, ggo::pi<float>() / 4);
	dt = ggo::rand<float>(3, 15);
}

//////////////////////////////////////////////////////////////
ggo::lagaude_animation_artist::seed::seed(const ggo::pos2f & pos, ggo::path_abc * path, float scale, float hue)
:
scale_animate_abc(pos, path, scale)
{
	_angle_generators.resize(ggo::rand<int>(3, 5));
	_life = ggo::rand<int>(80, 120);
	_dangle = ggo::rand<float>(-0.1f, 0.1f);
	_color = ggo::from_hsv<ggo::color_8u>(hue, ggo::rand<float>(0.5f, 1), ggo::rand<float>(0.5f, 1));
}

//////////////////////////////////////////////////////////////
bool ggo::lagaude_animation_artist::seed::update(int frame_index, const ggo::pos2f & pos)
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

      auto * particle = new ggo::lagaude_animation_artist::particle(pos + ggo::from_polar(angle, 0.02f * _scale), new ggo::velocity_path(0.02f * _scale, angle));
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
void ggo::lagaude_animation_artist::seed::render(void * buffer, int width, int height, int line_step, ggo::image_format format,
  int frame_index, const ggo::pos2f & pos) const
{
	_particles_animator.render(buffer, width, height, line_step, format);
}

//////////////////////////////////////////////////////////////
ggo::lagaude_animation_artist::particle::particle(const ggo::pos2f & pos, ggo::velocity_path * path)
:
ggo::path_animate_abc(pos, path)
{
}

//////////////////////////////////////////////////////////////
bool ggo::lagaude_animation_artist::particle::update(int frame_index, const ggo::pos2f & pos)
{
  _angle -= _dangle;
  _radius -= _dradius;

  return _radius >= 0;
}


//////////////////////////////////////////////////////////////
void ggo::lagaude_animation_artist::particle::render(void * buffer, int width, int height, int line_step, ggo::image_format format,
  int frame_index, const ggo::pos2f & pos) const
{
	ggo::pos2f p1 = ggo::map_fit(ggo::from_polar(_angle, _radius), 0.f, 1.f, width, height);
	ggo::pos2f p2 = ggo::map_fit(ggo::from_polar(_angle + 2 * ggo::pi<float>() / 3, _radius), 0.f, 1.f, width, height);
	ggo::pos2f p3 = ggo::map_fit(ggo::from_polar(_angle + 4 * ggo::pi<float>() / 3, _radius), 0.f, 1.f, width, height);
    
  ggo::vec2f disp(pos.x() * width, pos.y() * height);
  p1 += disp;
  p2 += disp;
  p3 += disp;    
	
  ggo::multi_shape_float multi_shape;
  
  float size = 0.003f * std::min(width, height);
  multi_shape.add_shape(std::make_shared<ggo::extended_segment_float>(p1, p2, size));
  multi_shape.add_shape(std::make_shared<ggo::extended_segment_float>(p2, p3, size));
  multi_shape.add_shape(std::make_shared<ggo::extended_segment_float>(p3, p1, size));

	ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(
    buffer, width, height, 3 * width, multi_shape, ggo::make_solid_brush(_color), ggo::alpha_blender_rgb8u(_opacity));
}

//////////////////////////////////////////////////////////////
bool ggo::lagaude_animation_artist::dust::update(int frame_index, const ggo::pos2f & pos)
{
  if (pos.x() > 1.1)
  {
    return false;
  }

  return true;
}

//////////////////////////////////////////////////////////////
void ggo::lagaude_animation_artist::dust::render(void * buffer, int width, int height, int line_step, ggo::image_format format,
  int frame_index, const ggo::pos2f & pos) const
{
  ggo::pos2f center(width * pos.x(), height * pos.y());
  float disc_radius = std::min(width, height) * _radius;
  float disc_width = std::min(width, height) * _width;

  auto disc1 = std::make_shared<ggo::disc_float>(center, disc_radius + 0.5f * disc_width);
  auto disc2 = std::make_shared<ggo::disc_float>(center, disc_radius - 0.5f * disc_width);

  ggo::multi_shape<float, ggo::boolean_mode::DIFFERENCE> opened_disc;
  opened_disc.add_shapes(disc1, disc2);

  ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(
    buffer, width, height, 3 * width, opened_disc, ggo::color_8u(ggo::round_to<uint8_t>(255.f * _val)));
}

