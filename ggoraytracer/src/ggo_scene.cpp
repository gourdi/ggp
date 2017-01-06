#include "ggo_scene.h"

namespace ggo
{
  //////////////////////////////////////////////////////////////
  scene::scene(const std::vector<std::shared_ptr<const ggo::object3d>> & objects,
               std::shared_ptr<const ggo::background3d_abc> background,
               const ggo::color_32f & ambient_color,
               std::shared_ptr<const ggo::fog_abc> fog)
  :
  _objects(objects),
  _background(background),
  _ambient_color(ambient_color),
  _fog(fog)
  {
    for (const auto & object : objects)
    {
      if (object->get_emissive_color() != ggo::black<ggo::color_32f>())
      {
        _lights.push_back(object);
      }
    }
  }
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  scene_builder::scene_builder(std::shared_ptr<const ggo::background3d_abc> background)
    :
    _background(background)
  {

  }

  //////////////////////////////////////////////////////////////
  std::shared_ptr<ggo::object3d> scene_builder::add_point_light(const ggo::color_32f & color, const ggo::pos3f & pos)
  {
    auto light = ggo::create_point_light(color, pos);

    _objects.push_back(light);

    return light;
  }

  //////////////////////////////////////////////////////////////
  std::shared_ptr<ggo::object3d> scene_builder::add_sphere_light(const ggo::color_32f & color, float radius, const ggo::pos3f & pos)
  {
    auto light = ggo::create_sphere_light(color, radius, pos);

    _objects.push_back(light);

    return light;
  }

  //////////////////////////////////////////////////////////////
  void scene_builder::add_object(std::shared_ptr<ggo::object3d> object, bool discard_basis)
  {
    object->set_discard_basis(discard_basis);

    _objects.push_back(object);
  }

  //////////////////////////////////////////////////////////////
  std::shared_ptr<ggo::object3d> scene_builder::add_object(std::shared_ptr<const ggo::raytracable_shape3d_abc_float> shape, const ggo::color_32f & color, bool discard_basis)
  {
    auto object = std::make_shared<ggo::object3d>(shape, color);

    object->set_discard_basis(discard_basis);

    _objects.push_back(object);

    return object;
  }

  //////////////////////////////////////////////////////////////
  std::shared_ptr<ggo::object3d> scene_builder::add_object(std::shared_ptr<const ggo::raytracable_shape3d_abc_float> shape, std::shared_ptr<const ggo::material_abc> material, bool discard_basis)
  {
    auto object = std::make_shared<ggo::object3d>(shape, material);

    object->set_discard_basis(discard_basis);

    _objects.push_back(object);

    return object;
  }
}
