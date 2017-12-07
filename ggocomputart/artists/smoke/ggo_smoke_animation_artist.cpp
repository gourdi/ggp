#include "ggo_smoke_animation_artist.h"
#include <ggo_kernel.h>
#include <ggo_buffer_fill.h>

namespace
{
  constexpr double time_step = 1. / 25.;

  //////////////////////////////////////////////////////////////
  // Here this(0, 0) is really located at (0, 0), meaning that
  // the array covers a (-0.5, size_x+0.5) x (-0.5, size_y+0.5)
  // region.
  double interpolate(const ggo::array<double, 2> & grid, double x, double y)
  {
    // Bilinear interpolation.
    int src_left = x < 0 ? int(x) - 1 : int(x);
    int src_bottom = y < 0 ? int(y) - 1 : int(y);

    double alpha_x = x - src_left;
    double alpha_y = y - src_bottom;
    GGO_ASSERT(alpha_x >= -0.0001 && alpha_x <= 1.0001);
    GGO_ASSERT(alpha_y >= -0.0001 && alpha_y <= 1.0001);

    src_left = ggo::pos_mod(src_left, grid.get_width());
    src_bottom = ggo::pos_mod(src_bottom, grid.get_height());

    int src_right = (src_left + 1) % grid.get_width();
    int src_top = (src_bottom + 1) % grid.get_height();
    double bottom = (1. - alpha_x) * grid(src_left, src_bottom) + alpha_x * grid(src_right, src_bottom);
    double top = (1. - alpha_x) * grid(src_left, src_top) + alpha_x * grid(src_right, src_top);
    return (1. - alpha_y) * bottom + alpha_y * top;
  }
}

//////////////////////////////////////////////////////////////
ggo::smoke_animation_artist::smoke_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt)
:
animation_artist_abc(width, height, line_step, pbf, rt),
_velocity_x1(width + 1, height),
_velocity_x2(width + 1, height),
_velocity_y1(width, height + 1),
_velocity_y2(width, height + 1),
_opacity1(width, height),
_opacity2(width, height),
_bkgd_buffer(3 * width * height),
_sources(4)
{
}

//////////////////////////////////////////////////////////////
void ggo::smoke_animation_artist::init()
{
  _frame_index = 0;

  _velocity_x_cur = &_velocity_x1;
  _velocity_x_tmp = &_velocity_x2;
  _velocity_y_cur = &_velocity_y1;
  _velocity_y_tmp = &_velocity_y2;
  _opacity_cur = &_opacity1;
  _opacity_tmp = &_opacity2;

  _velocity_x_cur->fill(0);
  _velocity_y_cur->fill(0);
  _velocity_x_tmp->fill(0);
  _velocity_y_tmp->fill(0);
  _opacity_cur->fill(0);
  _opacity_tmp->fill(0);

  const double area = 1.;
  _cell_size = area / std::sqrt(double(get_width()) * double(get_height()));

  _density = 10000.;

  for (int i = 0; i < _sources.get_count(); ++i)
  {
    ggo::smoke_animation_artist::fluid_source & source = _sources(i);

    switch (i)
    {
    case 0:
      // Top.
      source._disc.center() = { 0.5, 0.75 };
      source._angle = -ggo::pi<double>() / 2;
      break;
    case 1:
      // Right.
      source._disc.center() = { 0.5, 0.25 };
      source._angle = ggo::pi<double>();
      break;
    case 2:
      // Bottom.
      source._disc.center() = { 0.75, 0.5 };
      source._angle = ggo::pi<double>() / 2;
      break;
    case 3:
      // Left.
      source._disc.center() = { 0.25, 0.5 };
      source._angle = 0;
      break;
    }

    source._disc.radius() = ggo::rand<double>(0.03, 0.04);
    source._speed = 0.1; //ggo::rand<double>(0.001, 0.002);
    source._timer1 = ggo::rand<int>(50, 100);
    source._timer2 = ggo::rand<int>(50, 150);
    source._angle_amplitude = ggo::rand<double>(ggo::pi<double>() / 4, ggo::pi<double>());
    source._angle_offset = ggo::rand<double>(0, 2 * ggo::pi<double>());
    source._wave_length = ggo::rand<double>(0.05, 0.2);
    source._opacity = 0.;
  }

  ggo::smoke_animation_artist::fluid_source & source = _sources(ggo::rand<int>(0, 3));
  source._timer1 = 0;

  float hue = ggo::rand<float>();
  _smoke_color = ggo::from_hsv<ggo::color_8u>(hue, 1, ggo::rand<float>(0.2f, 0.8f));

  const ggo::color_8u color1 = ggo::from_hsv<ggo::color_8u>(hue, ggo::rand<float>(0.f, 0.2f), ggo::rand<float>(0.8f, 1.f));
  const ggo::color_8u color2 = ggo::from_hsv<ggo::color_8u>(hue, ggo::rand<float>(0.f, 0.2f), ggo::rand<float>(0.8f, 1.f));
  const ggo::color_8u color3 = ggo::from_hsv<ggo::color_8u>(hue, ggo::rand<float>(0.f, 0.2f), ggo::rand<float>(0.8f, 1.f));
  const ggo::color_8u color4 = ggo::from_hsv<ggo::color_8u>(hue, ggo::rand<float>(0.f, 0.2f), ggo::rand<float>(0.8f, 1.f));

  ggo::fill_4_colors<ggo::rgb_8u_yu>(_bkgd_buffer.data(), get_width(), get_height(), get_line_step(), color1, color2, color3, color4);
}

