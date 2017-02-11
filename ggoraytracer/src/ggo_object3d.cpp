#include "ggo_object3d.h"
#include <ggo_solid_color_material.h>
#include <ggo_raytracer.h>
#include <ggo_best_candidate_sequence.h>
#include <ggo_shape_sampling.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  object3d::object3d()
  {
  }

  //////////////////////////////////////////////////////////////
  object3d::object3d(std::shared_ptr<const ggo::raytracable_shape3d_abc_float> shape)
    :
    _shape(shape)
  {
  }

  //////////////////////////////////////////////////////////////
  object3d::object3d(std::shared_ptr<const ggo::raytracable_shape3d_abc_float> shape, const ggo::color_32f & color)
    :
    _shape(shape),
    _material(std::make_shared<ggo::solid_color_material>(color))
  {
  }

  //////////////////////////////////////////////////////////////
  object3d::object3d(std::shared_ptr<const ggo::raytracable_shape3d_abc_float> shape, std::shared_ptr<const ggo::material_abc> material)
    :
    _shape(shape),
    _material(material)
  {
  }

  //////////////////////////////////////////////////////////////
  void object3d::set_shape(std::shared_ptr<const ggo::raytracable_shape3d_abc_float> shape)
  {
    _shape = shape;
  }

  //////////////////////////////////////////////////////////////
  ggo::color_32f object3d::get_color(const ggo::pos3f & world_pos) const
  {
    if (!_material)
    {
      return ggo::black<ggo::color_32f>();
    }

    if (_discard_basis == true)
    {
      return _material->get_color(world_pos);
    }
    else
    {
      return _material->get_color(_basis.point_from_world_to_local(world_pos));
    }
  }

  //////////////////////////////////////////////////////////////
  const ggo::object3d * object3d::handle_self_intersection(ggo::ray3d_float & ray, bool inside) const
  {
    if (inside == false && _shape && _shape->is_convex() == true)
    {
      return this;
    }
     
    ray.pos() += 0.001f * ray.dir();
    return nullptr;
  }

  //////////////////////////////////////////////////////////////
  bool object3d::intersect_ray(const ggo::ray3d_float & ray, float & dist, ggo::ray3d_float & local_normal, ggo::ray3d_float & world_normal) const
  {
    if (!_shape)
    {
      return false;
    }

    if (_discard_basis == true)
    {
      if (_shape->intersect_ray(ray, dist, local_normal) == false)
      {
        return false;
      }

      world_normal = local_normal;
    }
    else
    {
      if (_shape->intersect_ray(_basis.ray_from_world_to_local(ray), dist, local_normal) == false)
      {
        return false;
      }

      world_normal = _basis.ray_from_local_to_world(local_normal);
    }

    return true;
  }

  //////////////////////////////////////////////////////////////
  ggo::pos3f object3d::sample_point(const ggo::pos3f & target_pos, float random_variable1, float random_variable2) const
  {
    // If no shape, we return point at the basis position. Use full for point lights.
    if (!_shape)
    {
      return _discard_basis ? ggo::pos3f(0.f, 0.f, 0.f) : _basis.pos();
    }
    else
    {
      if (_discard_basis == true)
      {
        return _shape->sample_point(target_pos, random_variable1, random_variable2);
      }
      else
      {
        ggo::pos3f point = _shape->sample_point(_basis.point_from_world_to_local(target_pos), random_variable1, random_variable2);

        return _basis.point_from_local_to_world(point);
      }
    }
  }

  //////////////////////////////////////////////////////////////
  ggo::ray3d_float object3d::sample_ray(float random_variable1, float random_variable2) const
  {
    if (!_shape)
    {
      return ggo::ray3d_float();
    }

    ggo::ray3d_float ray = _shape->sample_ray(random_variable1, random_variable2);

    if (_discard_basis == false)
    {
      _basis.ray_from_local_to_world(ray);
    }

    return ray;
  }

  //////////////////////////////////////////////////////////////
  ggo::ray3d_float object3d::get_reflected_ray(const ggo::ray3d_float & ray, const ggo::ray3d_float & world_normal) const
  {
    ggo::pos3f reflected_dir(ray.dir() - 2 * ggo::dot(world_normal.dir(), ray.dir()) * world_normal.dir());
    GGO_ASSERT(reflected_dir.is_normalized(0.001f) == true);
    GGO_ASSERT(ggo::dot(reflected_dir, world_normal.dir()) >= -0.001f); // Because of rounding errors, the dot product can be a little bit negative.

    return ggo::ray3d_float(world_normal.pos(), reflected_dir, false);
  }

  //////////////////////////////////////////////////////////////
  ggo::ray3d_float object3d::sample_reflection_ray(const ggo::ray3d_float & ray, const ggo::ray3d_float & world_normal, float random_variable1, float random_variable2) const
  {
    ggo::pos3f reflected_dir(ray.dir() - 2 * ggo::dot(world_normal.dir(), ray.dir()) * world_normal.dir());

    // Glossy materials.
    if (_roughness > 0)
    {
      reflected_dir = ggo::hemisphere_sampling(reflected_dir, random_variable1, random_variable2, _roughness * ggo::pi<float>() / 2);

      // Handle the case when the reflected ray goes inside the hit object.
      float dot = ggo::dot(world_normal.dir(), reflected_dir);
      if (dot < 0)
      {
        reflected_dir -= 2.f * dot * world_normal.dir();
      }
    }

    GGO_ASSERT(reflected_dir.is_normalized(0.001f) == true);
    GGO_ASSERT_GE(ggo::dot(reflected_dir, world_normal.dir()), -0.001f);

    return ggo::ray3d_float(world_normal.pos(), reflected_dir, false);
  }

  //////////////////////////////////////////////////////////////
  bool object3d::transmit_ray(ggo::ray3d_float & ray, ggo::ray3d_float world_normal, int & depth) const
  {
    // Transmit the ray into the current object.
    if (ggo::raytracer::transmit_ray(ray, world_normal, 1.0f, _density) == false)
    {
      return false;
    }

    // Bounce if ray internally until it leaves the current object.
    while (true)
    {
      depth -= 1;
      if (depth <= 0)
      {
        return false;
      }

      // Find intersection between the current ray and the current object.
      float dist = -1.f;
      if (_shape->intersect_ray(ray, dist, world_normal) == false)
      {
        return false;
      }

      // Does the ray gest out of the objects?
      if (ggo::raytracer::transmit_ray(ray, world_normal, _density, 1.f) == true)
      {
        return true;
      }
    }

    GGO_FAIL();

    return false;
  }
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  std::shared_ptr<ggo::object3d> create_sphere_light(const ggo::color_32f & color, float radius, const ggo::pos3f & pos)
  {
    auto shape = std::make_shared<ggo::centered_sphere3d<float>>(radius);
    auto object = std::make_shared<ggo::object3d>(shape);

    object->basis().pos() = pos;
    object->set_emissive_color(color);

    return object;
  }

  //////////////////////////////////////////////////////////////
  std::shared_ptr<ggo::object3d> create_point_light(const ggo::color_32f & color, const ggo::pos3f & pos)
  {
    auto object = std::make_shared<ggo::object3d>();

    object->basis().pos() = pos;
    object->set_emissive_color(color);

    return object;
  }
}

