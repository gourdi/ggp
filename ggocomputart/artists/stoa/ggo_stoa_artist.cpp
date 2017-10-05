#include "ggo_stoa_artist.h"
#include <ggo_interpolation3d.h>
#include <ggo_background3d_color.h>
#include <ggo_raytracer_global.h>
#include <ggo_point_camera.h>
#include <ggo_solid_color_material.h>
#include <ggo_marching_cubes.h>

namespace
{
  //////////////////////////////////////////////////////////////
  template <typename cmp_x_func, typename cmp_y_func, typename cmp_z_func>
  bool compare_point(const ggo::pos3f & p1, const ggo::pos3f & p2, cmp_x_func cmp_x, cmp_y_func cmp_y, cmp_z_func cmp_z)
  {
    return cmp_x(p1.x(), p2.x()) && cmp_y(p1.y(), p2.y()) && cmp_z(p1.z(), p2.z());
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
  struct noise3d
  {
    noise3d(float amplitude, float scale) : _amplitude(amplitude), _scale(scale)
    {
      _offset_x = ggo::rand<float>(0, 16);
      _offset_y = ggo::rand<float>(0, 16);
      _offset_z = ggo::rand<float>(0, 16);

      float angle1 = ggo::rand<float>(0.f, 2.f * ggo::pi<float>());
      float angle2 = ggo::rand<float>(0.f, 2.f * ggo::pi<float>());
      ggo::vec3f axis(std::cos(angle1) * std::sin(angle2), std::sin(angle1) * std::sin(angle2), std::cos(angle2));
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
    float dx = density(p.x() + epsilon, p.y(), p.z()) - density(p.x() - epsilon, p.y(), p.z());
    float dy = density(p.x(), p.y() + epsilon, p.z()) - density(p.x(), p.y() - epsilon, p.z());
    float dz = density(p.x(), p.y(), p.z() + epsilon) - density(p.x(), p.y(), p.z() - epsilon);

    return ggo::vertex<float>(p, { dx, dy, dz });
  }
}

//////////////////////////////////////////////////////////////
// RAYCASTER
ggo::stoa_artist::raycaster::raycaster(const std::vector<ggo::stoa_artist::face_object> & face_objects)
:
_octree({ get_bounding_box(face_objects), face_objects })
{
  auto recursion = [](ggo::tree<raycaster::node> & tree)
  {
    ggo::pos3f center = tree.data()._bounding_box.get_center();

    std::array<raycaster::node, 8> nodes;

    std::less_equal<float> le;
    std::greater_equal<float> ge;

    for (const auto & face_object : tree.data()._face_objects)
    {
      if (compare_face(face_object._face, center, le, le, le)) { nodes[0]._face_objects.push_back(face_object); }
      if (compare_face(face_object._face, center, le, le, ge)) { nodes[1]._face_objects.push_back(face_object); }
      if (compare_face(face_object._face, center, le, ge, le)) { nodes[2]._face_objects.push_back(face_object); }
      if (compare_face(face_object._face, center, le, ge, ge)) { nodes[3]._face_objects.push_back(face_object); }
      if (compare_face(face_object._face, center, ge, le, le)) { nodes[4]._face_objects.push_back(face_object); }
      if (compare_face(face_object._face, center, ge, le, ge)) { nodes[5]._face_objects.push_back(face_object); }
      if (compare_face(face_object._face, center, ge, ge, le)) { nodes[6]._face_objects.push_back(face_object); }
      if (compare_face(face_object._face, center, ge, ge, ge)) { nodes[7]._face_objects.push_back(face_object); }
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
const ggo::object3d_abc * ggo::stoa_artist::raycaster::hit_test(const ggo::ray3d_float & ray,
                                                                float & dist,
                                                                ggo::ray3d_float & local_normal,
                                                                ggo::ray3d_float & world_normal,
                                                                const ggo::object3d_abc * exclude_object1,
                                                                const ggo::object3d_abc * exclude_object2) const
{
  const ggo::object3d_abc * hit_object = nullptr;
  std::vector<const ggo::tree<raycaster::node> *> trees{ &_octree };

  dist = std::numeric_limits<float>::max();

  while (trees.empty() == false)
  {
    std::vector<const ggo::tree<raycaster::node> *> sub_trees;

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
          if (face_object._face.intersect_ray(ray, dist_cur, normal_cur) == true &&
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
bool ggo::stoa_artist::raycaster::check_visibility(const ggo::ray3d_float & ray,
                                                   float dist_max,
                                                   const ggo::object3d_abc * exclude_object1,
                                                   const ggo::object3d_abc * exclude_object2) const
{
  std::vector<const ggo::tree<raycaster::node> *> trees{ &_octree };

  while (trees.empty() == false)
  {
    std::vector<const ggo::tree<raycaster::node> *> sub_trees;

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
          if (face_object._face.intersect_ray(ray, dist, normal_cur) == true &&
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
ggo::aabox3d_float ggo::stoa_artist::raycaster::get_bounding_box(const std::vector<ggo::stoa_artist::face_object> & face_objects)
{
  ggo::aabox3d_float bounding_box(0.f, 0.f, 0.f, 0.f, 0.f, 0.f);

  for (const auto & face_object : face_objects)
  {
    bounding_box.merge_with(face_object._face.v1()._pos);
    bounding_box.merge_with(face_object._face.v2()._pos);
    bounding_box.merge_with(face_object._face.v3()._pos);
  }

  return bounding_box;
}

//////////////////////////////////////////////////////////////
// ARTIST

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
  auto cells = ggo::marching_cubes(density_func, ggo::pos3f(-range, -range, -range), steps, 2 * range / steps);

  std::vector<ggo::stoa_artist::face_object> face_objects;

  for (const auto & cell : cells)
  {
    for (const auto & triangle : cell._triangles)
    {
      auto v1 = compute_vertex(density_func, triangle.v1());
      auto v2 = compute_vertex(density_func, triangle.v2());
      auto v3 = compute_vertex(density_func, triangle.v3());

      ggo::face3d<float, true> face(v1, v2, v3);

      using object_t = ggo::diffuse_object3d<ggo::discard_all, ggo::face3d<float, true>, ggo::solid_color_material>;
      auto object = std::make_shared<object_t>(face, ggo::white_material());

      ggo::stoa_artist::face_object face_object{ object, face };

      face_objects.push_back(face_object);
    }
  }

  _raycaster.reset(new ggo::stoa_artist::raycaster(face_objects));
}

//////////////////////////////////////////////////////////////
void ggo::stoa_artist::render(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf, float hue,
                              const ggo::pos3f & light_pos1, const ggo::pos3f & light_pos2,
                              ggo::renderer_abc & renderer) const
{
  ggo::scene scene(std::make_shared<ggo::background3d_color>(ggo::from_hsv<ggo::color_32f>(hue, 1.f, 1.f)));

  // Lights.
  scene.add_sphere_light(ggo::from_hsv<ggo::color_32f>(hue, 0.5f, 0.8f), light_pos1, 1);
  scene.add_sphere_light(ggo::from_hsv<ggo::color_32f>(hue, 0.5f, 0.2f), light_pos2, 1);

  // Rendering.
  ggo::raytrace_params params;
  params._raycaster = _raycaster.get();
  renderer.render(buffer, width, height, line_step, pbf, scene, params);
}