//////////////////////////////////////////////////////////////
void ggo::smoke_animation_artist::process_sources()
{
  for (auto & source : _sources)
  {
    if (source._timer1 > 0)
    {
      source._timer1 -= 1;
    }
    else if (source._timer2 > 0)
    {
      source._timer2 -= 1;
      source._opacity = std::min(source._opacity + 0.1, 1.0);
    }
    else if (source._opacity > 0.)
    {
      source._opacity = std::max(source._opacity - 0.01, 0.0);
    }
  }

  for (int y = 0; y < get_height(); ++y)
  {
    for (int x = 0; x < get_width(); ++x)
    {
      for (const auto & source : _sources)
      {
        if (source._opacity > 0)
        {
          ggo::disc_double disc(source._disc);
          map_fit(disc, 0., 1.);

          double dist = ggo::distance({ static_cast<double>(x), static_cast<double>(y) }, disc.get_center());
          if (dist < disc.radius())
          {
            _opacity_cur->at(x, y) = std::min(_opacity_cur->at(x, y) + source._opacity, 1.0);
          }

          if (dist < 0.8 * disc.radius())
          {
            double angle = source._angle + source._angle_amplitude * std::cos(source._wave_length * _frame_index + source._angle_offset);

            _velocity_x_cur->at(x, y) = source._speed * std::cos(angle);
            _velocity_y_cur->at(x, y) = source._speed * std::sin(angle);
          }
        }
      }
    }
  }
}

//////////////////////////////////////////////////////////////
void ggo::smoke_animation_artist::velocity_self_advection()
{
  double scale = time_step / _cell_size;

  // Horizontal self advection.
  for (int y = 0; y < get_height(); ++y)
  {
    for (int x = 0; x < get_width() + 1; ++x)
    {
      double velocity_x = _velocity_x_cur->at_loop(x, y);
      double velocity_y = 0.25 * (_velocity_y_cur->at_loop(x, y) + _velocity_y_cur->at_loop(x, y + 1) + _velocity_y_cur->at_loop(x - 1, y) + _velocity_y_cur->at_loop(x - 1, y + 1));

      double dx = velocity_x * scale;
      double dy = velocity_y * scale;

      double src_x = static_cast<double>(x) - dx;
      double src_y = static_cast<double>(y) - dy;

      _velocity_x_tmp->at(x, y) = interpolate(*_velocity_x_cur, src_x, src_y);
    }
  }

  std::swap(_velocity_x_tmp, _velocity_x_cur);

  // Vertical self advection.
  for (int y = 0; y < get_height() + 1; ++y)
  {
    for (int x = 0; x < get_width(); ++x)
    {
      double velocity_x = 0.25 * (_velocity_x_cur->at_loop(x, y) + _velocity_x_cur->at_loop(x, y - 1) + _velocity_x_cur->at_loop(x + 1, y) + _velocity_x_cur->at_loop(x + 1, y - 1));
      double velocity_y = _velocity_y_cur->at_loop(x, y);

      double dx = velocity_x * scale;
      double dy = velocity_y * scale;

      double src_x = static_cast<double>(x) - dx;
      double src_y = static_cast<double>(y) - dy;

      _velocity_y_tmp->at(x, y) = interpolate(*_velocity_y_cur, src_x, src_y);
    }
  }

  std::swap(_velocity_y_tmp, _velocity_y_cur);
}

