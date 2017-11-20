#include "ggo_smoke_animation_artist.h"
#include <ggo_kernel.h>
#include <ggo_buffer_fill.h>

#if 0

//////////////////////////////////////////////////////////////
ggo::smoke_animation_artist::loop_array2d::loop_array2d(int size_x, int size_y)
:
ggo::array<double, 2>(size_x, size_y)
{
}

//////////////////////////////////////////////////////////////
// Here this(0, 0) is really located at (0, 0), meaning that
// the array covers a (-0.5, size_x+0.5) x (-0.5, size_y+0.5)
// region.
double ggo::smoke_animation_artist::loop_array2d::interpolate(double x, double y) const
{
	// Bilinear interpolation.
	int src_left = x < 0 ? int(x) - 1 : int(x);
	int src_bottom = y < 0 ? int(y) - 1 : int(y);
	
	double alpha_x = x - src_left; 
	double alpha_y = y - src_bottom;
	GGO_ASSERT(alpha_x >= -0.0001 && alpha_x <= 1.0001);
	GGO_ASSERT(alpha_y >= -0.0001 && alpha_y <= 1.0001);
	
	src_left = ggo::pos_mod(src_left, get_size<0>());
	src_bottom = ggo::pos_mod(src_bottom, get_size<1>());
	
	int src_right = (src_left + 1) % get_size<0>();
	int src_top = (src_bottom + 1) % get_size<1>();

	double bottom = (1. - alpha_x) * this->operator()(src_left, src_bottom) + alpha_x * this->operator()(src_right, src_bottom);
	double top = (1. - alpha_x) * this->operator()(src_left, src_top) + alpha_x * this->operator()(src_right, src_top);

	return (1. - alpha_y) * bottom + alpha_y * top;
}

//////////////////////////////////////////////////////////////
double ggo::smoke_animation_artist::loop_array2d::loop_value(int x, int y) const
{
	x = ggo::pos_mod(x, get_size<0>());
	y = ggo::pos_mod(y, get_size<1>());
	
	return this->operator()(x, y);
}

#endif

//////////////////////////////////////////////////////////////
ggo::smoke_animation_artist::smoke_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt)
:
animation_artist_abc(width, height, line_step, pbf, rt)

#if 0
,
_velocity_x1(width + 1, height),
_velocity_x2(width + 1, height),
_velocity_y1(width, height + 1),
_velocity_y2(width, height + 1),
_density1(width, height),
_density2(width, height),
_bkgd_buffer(3 * width * height),
_sources(4)
#endif
{
}

