#include "ggo_stoa_artist.h"
#include <ggo_interpolation3d.h>
#include <ggo_background3d_color.h>
#include <ggo_raytracer_global.h>
#include <ggo_point_camera.h>
#include <ggo_marching_cubes.h>
#include <ggo_object3d.h>

namespace
{
  //////////////////////////////////////////////////////////////
  template <typename cmp_x_func, typename cmp_y_func, typename cmp_z_func>
  bool compare_point(const ggo::pos3f & p1, const ggo::pos3f & p2, cmp_x_func cmp_x, cmp_y_func cmp_y, cmp_z_func cmp_z)
  {
    return cmp_x(p1.get<0>(), p2.get<0>()) && cmp_y(p1.get<1>(), p2.get<1>()) && cmp_z(p1.get<2>(), p2.get<2>());
  }

  //////////////////////////////////////////////////////////////
  template <typename cmp_x_func, typename cmp_y_func, typename cmp_z_func>
  bool compare_face(const ggo::face3d<float, true> & f, const ggo::pos3f & p, cmp_x_func cmp_x, cmp_y_func cmp_y, cmp_z_func cmp_z)
  {
    return compare_point(f.v1()._pos, p, cmp_x, cmp_y, cmp_z) ||
           compare_point(f.v2()._pos, p, cmp_x, cmp_y, cmp_z) ||
           compare_point(f.v3()._pos, p, cmp_x, cmp_y, cmp_z);
  }

  //////////////////////////////////////////////////////////////
  struct ggo_noise3d
  {
    ggo_noise3d(float amplitude, float scale) : _amplitude(amplitude), _scale(scale)
    {
      _offset_x = ggo::rand_float(0, 16);
      _offset_y = ggo::rand_float(0, 16);
      _offset_z = ggo::rand_float(0, 16);

      float angle1 = ggo::rand_float(0.f, 2.f * ggo::PI<float>());
      float angle2 = ggo::rand_float(0.f, 2.f * ggo::PI<float>());
      ggo::vec3f axis(std::cos(angle1) * std::sin(angle2), std::sin(angle1) * std::sin(angle2), std::cos(angle2));
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

  //////////////////////////////////////////////////////////////
  template <typename DensityFunc>
  ggo::vertex<float> compute_vertex(DensityFunc density, const ggo::pos3f & p)
  {
    const float epsilon = 0.01f;
    float dx = density(p.get<0>() + epsilon, p.get<1>(), p.get<2>()) - density(p.get<0>() - epsilon, p.get<1>(), p.get<2>());
    float dy = density(p.get<0>(), p.get<1>() + epsilon, p.get<2>()) - density(p.get<0>(), p.get<1>() - epsilon, p.get<2>());
    float dz = density(p.get<0>(), p.get<1>(), p.get<2>() + epsilon) - density(p.get<0>(), p.get<1>(), p.get<2>() - epsilon);

    return ggo::vertex<float>(p, { dx, dy, dz });
  }
}

//////////////////////////////////////////////////////////////
// RAYCASTER
ggo_stoa_artist::ggo_stoa_raycaster::ggo_stoa_raycaster(const std::vector<ggo_stoa_artist::ggo_face_object> & face_objects)
:
_octree({ get_bounding_box(face_objects), face_objects })
{
  auto recursion = [](ggo::tree<ggo_node> & tree)
  {
    ggo::pos3f center = tree.data()._bounding_box.get_center();

    std::array<ggo_node, 8> nodes;

    std::less_equal<float> le;
    std::greater_equal<float> ge;

    for (const auto & face_object : tree.data()._face_objects)
    {
      if (compare_face(*face_object._face, center, le, le, le)) { nodes[0]._face_objects.push_back(face_object); }
      if (compare_face(*face_object._face, center, le, le, ge)) { nodes[1]._face_objects.push_back(face_object); }
      if (compare_face(*face_object._face, center, le, ge, le)) { nodes[2]._face_objects.push_back(face_object); }
      if (compare_face(*face_object._face, center, le, ge, ge)) { nodes[3]._face_objects.push_back(face_object); }
      if (compare_face(*face_object._face, center, ge, le, le)) { nodes[4]._face_objects.push_back(face_object); }
      if (compare_face(*face_object._face, center, ge, le, ge)) { nodes[5]._face_objects.push_back(face_object); }
      if (compare_face(*face_object._face, center, ge, ge, le)) { nodes[6]._face_objects.push_back(face_object); }
      if (compare_face(*face_object._face, center, ge, ge, ge)) { nodes[7]._face_objects.push_back(face_object); }
    }

    for (auto & node : nodes)
    {
      if (node._face_objects.empty() == false)
      {
        node._bounding_box = get_bounding_box(node._face_objects);
        tree.create_leaf(node);
      }
    }

    tree.data()._face_objects.clear();
  };

  _octree.visit_leaves(recursion);
  _octree.visit_leaves(recursion);
  _octree.visit_leaves(recursion);
}

//////////////////////////////////////////////////////////////
const ggo::object3d * ggo_stoa_artist::ggo_stoa_raycaster::hit_test(const ggo::ray3d_float & ray,
                                                                    float & dist,
                                                                    ggo::ray3d_float & local_normal,
                                                                    ggo::ray3d_float & world_normal,
                                                                    const ggo::object3d * exclude_object1,
                                                                    const ggo::object3d * exclude_object2) const
{
  const ggo::object3d * hit_object = nullptr;
  std::vector<const ggo::tree<ggo_node> *> trees{ &_octree };

  dist = std::numeric_limits<float>::max();

  while (trees.empty() == false)
  {
    std::vector<const ggo::tree<ggo_node> *> sub_trees;

    for (const auto * tree : trees)
    {
      // Fast reject.
      if (tree->data()._bounding_box.is_point_inside(ray.pos()) == true || tree->data()._bounding_box.intersect_ray(ray) == true)
      {
        // Check for intersection.
        for (const auto & face_object : tree->data()._face_objects)
        {
          float dist_cur = 0.f;
          ggo::ray3d_float normal_cur;
          if (face_object._face->intersect_ray(ray, dist_cur, normal_cur) == true &&
              dist_cur < dist &&
              exclude_object1 != face_object._object.get() &&
              exclude_object2 != face_object._object.get())
          {
            dist = dist_cur;
            local_normal = normal_cur;
            world_normal = normal_cur;
            hit_object = face_object._object.get();
          }
        }

        // Recursion.
        for (const auto & subtree : tree->subtrees())
        {
          sub_trees.push_back(&subtree);
        }
      }
    }

    trees = sub_trees;
  }

  return hit_object;
}

//////////////////////////////////////////////////////////////
bool ggo_stoa_artist::ggo_stoa_raycaster::check_visibility(const ggo::ray3d_float & ray,
                                                           float dist_max,
                                                           const ggo::object3d * exclude_object1,
                                                           const ggo::object3d * exclude_object2) const
{
  std::vector<const ggo::tree<ggo_node> *> trees{ &_octree };

  while (trees.empty() == false)
  {
    std::vector<const ggo::tree<ggo_node> *> sub_trees;

    for (const auto * tree : trees)
    {
      // Fast reject.
      if (tree->data()._bounding_box.is_point_inside(ray.pos()) == true || tree->data()._bounding_box.intersect_ray(ray) == true)
      {
        // Check for intersection.
        for (const auto & face_object : tree->data()._face_objects)
        {
          float dist = 0.f;
          ggo::ray3d_float normal_cur;
          if (face_object._face->intersect_ray(ray, dist, normal_cur) == true &&
              dist < dist_max &&
              exclude_object1 != face_object._object.get() &&
              exclude_object2 != face_object._object.get())
          {
            return true;
          }
        }

        // Recursion.
        for (const auto & subtree : tree->subtrees())
        {
          sub_trees.push_back(&subtree);
        }
      }
    }

    trees = sub_trees;
  }

  return false;
}

//////////////////////////////////////////////////////////////
ggo::aabox3d_float ggo_stoa_artist::ggo_stoa_raycaster::get_bounding_box(const std::vector<ggo_stoa_artist::ggo_face_object> & face_objects)
{
  ggo::aabox3d_float bounding_box(0.f, 0.f, 0.f, 0.f, 0.f, 0.f);

  for (const auto & face_object : face_objects)
  {
    bounding_box.merge_with(face_object._face->v1()._pos);
    bounding_box.merge_with(face_object._face->v2()._pos);
    bounding_box.merge_with(face_object._face->v3()._pos);
  }

  return bounding_box;
}

//////////////////////////////////////////////////////////////
// ARTIST

//////////////////////////////////////////////////////////////
ggo_stoa_artist::ggo_stoa_artist(int steps)
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
  auto cells = ggo::marching_cubes(density_func, ggo::pos3f(-range, -range, -range), steps, 2 * range / steps);

