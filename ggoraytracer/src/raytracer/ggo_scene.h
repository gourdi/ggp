#ifndef __GGO_SCENE__
#define __GGO_SCENE__

#include <raytracer/backgrounds/ggo_background3d_abc.h>
#include <raytracer/objects3d/ggo_point_light.h>
#include <raytracer/objects3d/solid_objects/ggo_diffuse_object3d.h>
#include <raytracer/objects3d/solid_objects/ggo_transparent_object3d.h>
#include <raytracer/objects3d/solid_objects/ggo_simple_color_object3d.h>
#include <raytracer/objects3d/solid_objects/ggo_shape_light.h>

#include <memory>

namespace ggo
{
  class scene final
  {
  public:

    scene(std::shared_ptr<const ggo::background3d_abc> background, std::shared_ptr<const ggo::indirect_lighting_abc> indirect_lighting = nullptr);

    point_light & add_point_light(const ggo::color_32f & color, const ggo::pos3f & pos);

    template <uint32_t flags, typename shape_t>
    auto & add_shape_light(const shape_t & shape, const ggo::color_32f & color)
    {
      using object_t = shape_light<flags, shape_t>;

      auto object = std::make_shared<object_t>(shape, color);

      _solid_objects.push_back(object);
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

      _solid_objects.push_back(object);

      return *object;
    }

    template <uint32_t flags, typename shape_t, typename material_t>
    auto & add_diffuse_object(const shape_t & shape, const material_t & material)
    {
      using object_t = diffuse_object3d<flags, shape_t, material_t>;

      auto object = std::make_shared<object_t>(shape, material);

      _solid_objects.push_back(object);

      return *object;
    }

    template <uint32_t flags, typename shape_t>
    auto & add_transparent_object(const shape_t & shape, const ggo::color_32f & color, float density)
    {
      using object_t = transparent_object3d<flags, shape_t>;

      auto object = std::make_shared<object_t>(shape, color, density);

      _solid_objects.push_back(object);

      return *object;
    }

    template <typename volumetric_object_t, typename... args>
    void emplace_volumetric_object(args&&... a)
    {
      auto object = std::make_shared<volumetric_object_t>(std::forward<args>(a)...);
      _volumetric_objects.push_back(object);
    }

    void  set_ambient_color(const ggo::color_32f & ambient_color) { _ambient_color = ambient_color; }

    const auto &  solid_objects() const { return _solid_objects; }
    const auto &  lights() const { return _lights; }
    const auto &  volumetric_objects() const { return _volumetric_objects; }
    const auto &  background() const { return *_background; }
    
  private:

    std::vector<std::shared_ptr<const ggo::object3d_abc>> _solid_objects;
    std::vector<std::shared_ptr<const ggo::object3d_abc>> _lights;
    std::vector<std::shared_ptr<const ggo::object3d_abc>> _volumetric_objects;
    std::shared_ptr<const ggo::background3d_abc>          _background;
    std::shared_ptr<const ggo::indirect_lighting_abc>     _indirect_lighting;
    ggo::color_32f                                        _ambient_color = ggo::black<ggo::color_32f>();
  };
}

#endif

