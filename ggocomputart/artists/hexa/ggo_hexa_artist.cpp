#include "ggo_hexa_artist.h"
#include <ggo_distance.h>
#include <ggo_renderer_abc.h>
#include <ggo_raytracer_global.h>
#include <ggo_solid_color_material.h>
#include <ggo_background3d_color.h>
#include <ggo_octree_raycaster.h>

namespace
{
  constexpr float camera_dist = 250.f;
  constexpr float radius = 20.f;

  //////////////////////////////////////////////////////////////
  template <uint32_t border_flags>
  void create_hexa(
    float x, float y,
    const ggo::color_32f & top_color, const ggo::color_32f & side_color,
    const std::vector<ggo::hexa_artist::sinusoid> & sinusoids, 
    float progress,
    ggo::scene & scene)
  {
    float dist = ggo::distance(x, y);
    if (dist < radius)
    {
      float height = std::sqrt(radius * radius - dist * dist);

      float scale = 0.f;
      for (const auto & sinusoid : sinusoids)
      {
        ggo::vec2f dir(std::cos(sinusoid._angle), std::sin(sinusoid._angle));
        float proj = ggo::dot(dir, { x, y });
        float phase = sinusoid._phase_inf + ggo::ease_inout(progress) * (sinusoid._phase_sup - sinusoid._phase_inf);
        scale += sinusoid._amplitude * (1.f + std::cos(phase + 2.f * ggo::pi<float>() * proj / sinusoid._wave_length));
      }

      height *= scale;

      for (int i = 0; i < 6; ++i)
      {
        float angle1 = i * ggo::pi<float>() / 3;
        float angle2 = (i + 1) * ggo::pi<float>() / 3;

        scene.add_simple_color_object<ggo::discard_all, ggo::triangle3d<float, true>, ggo::solid_color_material>({
          { x + std::cos(angle1), y + std::sin(angle1), height },
          { x + std::cos(angle2), y + std::sin(angle2), height },
          { x, y, height } }, top_color);

        auto & side1 = scene.add_diffuse_object<border_flags, ggo::triangle3d<float, true>, ggo::solid_color_material>({
          { x + std::cos(angle1), y + std::sin(angle1), height },
          { x + std::cos(angle2), y + std::sin(angle2), height },
          { x + std::cos(angle2), y + std::sin(angle2), -height } }, side_color);
        side1.set_reflection_factor(0.25f);

        auto & side2 = scene.add_diffuse_object<border_flags, ggo::triangle3d<float, true>, ggo::solid_color_material>({
          { x + std::cos(angle1), y + std::sin(angle1), -height },
          { x + std::cos(angle1), y + std::sin(angle1), height },
          { x + std::cos(angle2), y + std::sin(angle2), -height } }, side_color);
        side2.set_reflection_factor(0.25f);

        if (!(border_flags & ggo::discard_roughness))
        {
          side1.set_roughness(0.1f);
          side2.set_roughness(0.1f);
        }
      }
    }
  }

  //////////////////////////////////////////////////////////////
  void create_hexas(const ggo::color_32f & top_color, const ggo::color_32f & side_color, const std::vector<ggo::hexa_artist::sinusoid> & sinusoids, float progress, bool enable_roughness, ggo::scene & scene)
  {
    const int range = ggo::round_to<int>(radius);
    for (int x = -range; x < range; ++x)
    {
      for (int y = -range; y < range; ++y)
      {
        float x1 = 3.f * x;
        float y1 = y * std::sqrt(3.f);

        float x2 = 3 * x + 1.5f;
        float y2 = (2 * y + 1) * std::sqrt(3.f) / 2;

        if (enable_roughness == true)
        {
          constexpr uint32_t flags = ggo::discard_basis | ggo::discard_phong;
          create_hexa<flags>(x1, y1, top_color, side_color, sinusoids, progress, scene);
          create_hexa<flags>(x2, y2, top_color, side_color, sinusoids, progress, scene);
        }
        else
        {
          constexpr uint32_t flags = ggo::discard_basis | ggo::discard_phong | ggo::discard_roughness;
          create_hexa<flags>(x1, y1, top_color, side_color, sinusoids, progress, scene);
          create_hexa<flags>(x2, y2, top_color, side_color, sinusoids, progress, scene);
        }
      }
    }
  }
}

//////////////////////////////////////////////////////////////
ggo::basis3d_float ggo::hexa_artist::generate_camera_basis(float progress) const
{
  ggo::basis3d_float camera_basis({ 0.f, 0.f, camera_dist });

  float angle = ggo::linear_interpolation(0.f, _camera_angle_start, 1.f, _camera_angle_end, progress);

  camera_basis.rotate(ggo::ray3d_float::O_X(), 1.1f);
  camera_basis.rotate(ggo::ray3d_float::O_Z(), angle);

  return camera_basis;
}

//////////////////////////////////////////////////////////////
ggo::hexa_artist::hexa_artist(bool enable_roughness)
:
_enable_roughness(enable_roughness)
{
  float amplitude = 0.1f;
  float wave_length = 100.f;
  for (int i = 0; i < 10; ++i)
  {
    float phase_inf = ggo::rand<float>(0, 2.f * ggo::pi<float>());
    float phase_sup = phase_inf + ggo::rand<float>(32.f, 48.f);
    float angle = ggo::rand<float>(0, 2.f * ggo::pi<float>());

    _sinusoids.push_back({ wave_length, amplitude, angle, phase_inf, phase_sup });

    amplitude *= 0.95f;
    wave_length *= 0.75f;

    printf("%f %f\n", amplitude, wave_length);
  }

  _back_color = ggo::from_hsv<ggo::color_32f>(ggo::rand<float>(), 1.f, 1.f);

  float grayscale = ggo::rand<float>(0.6f, 0.9f);
  _side_color = ggo::color_32f(grayscale, grayscale, grayscale);
  grayscale += 0.5f * (1.f - grayscale);
  _top_color = ggo::color_32f(grayscale, grayscale, grayscale);

  _camera_angle_start = ggo::rand<float>(-0.2f, 0.2f);
  _camera_angle_end = ggo::rand<float>(-0.2f, 0.2f);
}

//////////////////////////////////////////////////////////////
void ggo::hexa_artist::render(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf,
                              float progress, ggo::renderer_abc & renderer)
{

  ggo::scene scene(std::make_shared<ggo::background3d_color>(_back_color));

  // Objects.
  create_hexas(_top_color, _side_color, _sinusoids, progress, _enable_roughness, scene);

  // Add lights to scene.
  scene.add_sphere_light(ggo::white<ggo::color_32f>(), { 0.f, -1000.f, 1000.f }, 100.f);

  // Raytracer parameters.
  ggo::octree_raycaster raycaster(scene, 4);
  ggo::raytrace_params raytrace_params;
  raytrace_params._raycaster = &raycaster;

  // Rendering.
  renderer.render(buffer, width, height, line_step, pbf, scene, raytrace_params);
}
