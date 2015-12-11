#include "ggo_stoa_artist.h"
#include <ggo_interpolation3d.h>
#include <ggo_background3d_color.h>
#include <ggo_raytracer_global.h>
#include <ggo_point_camera.h>
#include <ggo_marching_cubes.h>
#include <ggo_object3d.h>

namespace
{
  struct ggo_noise3d
  {
    ggo_noise3d(float amplitude, float scale) : _amplitude(amplitude), _scale(scale)
    {
      _offset_x = ggo::rand_float(0, 16);
      _offset_y = ggo::rand_float(0, 16);
      _offset_z = ggo::rand_float(0, 16);

      float angle1 = ggo::rand_float(0.f, 2.f * ggo::PI<float>());
      float angle2 = ggo::rand_float(0.f, 2.f * ggo::PI<float>());
      ggo::vector3d_float axis(std::cos(angle1) * std::sin(angle2), std::sin(angle1) * std::sin(angle2), std::cos(angle2));
      ggo::fill_rotation_matrix(axis, ggo::rand_float(0, 2 * ggo::PI<float>()), _rotation);

      for (auto & value : _noise3d)
      {
        value = ggo::rand_float(-1.f, 1.f);
      }
    }

    float evaluate(float x, float y, float z) const
    {
      ggo::apply_matrix3d(_rotation, x, y, z);

      x += _offset_x;
      y += _offset_y;
      z += _offset_z;

      return _amplitude * ggo::bilinear_interpolation3d(_noise3d.data(), 16, 16, 16, _scale * x, _scale * y, _scale * z);
    }

    std::array<float, 16 * 16 * 16> _noise3d;
    float _offset_x;
    float _offset_y;
    float _offset_z;
    float _amplitude;
    float _scale;
    float _rotation[3][3];
  };

  template <typename DensityFunc>
  ggo::vertex<float> compute_vertex(DensityFunc density, const ggo::point3d_float & p)
  {
    const float epsilon = 0.01f;
    float dx = density(p.x() + epsilon, p.y(), p.z()) - density(p.x() - epsilon, p.y(), p.z());
    float dy = density(p.x(), p.y() + epsilon, p.z()) - density(p.x(), p.y() - epsilon, p.z());
    float dz = density(p.x(), p.y(), p.z() + epsilon) - density(p.x(), p.y(), p.z() - epsilon);

    return ggo::vertex<float>(p, { dx, dy, dz });
  }
}

//////////////////////////////////////////////////////////////
// RAYCASTER
ggo_stoa_artist::ggo_stoa_raycaster::ggo_stoa_raycaster(const std::vector<ggo_stoa_artist::ggo_raycaster_cell> & cells)
{
  std::cout << "cells: " << cells.size() << std::endl;

  // Sort cells.
  for (const auto & cell : cells)
  {
    std::ostringstream oss;

    int x = cell._x / 4;
    int y = cell._y / 4;
    int z = cell._z / 4;

    oss << x << "-" << y << "-" << z;

    _aggregates[oss.str()]._cells.push_back(cell);
  }

  std::cout << "aggregates: " << _aggregates.size() << std::endl;

  // Build each aggregates' bounding box.
  for (auto & map_item : _aggregates)
  {
    auto & aggregate = map_item.second;

    aggregate._bounding_box = aggregate._cells.front()._bounding_box;
    for (const auto & cell : aggregate._cells)
    {
      aggregate._bounding_box.merge_with(cell._bounding_box);
    }
  }
}

