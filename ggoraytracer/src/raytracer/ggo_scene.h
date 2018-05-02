#ifndef __GGO_SCENE__
#define __GGO_SCENE__

#include <raytracer/volumetric_effects/ggo_volumetric_effect_abc.h>
#include <raytracer/backgrounds/ggo_background3d_abc.h>
#include <raytracer/objects3d/ggo_diffuse_object3d.h>
#include <raytracer/objects3d/ggo_transparent_object3d.h>
#include <raytracer/objects3d/ggo_simple_color_object3d.h>
#include <raytracer/objects3d/ggo_point_light.h>
#include <raytracer/objects3d/ggo_shape_light.h>
#include <memory>

namespace ggo
{
  class scene final
  {
  public:

    scene(std::shared_ptr<const ggo::background3d_abc> background);

    point_light & add_point_light(const ggo::color_32f & color, const ggo::pos3f & pos);

    template <uint32_t flags, typename shape_t>
    auto & add_shape_light(const shape_t & shape, const ggo::color_32f & color)
    {
      using object_t = shape_light<flags, shape_t>;

      auto object = std::make_shared<object_t>(shape, color);

      _visible_objects.push_back(object);
      _casting_shadows_objects.push_back(object);
      _lights.push_back(object);

      return *object;
    }

    auto & add_sphere_light(const ggo::color_32f & color, const ggo::pos3f & center, float radius)
    {
      constexpr int flags = ggo::discard_roughness | ggo::discard_phong | ggo::discard_reflection;

      auto & object = add_shape_light<flags>(ggo::centered_sphere3d_float(radius), color);
      object.basis().pos() = center;

      return object;
    }

    template <uint32_t flags, typename shape_t, typename material_t>
    auto & add_simple_color_object(const shape_t & shape, const material_t & material)
    {
      using object_t = simple_color_object3d<flags, shape_t, material_t>;

      auto object = std::make_shared<object_t>(shape, material);

      _visible_objects.push_back(object);
      _casting_shadows_objects.push_back(object);

      return *object;
    }

    template <uint32_t flags, typename shape_t, typename material_t>
    auto & add_diffuse_object(const shape_t & shape, const material_t & material)
    {
      using object_t = diffuse_object3d<flags, shape_t, material_t>;

      auto object = std::make_shared<object_t>(shape, material);

      _visible_objects.push_back(object);
      _casting_shadows_objects.push_back(object);

      return *object;
    }

    template <uint32_t flags, typename shape_t>
    auto & add_transparent_object(const shape_t & shape, const ggo::color_32f & color, float density)
    {
      using object_t = transparent_object3d<flags, shape_t>;

      auto object = std::make_shared<object_t>(shape, color, density);

      _visible_objects.push_back(object);
      _casting_shadows_objects.push_back(object);

      return *object;
    }

    template <typename volumetric_effect_t, typename... args>
    void  emplace_volumetric_effect(args&&... a)
    {
      auto effect = std::make_shared<volumetric_effect_t>(std::forward<args>(a)...);
      _volumetric_effects.push_back(effect);
    }

    auto  get_volumetric_effects() const
    {
      return _volumetric_effects;
    }

    void  set_ambient_color(const ggo::color_32f & ambient_color) { _ambient_color = ambient_color; }

    const auto &  background() const { return *_background; }

    const auto &  lights() const { return _lights; }
    const auto &  visible_objects() const { return _visible_objects; }
    const auto &  casting_shadows_objects() const { return _casting_shadows_objects; }

  private:

    std::vector<std::shared_ptr<const ggo::object3d_abc>>           _visible_objects;
    std::vector<std::shared_ptr<const ggo::object3d_abc>>           _casting_shadows_objects;
    std::vector<std::shared_ptr<const ggo::object3d_abc>>           _lights;
    std::vector<std::shared_ptr<const ggo::volumetric_effect_abc>>  _volumetric_effects;
    std::shared_ptr<const ggo::background3d_abc>                    _background;
    ggo::color_32f                                                  _ambient_color = ggo::black<ggo::color_32f>();
  };
}

#endif