//////////////////////////////////////////////////////////////
void ggo::smoke_animation_artist::init()
{
#if 0
  ++_frame_index;

  _velocity_x_cur = &_velocity_x1;
  _velocity_x_tmp = &_velocity_x2;
  _velocity_y_cur = &_velocity_y1;
  _velocity_y_tmp = &_velocity_y2;
  _density_cur = &_density1;
  _density_tmp = &_density2;

  _velocity_x_cur->fill(0);
  _velocity_y_cur->fill(0);
  _velocity_x_tmp->fill(0);
  _velocity_y_tmp->fill(0);
  _density_cur->fill(0);
  _density_tmp->fill(0);

  double center_x = get_width() / 2;
  double center_y = get_height() / 2;

  for (int i = 0; i < _sources.get_count(); ++i)
  {
    ggo::smoke_animation_artist::fluid_source & source = _sources(i);

    switch (i)
    {
    case 0:
      // Top.
      source._circle.center().get<0>() = center_x;
      source._circle.center().get<1>() = center_y + 0.25 * get_min_size();
      source._angle = -ggo::pi<float>() / 2;
      break;
    case 1:
      // Right.
      source._circle.center().get<0>() = center_x + 0.25 * get_min_size();
      source._circle.center().get<1>() = center_y;
      source._angle = ggo::pi<float>();
      break;
    case 2:
      // Bottom.
      source._circle.center().get<0>() = center_x;
      source._circle.center().get<1>() = center_y - 0.25 * get_min_size();
      source._angle = ggo::pi<float>() / 2;
      break;
    case 3:
      // Left.
      source._circle.center().get<0>() = center_x - 0.25 * get_min_size();
      source._circle.center().get<1>() = center_y;
      source._angle = 0;
      break;
    }

    source._circle.radius() = ggo::rand<float>(0.03f * get_min_size(), 0.04f * get_min_size());
    source._speed = ggo::rand<float>(0.007f * get_min_size(), 0.009f * get_min_size());
    source._timer1 = ggo::rand<int>(50, 100);
    source._timer2 = ggo::rand<int>(50, 150);
    source._angle_amplitude = ggo::rand<float>(ggo::pi<float>() / 4, ggo::pi<float>());
    source._angle_offset = ggo::rand<float>(0, 2 * ggo::pi<float>());
    source._wave_length = ggo::rand<float>(0.05f, 0.2f);
    source._density = 0;
  }

  ggo::smoke_animation_artist::fluid_source & source = _sources(ggo::rand<int>(0, 3));
  source._timer1 = 0;

  float hue = ggo::rand<float>();
  _smoke_color = ggo::from_hsv<ggo::color_8u>(hue, 1, ggo::rand<float>(0.2f, 0.8f));

  const ggo::color_8u color1 = ggo::from_hsv<ggo::color_8u>(hue, ggo::rand<float>(0.f, 0.2f), ggo::rand<float>(0.8f, 1.f));
  const ggo::color_8u color2 = ggo::from_hsv<ggo::color_8u>(hue, ggo::rand<float>(0.f, 0.2f), ggo::rand<float>(0.8f, 1.f));
  const ggo::color_8u color3 = ggo::from_hsv<ggo::color_8u>(hue, ggo::rand<float>(0.f, 0.2f), ggo::rand<float>(0.8f, 1.f));
  const ggo::color_8u color4 = ggo::from_hsv<ggo::color_8u>(hue, ggo::rand<float>(0.f, 0.2f), ggo::rand<float>(0.8f, 1.f));

  ggo::fill_4_colors<ggo::rgb_8u_yu>(_bkgd_buffer.data(), get_width(), get_height(), get_line_step(),
    color1, color2, color3, color4, ggo::pixel_rect::from_width_height(get_width(), get_height()));
#endif
}