const ggo::object3d * ggo_stoa_artist::ggo_stoa_raycaster::hit_test(const ggo::ray3d_float & ray,
                                                                    float & dist,
                                                                    ggo::ray3d_float & local_normal,
                                                                    ggo::ray3d_float & world_normal,
                                                                    const ggo::object3d * exclude_object1,
                                                                    const ggo::object3d * exclude_object2) const
{
  const ggo::object3d * hit_object = nullptr;

  dist = std::numeric_limits<float>::max();

  for (const auto & map_item : _aggregates)
  {
    const auto & aggregate = map_item.second;
    if (aggregate._bounding_box.intersect_ray(ray) == true)
    {
      for (const auto & cell : aggregate._cells)
      {
        if (cell._bounding_box.intersect_ray(ray) == true)
        {
          for (const auto & face_object : cell._face_objects)
          {
            float dist_cur = 0.f;
            ggo::ray3d_float normal_cur;
            if (face_object._face->intersect_ray(ray, dist_cur, normal_cur) == true &&
                dist_cur < dist &&
                exclude_object1 != face_object._object &&
                exclude_object2 != face_object._object)
            {
              dist = dist_cur;
              local_normal = normal_cur;
              world_normal = normal_cur;
              hit_object = face_object._object;
            }
          }
        }
      }
    }
  }

  return hit_object;
}

bool ggo_stoa_artist::ggo_stoa_raycaster::hit_test(const ggo::ray3d_float & ray,
                                                   float dist_max,
                                                   const ggo::object3d * exclude_object1,
                                                   const ggo::object3d * exclude_object2) const
{
  for (const auto & map_item : _aggregates)
  {
    const auto & aggregate = map_item.second;
    if (aggregate._bounding_box.intersect_ray(ray) == true)
    {
      for (const auto & cell : aggregate._cells)
      {
        if (cell._bounding_box.intersect_ray(ray) == true)
        {
          for (const auto & face_object : cell._face_objects)
          {
            float dist = 0.f;
            ggo::ray3d_float normal_cur;
            if (face_object._face->intersect_ray(ray, dist, normal_cur) == true &&
              dist < dist_max &&
              exclude_object1 != face_object._object &&
              exclude_object2 != face_object._object)
            {
              return true;
            }
          }
        }
      }
    }
  }

  return false;
}

//////////////////////////////////////////////////////////////
ggo_stoa_artist::ggo_stoa_artist()
{
  // Objects.
  std::vector<ggo_noise3d> noises;

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
  const int steps = 384;
  auto cells = ggo::marching_cubes(density_func, ggo::point3d_float(-range, -range, -range), steps, 2 * range / steps);

  std::vector<ggo_raycaster_cell> raycaster_cells;

  for (const auto & cell : cells)
  {
    ggo_raycaster_cell raycaster_cell;
    raycaster_cell._bounding_box = cell._bounding_box;
    raycaster_cell._x = cell._x;
    raycaster_cell._y = cell._y;
    raycaster_cell._z = cell._z;

    for (const auto & triangle : cell._triangles)
    {
      auto v1 = compute_vertex(density_func, triangle.v1());
      auto v2 = compute_vertex(density_func, triangle.v2());
      auto v3 = compute_vertex(density_func, triangle.v3());

      auto face_ptr = std::make_shared<ggo::face3d<float, true>>(v1, v2, v3);
      auto object_ptr = std::make_shared<ggo::object3d>(face_ptr, ggo::color::WHITE);

      _objects.push_back(object_ptr);

      raycaster_cell._face_objects.emplace_back(object_ptr.get(), face_ptr.get());
    }

    raycaster_cells.push_back(raycaster_cell);
  }

  _raycaster.reset(new ggo_stoa_raycaster(raycaster_cells));

}

//////////////////////////////////////////////////////////////
void ggo_stoa_artist::render(uint8_t * buffer, int width, int height, float hue,
                             const ggo::point3d_float& light_pos1, const ggo::point3d_float& light_pos2,
                             ggo::renderer_abc& renderer) const
{
  ggo::scene_builder scene_builder(std::make_shared<ggo::background3d_color>(ggo::color::from_hsv(hue, 1.f, 1.f)));

  for (auto object : _objects)
  {
    scene_builder.add_object(object);
  }

  // Lights.
  scene_builder.add_sphere_light(ggo::color::from_hsv(hue, 0.5f, 0.8f), 1, light_pos1);
  scene_builder.add_sphere_light(ggo::color::from_hsv(hue, 0.5f, 0.2f), 1, light_pos2);

  // Rendering.
  renderer.render(buffer, width, height, scene_builder);
}