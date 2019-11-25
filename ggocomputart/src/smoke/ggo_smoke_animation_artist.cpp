//#include "ggo_smoke_animation_artist.h"
//#include <kernel/ggo_kernel.h>
//#include <2d/ggo_image.h>
//#include <2d/processing/ggo_gaussian_blur.h>
//#include <2d/fill/ggo_fill.h>
//
//namespace
//{
//  constexpr float time_step = 1.f / 25.f;
//
//  //////////////////////////////////////////////////////////////
//  // Here this(0, 0) is really located at (0, 0), meaning that
//  // the array covers a (-0.5, size_x+0.5) x (-0.5, size_y+0.5)
//  // region.
//  float interpolate(const ggo::array<float, 2> & grid, float x, float y)
//  {
//    // Bilinear interpolation.
//    int src_left = x < 0 ? int(x) - 1 : int(x);
//    int src_bottom = y < 0 ? int(y) - 1 : int(y);
//
//    float alpha_x = x - src_left;
//    float alpha_y = y - src_bottom;
//    GGO_ASSERT(alpha_x >= -0.0001 && alpha_x <= 1.0001);
//    GGO_ASSERT(alpha_y >= -0.0001 && alpha_y <= 1.0001);
//
//    src_left = ggo::pos_mod(src_left, grid.width());
//    src_bottom = ggo::pos_mod(src_bottom, grid.height());
//
//    int src_right = (src_left + 1) % grid.width();
//    int src_top = (src_bottom + 1) % grid.height();
//    float bottom = (1.f - alpha_x) * grid(src_left, src_bottom) + alpha_x * grid(src_right, src_bottom);
//    float top = (1.f - alpha_x) * grid(src_left, src_top) + alpha_x * grid(src_right, src_top);
//    return (1.f - alpha_y) * bottom + alpha_y * top;
//  }
//}
//
////////////////////////////////////////////////////////////////
//ggo::smoke_animation_artist::smoke_animation_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order)
//:
//fixed_frames_count_animation_artist_abc(width, height, line_byte_step, pixel_type, memory_lines_order, 600),
//_velocity_x1(width + 1, height),
//_velocity_x2(width + 1, height),
//_velocity_y1(width, height + 1),
//_velocity_y2(width, height + 1),
//_opacity1(width, height),
//_opacity2(width, height),
//_bkgd_buffer(3 * width * height),
//_sources(4)
//{
//  _velocity_x_cur = &_velocity_x1;
//  _velocity_x_tmp = &_velocity_x2;
//  _velocity_y_cur = &_velocity_y1;
//  _velocity_y_tmp = &_velocity_y2;
//  _opacity_cur = &_opacity1;
//  _opacity_tmp = &_opacity2;
//
//  _velocity_x_cur->fill(0);
//  _velocity_y_cur->fill(0);
//  _velocity_x_tmp->fill(0);
//  _velocity_y_tmp->fill(0);
//  _opacity_cur->fill(0);
//  _opacity_tmp->fill(0);
//
//  const float area = 1.;
//  _cell_size = area / std::sqrt(float(width) * float(height));
//
//  _density = ggo::rand<float>(1.f, 5.f);// 2.;
//  std::cout << "density: " << _density << std::endl;
//
//  for (int i = 0; i < _sources.count(); ++i)
//  {
//    ggo::smoke_animation_artist::fluid_source & source = _sources(i);
//
//    switch (i)
//    {
//    case 0:
//      // Top.
//      source._disc.center() = { 0.5, 0.75 };
//      source._angle = -ggo::pi<float>() / 2;
//      break;
//    case 1:
//      // Right.
//      source._disc.center() = { 0.5, 0.25 };
//      source._angle = ggo::pi<float>();
//      break;
//    case 2:
//      // Bottom.
//      source._disc.center() = { 0.75, 0.5 };
//      source._angle = ggo::pi<float>() / 2;
//      break;
//    case 3:
//      // Left.
//      source._disc.center() = { 0.25, 0.5 };
//      source._angle = 0;
//      break;
//    }
//
//    source._disc.radius() = ggo::rand<float>(0.03f, 0.04f);
//    source._speed = ggo::rand<float>(0.04f, 0.09f); // 0.05;
//    std::cout << "source speed: " << source._speed << std::endl;
//    source._timer1 = ggo::rand<int>(50, 100);
//    source._timer2 = ggo::rand<int>(50, 150);
//    source._angle_amplitude = ggo::rand<float>(ggo::pi<float>() / 4, ggo::pi<float>());
//    source._angle_offset = ggo::rand<float>(0, 2 * ggo::pi<float>());
//    source._wave_length = ggo::rand<float>(0.05f, 0.2f);
//    source._opacity = 0.f;
//  }
//
//  ggo::smoke_animation_artist::fluid_source & source = _sources(ggo::rand<int>(0, 3));
//  source._timer1 = 0;
//
//  float hue = ggo::rand<float>();
//  _smoke_color = ggo::from_hsv<ggo::rgb_8u>(hue, 1, ggo::rand<float>(0.2f, 0.8f));
//
//  const ggo::rgb_8u color1 = ggo::from_hsv<ggo::rgb_8u>(hue, ggo::rand<float>(0.f, 0.2f), ggo::rand<float>(0.8f, 1.f));
//  const ggo::rgb_8u color2 = ggo::from_hsv<ggo::rgb_8u>(hue, ggo::rand<float>(0.f, 0.2f), ggo::rand<float>(0.8f, 1.f));
//  const ggo::rgb_8u color3 = ggo::from_hsv<ggo::rgb_8u>(hue, ggo::rand<float>(0.f, 0.2f), ggo::rand<float>(0.8f, 1.f));
//  const ggo::rgb_8u color4 = ggo::from_hsv<ggo::rgb_8u>(hue, ggo::rand<float>(0.f, 0.2f), ggo::rand<float>(0.8f, 1.f));
//
//  ggo::image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up> img(_bkgd_buffer.data(), { width, height }, line_byte_step);
//  ggo::fill_4_colors(img, color1, color2, color3, color4);
//}
//
////////////////////////////////////////////////////////////////
//void ggo::smoke_animation_artist::process_sources(int frame_index)
//{
//  for (auto & source : _sources)
//  {
//    if (source._timer1 > 0)
//    {
//      source._timer1 -= 1;
//    }
//    else if (source._timer2 > 0)
//    {
//      source._timer2 -= 1;
//      source._opacity = std::min(source._opacity + 0.1f, 1.0f);
//    }
//    else if (source._opacity > 0.)
//    {
//      source._opacity = std::max(source._opacity - 0.01f, 0.0f);
//    }
//  }
//
//  for (int y = 0; y < height(); ++y)
//  {
//    for (int x = 0; x < width(); ++x)
//    {
//      for (const auto & source : _sources)
//      {
//        if (source._opacity > 0)
//        {
//          ggo::disc_f disc = map_fit(ggo::disc_f(source._disc), 0., 1.);
//
//          float dist = ggo::distance({ static_cast<float>(x), static_cast<float>(y) }, disc.get_center());
//          if (dist < disc.radius())
//          {
//            _opacity_cur->at(x, y) = std::min(_opacity_cur->at(x, y) + source._opacity, 1.0f);
//          }
//
//          if (dist < 0.8 * disc.radius())
//          {
//            float angle = source._angle + source._angle_amplitude * std::cos(source._wave_length * frame_index + source._angle_offset);
//
//            _velocity_x_cur->at(x, y) = source._speed * std::cos(angle);
//            _velocity_y_cur->at(x, y) = source._speed * std::sin(angle);
//          }
//        }
//      }
//    }
//  }
//}
//
////////////////////////////////////////////////////////////////
//void ggo::smoke_animation_artist::velocity_self_advection()
//{
//  float scale = time_step / _cell_size;
//
//  // Horizontal self advection.
//  for (int y = 0; y < height(); ++y)
//  {
//    for (int x = 0; x < width() + 1; ++x)
//    {
//      float velocity_x = _velocity_x_cur->at_loop(x, y);
//      float velocity_y = 0.25f * (_velocity_y_cur->at_loop(x, y) + _velocity_y_cur->at_loop(x, y + 1) + _velocity_y_cur->at_loop(x - 1, y) + _velocity_y_cur->at_loop(x - 1, y + 1));
//
//      float dx = velocity_x * scale;
//      float dy = velocity_y * scale;
//
//      float src_x = static_cast<float>(x) - dx;
//      float src_y = static_cast<float>(y) - dy;
//
//      _velocity_x_tmp->at(x, y) = interpolate(*_velocity_x_cur, src_x, src_y);
//    }
//  }
//
//  std::swap(_velocity_x_tmp, _velocity_x_cur);
//
//  // Vertical self advection.
//  for (int y = 0; y < height() + 1; ++y)
//  {
//    for (int x = 0; x < width(); ++x)
//    {
//      float velocity_x = 0.25f * (_velocity_x_cur->at_loop(x, y) + _velocity_x_cur->at_loop(x, y - 1) + _velocity_x_cur->at_loop(x + 1, y) + _velocity_x_cur->at_loop(x + 1, y - 1));
//      float velocity_y = _velocity_y_cur->at_loop(x, y);
//
//      float dx = velocity_x * scale;
//      float dy = velocity_y * scale;
//
//      float src_x = static_cast<float>(x) - dx;
//      float src_y = static_cast<float>(y) - dy;
//
//      _velocity_y_tmp->at(x, y) = interpolate(*_velocity_y_cur, src_x, src_y);
//    }
//  }
//
//  std::swap(_velocity_y_tmp, _velocity_y_cur);
//}
//
////////////////////////////////////////////////////////////////
//void ggo::smoke_animation_artist::make_incompressible()
//{
//  // Compute pressure.
//  ggo::array<float, 2> divergence(width(), height());
//  ggo::array<float, 2> pressure(width(), height());
//  for (int y = 0; y < height(); ++y)
//  {
//    for (int x = 0; x < width(); ++x)
//    {
//      divergence(x, y) = -(_velocity_x_cur->at_loop(x + 1, y) - _velocity_x_cur->at_loop(x, y) + _velocity_y_cur->at_loop(x, y + 1) - _velocity_y_cur->at_loop(x, y));
//      divergence(x, y) /= _cell_size;
//      pressure(x, y) = 0.;
//    }
//  }
//
//  // Solve pressure with Gauss-Seidel.
//  {
//    float scale = _density * _cell_size * _cell_size / time_step;
//    for (int k = 0; k < 50; ++k)
//    {
//      for (int y = 0; y < height(); ++y)
//      {
//        for (int x = 0; x < width(); ++x)
//        {
//          float new_pressure = divergence(x, y) * scale;
//          new_pressure += pressure.at_loop(x - 1, y);
//          new_pressure += pressure.at_loop(x + 1, y);
//          new_pressure += pressure.at_loop(x, y - 1);
//          new_pressure += pressure.at_loop(x, y + 1);
//          new_pressure /= 4.;
//          pressure(x, y) = new_pressure;
//        }
//      }
//    }
//  }
//
//  // Apply pressure.
//  {
//    float scale = time_step / (_density * _cell_size);
//    for (int y = 0; y < height(); ++y)
//    {
//      for (int x = 0; x < width() + 1; ++x)
//      {
//        _velocity_x_cur->at(x, y) -= scale * (pressure.at_loop(x, y) - pressure.at_loop(x - 1, y));
//      }
//    }
//
//    for (int y = 0; y < height() + 1; ++y)
//    {
//      for (int x = 0; x < width(); ++x)
//      {
//        _velocity_y_cur->at(x, y) -= scale * (pressure.at_loop(x, y) - pressure.at_loop(x, y - 1));
//      }
//    }
//  }
//}
//
////////////////////////////////////////////////////////////////
//void ggo::smoke_animation_artist::opacity_advection()
//{
//  float scale = time_step / _cell_size;
//
//  for (int y = 0; y < height(); ++y)
//  {
//    for (int x = 0; x < width(); ++x)
//    {
//      // Backtracking.
//      float velocity_x = 0.5f * (_velocity_x_cur->at_loop(x, y) + _velocity_x_cur->at_loop(x + 1, y));
//      float velocity_y = 0.5f * (_velocity_y_cur->at_loop(x, y) + _velocity_y_cur->at_loop(x, y + 1));
//
//      float dx = velocity_x * scale;
//      float dy = velocity_y * scale;
//
//      float src_x = static_cast<float>(x) - dx;
//      float src_y = static_cast<float>(y) - dy;
//
//      _opacity_tmp->at(x, y) = interpolate(*_opacity_cur, src_x, src_y);
//    }
//  }
//
//  std::swap(_opacity_cur, _opacity_tmp);
//}
//
////////////////////////////////////////////////////////////////
//void ggo::smoke_animation_artist::diffuse()
//{
//  float stddev_opacity = 0.0001f * std::sqrt(float(width() * height()));
//  float stddev_velocity = 0.0001f * std::sqrt(float(width() * height()));
//
//  image_t<ggo::pixel_type::y_32f, ggo::lines_order::up> img_opacity(_opacity_cur->data(), { _opacity_cur->width(), _opacity_cur->height() });
//  ggo::gaussian_blur(img_opacity, stddev_opacity, ggo::border_mode::loop);
//
//  image_t<ggo::pixel_type::y_32f, ggo::lines_order::up> img_velocity_x_cur(_velocity_x_cur->data(), { _velocity_x_cur->width(), _velocity_x_cur->height() });
//  ggo::gaussian_blur(img_velocity_x_cur, stddev_velocity, ggo::border_mode::loop);
//
//  image_t<ggo::pixel_type::y_32f, ggo::lines_order::up> img_velocity_y_cur(_velocity_y_cur->data(), { _velocity_y_cur->width(), _velocity_y_cur->height() });
//  ggo::gaussian_blur(img_velocity_y_cur, stddev_velocity, ggo::border_mode::loop);
//}
//
////////////////////////////////////////////////////////////////
//void ggo::smoke_animation_artist::render_frame(void * buffer, int frame_index, float time_step)
//{
//  process_sources(frame_index);
//  velocity_self_advection();
//  make_incompressible();
//  opacity_advection();
//  diffuse();
//
//	// Draw the opacity.
//  ggo::image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up> bkgd(buffer, size(), line_byte_step());
//  ggo::image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up> img(buffer, size(), line_byte_step());
//
//  for (int y = 0; y < height(); ++y)
//  {
//    for (int x = 0; x < width(); ++x)
//    {
//      float opacity = _opacity_cur->at(x, y);
//
//      ggo::rgb_8u c_8u = bkgd.read_pixel(x, y);
//      c_8u.r() = ggo::round_to<uint8_t>(opacity * _smoke_color.r() + (1. - opacity) * c_8u.r());
//      c_8u.g() = ggo::round_to<uint8_t>(opacity * _smoke_color.g() + (1. - opacity) * c_8u.g());
//      c_8u.b() = ggo::round_to<uint8_t>(opacity * _smoke_color.b() + (1. - opacity) * c_8u.b());
//      img.write_pixel(x, y, c_8u);
//    }
//  }
//}
