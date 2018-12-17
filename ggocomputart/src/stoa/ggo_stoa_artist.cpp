#include "ggo_stoa_artist.h"
#include <kernel/math/interpolation/ggo_interpolation3d.h>
#include <kernel/math/marching_algorithms/ggo_marching_cubes.h>
#include <raytracer/backgrounds/ggo_background3d_color.h>
#include <raytracer/ggo_raytracer_global.h>
#include <raytracer/cameras/ggo_point_camera.h>
#include <raytracer/materials/ggo_solid_color_material.h>
#include <raytracer/raycasters/ggo_octree_raycaster.h>

namespace
{
  //////////////////////////////////////////////////////////////
  template <typename cmp_x_func, typename cmp_y_func, typename cmp_z_func>
  bool compare_point(const ggo::pos3_f & p1, const ggo::pos3_f & p2, cmp_x_func cmp_x, cmp_y_func cmp_y, cmp_z_func cmp_z)
  {
    return cmp_x(p1.x(), p2.x()) && cmp_y(p1.y(), p2.y()) && cmp_z(p1.z(), p2.z());
  }

  //////////////////////////////////////////////////////////////
  template <typename cmp_x_func, typename cmp_y_func, typename cmp_z_func>
  bool compare_face(const ggo::face3d<float, true> & f, const ggo::pos3_f & p, cmp_x_func cmp_x, cmp_y_func cmp_y, cmp_z_func cmp_z)
  {
    return compare_point(f.v1()._pos, p, cmp_x, cmp_y, cmp_z) ||
           compare_point(f.v2()._pos, p, cmp_x, cmp_y, cmp_z) ||
           compare_point(f.v3()._pos, p, cmp_x, cmp_y, cmp_z);
  }

  //////////////////////////////////////////////////////////////
  struct noise3d
  {
    noise3d(float amplitude, float scale) : _amplitude(amplitude), _scale(scale)
    {
      _offset_x = ggo::rand<float>(0, 16);
      _offset_y = ggo::rand<float>(0, 16);
      _offset_z = ggo::rand<float>(0, 16);

      float angle1 = ggo::rand<float>(0.f, 2.f * ggo::pi<float>());
      float angle2 = ggo::rand<float>(0.f, 2.f * ggo::pi<float>());
      ggo::vec3_f axis(std::cos(angle1) * std::sin(angle2), std::sin(angle1) * std::sin(angle2), std::cos(angle2));
      ggo::fill_rotation_matrix(axis, ggo::rand<float>(0, 2 * ggo::pi<float>()), _rotation);

      for (auto & value : _noise3d)
      {
        value = ggo::rand<float>(-1.f, 1.f);
      }
    }

    float evaluate(float x, float y, float z) const
    {
      ggo::apply_matrix3d(_rotation, x, y, z);

      x += _offset_x;
      y += _offset_y;
      z += _offset_z;

      return _amplitude * ggo::linear_interpolation3d(_noise3d.data(), 16, 16, 16, _scale * x, _scale * y, _scale * z);
    }

    std::array<float, 16 * 16 * 16> _noise3d;
    float _offset_x;
    float _offset_y;
    float _offset_z;
    float _amplitude;
    float _scale;
    float _rotation[3][3];
  };

  //////////////////////////////////////////////////////////////
  template <typename DensityFunc>
  ggo::vertex<float> compute_vertex(DensityFunc density, const ggo::pos3_f & p)
  {
    const float epsilon = 0.01f;
    float dx = density(p.x() + epsilon, p.y(), p.z()) - density(p.x() - epsilon, p.y(), p.z());
    float dy = density(p.x(), p.y() + epsilon, p.z()) - density(p.x(), p.y() - epsilon, p.z());
    float dz = density(p.x(), p.y(), p.z() + epsilon) - density(p.x(), p.y(), p.z() - epsilon);

    return ggo::vertex<float>(p, { dx, dy, dz });
  }
}

//////////////////////////////////////////////////////////////
ggo::stoa_artist::stoa_artist(int steps)
{
  // Objects.
  std::vector<noise3d> noises;

  float amplitude = 1.f;
  float scale = 0.5f;

  for (int i = 1; i <= 22; ++i)
  {
    noises.emplace_back(amplitude, scale);

    printf("amplitude: %f - scale: %f\n", amplitude, scale);

    amplitude /= 1.25f;
    scale *= 1.25f;
  }

  auto density_func = [&](float x, float y, float z)
  {
    float v = std::sqrt(x * x + y * y + z * z);

    for (const auto & noise : noises)
    {
      v += noise.evaluate(x, y, z);
    }

    v -= 1.75f;

    return v;
  };

  const float range = 3.2f;
  auto cells = ggo::marching_cubes(density_func, ggo::pos3_f(-range, -range, -range), steps, 2 * range / steps);

  for (const auto & cell : cells)
  {
    for (const auto & triangle : cell._triangles)
    {
      auto v1 = compute_vertex(density_func, triangle.v1());
      auto v2 = compute_vertex(density_func, triangle.v2());
      auto v3 = compute_vertex(density_func, triangle.v3());

      _faces.emplace_back(v1, v2, v3);
    }
  }
}

//////////////////////////////////////////////////////////////
void ggo::stoa_artist::render(void * buffer, int width, int height, int line_step, ggo::image_format format, float hue,
                              const ggo::pos3_f & light_pos1, const ggo::pos3_f & light_pos2,
                              ggo::renderer_abc & renderer) const
{
  ggo::scene scene(std::make_shared<ggo::background3d_color>(ggo::from_hsv<ggo::rgb_32f>(hue, 1.f, 1.f)));

  // Faces.
  for (const auto & face : _faces)
  {
    scene.add_diffuse_object<ggo::discard_all, ggo::face3d<float, true>, ggo::solid_color_material>(face, ggo::white_material());
  }

  // Lights.
  scene.add_sphere_light(ggo::from_hsv<ggo::rgb_32f>(hue, 0.5f, 0.8f), light_pos1, 1);
  scene.add_sphere_light(ggo::from_hsv<ggo::rgb_32f>(hue, 0.5f, 0.2f), light_pos2, 1);

  // Rendering.
  ggo::octree_raycaster raycaster(scene.solid_objects());

  ggo::raytrace_params params;
  params._raycaster = &raycaster;
  renderer.render(buffer, width, height, line_step, format, scene, params);
}