  std::vector<ggo_face_object> face_objects;

  for (const auto & cell : cells)
  {
    for (const auto & triangle : cell._triangles)
    {
      auto v1 = compute_vertex(density_func, triangle.v1());
      auto v2 = compute_vertex(density_func, triangle.v2());
      auto v3 = compute_vertex(density_func, triangle.v3());

      auto face_ptr = std::make_shared<const ggo::face3d<float, true>>(v1, v2, v3);
      auto object_ptr = std::make_shared<const ggo::object3d>(face_ptr, ggo::color::WHITE);

      ggo_face_object face_object{ object_ptr, face_ptr };

      face_objects.push_back(face_object);
    }
  }

  _raycaster.reset(new ggo_stoa_raycaster(face_objects));
}

//////////////////////////////////////////////////////////////
void ggo_stoa_artist::render(uint8_t * buffer, int width, int height, float hue,
                             const ggo::pos3f& light_pos1, const ggo::pos3f& light_pos2,
                             ggo::renderer_abc& renderer) const
{
  ggo::scene_builder scene_builder(std::make_shared<ggo::background3d_color>(ggo::color::from_hsv(hue, 1.f, 1.f)));

  // Lights.
  scene_builder.add_sphere_light(ggo::color::from_hsv(hue, 0.5f, 0.8f), 1, light_pos1);
  scene_builder.add_sphere_light(ggo::color::from_hsv(hue, 0.5f, 0.2f), 1, light_pos2);

  // Rendering.
  ggo::raytrace_params params;
  params._raycaster = _raycaster.get();
  renderer.render(buffer, width, height, scene_builder, params);
}