//////////////////////////////////////////////////////////////
bool ggo::smoke_animation_artist::prepare_frame()
{
#if 0
  const int frames_count = 600;

  if (_frame_index > frames_count)
  {
    return false;
  }

  // Process the sources.
  for (auto & source : _sources)
  {
    if (source._timer1 > 0)
    {
      source._timer1 -= 1;
    }
    else if (source._timer2 > 0)
    {
      --source._timer2;
      source._density = std::min(source._density + 0.1, 1.0);
    }
    else if (source._density > 0)
    {
      source._density = std::max(source._density - 0.01, 0.0);
    }
  }

  for (int y = 0; y < get_height(); ++y)
  {
    for (int x = 0; x < get_width(); ++x)
    {
      for (const auto & source : _sources)
      {
        if (source._density > 0)
        {
          double dist = ggo::distance(double(x), double(y), source._circle.center().get<0>(), source._circle.center().get<1>());
          if (dist < source._circle.radius())
          {
            _density_cur->operator()(x, y) = std::min(_density_cur->operator()(x, y) + source._density, 1.0);
          }

          if (dist < 0.8 * source._circle.radius())
          {
            double angle = source._angle + source._angle_amplitude * std::cos(source._wave_length * _frame_index + source._angle_offset);

            _velocity_x_cur->operator()(x, y) = source._speed * std::cos(angle);
            _velocity_y_cur->operator()(x, y) = source._speed * std::sin(angle);
          }
        }
      }
    }
  }

  // Horizontal self advection.
  for (int y = 0; y < get_height(); ++y)
  {
    for (int x = 0; x < get_width() + 1; ++x)
    {
      double velocity_x = _velocity_x_cur->loop_value(x, y);
      double velocity_y = 0.25 * (_velocity_y_cur->loop_value(x, y) + _velocity_y_cur->loop_value(x, y + 1) + _velocity_y_cur->loop_value(x - 1, y) + _velocity_y_cur->loop_value(x - 1, y + 1));

      _velocity_x_tmp->operator()(x, y) = _velocity_x_cur->interpolate(x - velocity_x, y - velocity_y);
    }
  }

  std::swap(_velocity_x_tmp, _velocity_x_cur);

  // Vertical self advection.
  for (int y = 0; y < get_height() + 1; ++y)
  {
    for (int x = 0; x < get_width(); ++x)
    {
      double velocity_x = 0.25 * (_velocity_x_cur->loop_value(x, y) + _velocity_x_cur->loop_value(x, y - 1) + _velocity_x_cur->loop_value(x + 1, y) + _velocity_x_cur->loop_value(x + 1, y - 1));
      double velocity_y = _velocity_y_cur->loop_value(x, y);

      _velocity_y_tmp->operator()(x, y) = _velocity_y_cur->interpolate(x - velocity_x, y - velocity_y);
    }
  }

  std::swap(_velocity_y_tmp, _velocity_y_cur);

  // Compute pressure.
  ggo::smoke_animation_artist::loop_array2d divergence(get_width(), get_height());
  ggo::smoke_animation_artist::loop_array2d pressure(get_width(), get_height());
  for (int y = 0; y < get_height(); ++y)
  {
    for (int x = 0; x < get_width(); ++x)
    {
      divergence(x, y) = -(_velocity_x_cur->loop_value(x + 1, y) - _velocity_x_cur->loop_value(x, y) + _velocity_y_cur->loop_value(x, y + 1) - _velocity_y_cur->loop_value(x, y));
      divergence(x, y) *= get_min_size();
      pressure(x, y) = 0;
    }
  }

  // Solve pressure with Gauss-Seidel.
  for (int k = 0; k < 50; ++k)
  {
    for (int y = 0; y < get_height(); ++y)
    {
      for (int x = 0; x < get_width(); ++x)
      {
        pressure(x, y) = (divergence(x, y) + pressure.loop_value(x - 1, y) + pressure.loop_value(x + 1, y) + pressure.loop_value(x, y - 1) + pressure.loop_value(x, y + 1)) / 4;
      }
    }
  }

  // Apply pressure.
  for (int y = 0; y < get_height(); ++y)
  {
    for (int x = 0; x < get_width() + 1; ++x)
    {
      _velocity_x_cur->operator()(x, y) -= (pressure.loop_value(x, y) - pressure.loop_value(x - 1, y)) / get_min_size();
    }
  }

  for (int y = 0; y < get_height() + 1; ++y)
  {
    for (int x = 0; x < get_width(); ++x)
    {
      _velocity_y_cur->operator()(x, y) -= (pressure.loop_value(x, y) - pressure.loop_value(x, y - 1)) / get_min_size();
    }
  }

  // Density advection.
  for (int y = 0; y < get_height(); ++y)
  {
    for (int x = 0; x < get_width(); ++x)
    {
      // Backtracking.
      double velocity_x = 0.5 * (_velocity_x_cur->loop_value(x, y) + _velocity_x_cur->loop_value(x + 1, y));
      double velocity_y = 0.5 * (_velocity_y_cur->loop_value(x, y) + _velocity_y_cur->loop_value(x, y + 1));

      double density = _density_cur->interpolate(x - velocity_x, y - velocity_y);

      _density_tmp->operator()(x, y) = density;
    }
  }

  std::swap(_density_cur, _density_tmp);
#endif

  return true;
}

//////////////////////////////////////////////////////////////
void ggo::smoke_animation_artist::process_frame(void * buffer, const ggo::rect_int & clipping)
{
#if 0
	// Draw the density.
  for_each_pixel([&](int x, int y)
  {
    double density = _density_cur->operator()(x, y);

    ggo::color_8u c_8u = ggo::read_pixel<ggo::rgb_8u_yu>(_bkgd_buffer.data(), x, y, get_height(), 3 * get_width());
    c_8u.r() = ggo::to<uint8_t>(density * 255 * _smoke_color.r() + (1 - density) * c_8u.r());
    c_8u.g() = ggo::to<uint8_t>(density * 255 * _smoke_color.g() + (1 - density) * c_8u.g());
    c_8u.b() = ggo::to<uint8_t>(density * 255 * _smoke_color.b() + (1 - density) * c_8u.b());
    ggo::write_pixel<ggo::rgb_8u_yu>(buffer, x, y, get_height(), get_line_step(), c_8u);
  });
#endif
}