//////////////////////////////////////////////////////////////
void ggo::smoke_animation_artist::make_incompressible()
{
  // Compute pressure.
  ggo::array<double, 2> divergence(get_width(), get_height());
  ggo::array<double, 2> pressure(get_width(), get_height());
  for (int y = 0; y < get_height(); ++y)
  {
    for (int x = 0; x < get_width(); ++x)
    {
      divergence(x, y) = -(_velocity_x_cur->at_loop(x + 1, y) - _velocity_x_cur->at_loop(x, y) + _velocity_y_cur->at_loop(x, y + 1) - _velocity_y_cur->at_loop(x, y));
      divergence(x, y) /= _cell_size;
      pressure(x, y) = 0.;
    }
  }

  // Solve pressure with Gauss-Seidel.
  for (int k = 0; k < 50; ++k)
  {
    for (int y = 0; y < get_height(); ++y)
    {
      for (int x = 0; x < get_width(); ++x)
      {
        pressure(x, y) = (divergence(x, y) + pressure.at_loop(x - 1, y) + pressure.at_loop(x + 1, y) + pressure.at_loop(x, y - 1) + pressure.at_loop(x, y + 1)) / 4;
      }
    }
  }

  // Apply pressure.
  double scale = time_step / (_density * _cell_size);
  for (int y = 0; y < get_height(); ++y)
  {
    for (int x = 0; x < get_width() + 1; ++x)
    {
      _velocity_x_cur->at(x, y) -= scale * (pressure.at_loop(x, y) - pressure.at_loop(x - 1, y));
    }
  }

  for (int y = 0; y < get_height() + 1; ++y)
  {
    for (int x = 0; x < get_width(); ++x)
    {
      _velocity_y_cur->at(x, y) -= scale * (pressure.at_loop(x, y) - pressure.at_loop(x, y - 1));
    }
  }
}

//////////////////////////////////////////////////////////////
void ggo::smoke_animation_artist::opacity_advection()
{
  double scale = time_step / _cell_size;

  for (int y = 0; y < get_height(); ++y)
  {
    for (int x = 0; x < get_width(); ++x)
    {
      // Backtracking.
      double velocity_x = 0.5 * (_velocity_x_cur->at_loop(x, y) + _velocity_x_cur->at_loop(x + 1, y));
      double velocity_y = 0.5 * (_velocity_y_cur->at_loop(x, y) + _velocity_y_cur->at_loop(x, y + 1));

      double dx = velocity_x * scale;
      double dy = velocity_y * scale;

      double src_x = static_cast<double>(x) - dx;
      double src_y = static_cast<double>(y) - dy;

      _opacity_tmp->at(x, y) = interpolate(*_opacity_cur, src_x, src_y);
    }
  }

  std::swap(_opacity_cur, _opacity_tmp);
}

//////////////////////////////////////////////////////////////
bool ggo::smoke_animation_artist::prepare_frame()
{
  const int frames_count = 600;

  if (_frame_index > frames_count)
  {
    return false;
  }

  process_sources();
  velocity_self_advection();
  make_incompressible();
  opacity_advection();

  ++_frame_index;

  return true;
}

//////////////////////////////////////////////////////////////
void ggo::smoke_animation_artist::process_frame(void * buffer, const ggo::rect_int & clipping)
{
	// Draw the opacity.
  for (int y = 0; y < get_height(); ++y)
  {
    for (int x = 0; x < get_width(); ++x)
    {
      double opacity = _opacity_cur->at(x, y);

      ggo::color_8u c_8u = ggo::read_pixel<ggo::rgb_8u_yu>(_bkgd_buffer.data(), x, y, get_height(), 3 * get_width());
      c_8u.r() = ggo::round_to<uint8_t>(opacity * _smoke_color.r() + (1. - opacity) * c_8u.r());
      c_8u.g() = ggo::round_to<uint8_t>(opacity * _smoke_color.g() + (1. - opacity) * c_8u.g());
      c_8u.b() = ggo::round_to<uint8_t>(opacity * _smoke_color.b() + (1. - opacity) * c_8u.b());
      ggo::write_pixel<ggo::rgb_8u_yu>(buffer, x, y, get_height(), get_line_step(), c_8u);
    }
  }
}
