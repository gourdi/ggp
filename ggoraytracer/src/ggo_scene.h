#ifndef __GGO_SCENE__
#define __GGO_SCENE__

#include <ggo_fog_abc.h>
#include <ggo_background3d_abc.h>
#include <ggo_diffuse_object3d.h>
#include <ggo_transparent_object3d.h>
#include <ggo_simple_color_object3d.h>
#include <memory>

namespace ggo
{
  class scene final
  {
  public:

          scene(std::shared_ptr<const ggo::background3d_abc> background);

    void  add_point_light(const ggo::color_32f & color, const ggo::pos3f & pos);
    void  add_sphere_light(const ggo::color_32f & color, const ggo::pos3f & center, float radius);

    template <uint32_t flags, typename shape_t, typename material_t>
    ggo::simple_color_object3d<flags, shape_t, material_t> & add_simple_color_object(const shape_t & shape, const material_t & material)
    {
      using object_t = simple_color_object3d<flags, shape_t, material_t>;

      auto object = std::make_shared<object_t>(shape, material);

      _objects.push_back(object);

      return *object;
    }

    template <uint32_t flags, typename shape_t, typename material_t>
    ggo::diffuse_object3d<flags, shape_t, material_t> & add_diffuse_object(const shape_t & shape, const material_t & material)
    {
      using object_t = diffuse_object3d<flags, shape_t, material_t>;

      auto object = std::make_shared<object_t>(shape, material);

      _objects.push_back(object);

      return *object;
    }

    template <uint32_t flags, typename shape_t>
    ggo::transparent_object3d<flags, shape_t> & add_transparent_object(const shape_t & shape, const ggo::color_32f & color, float density)
    {
      using object_t = transparent_object3d<flags, shape_t>;

      auto object = std::make_shared<object_t>(shape, color, density);

      _objects.push_back(object);

      return *object;
    }

    void  set_fog(std::shared_ptr<const ggo::fog_abc> fog) { _fog = fog; }
    void  set_ambient_color(const ggo::color_32f & ambient_color) { _ambient_color = ambient_color; }

    const auto *  fog() const { return _fog.get(); }
    const auto &  background() const { return *_background; }

    const auto &  lights() const { return _lights; }
    const auto &  objects() const { return _objects; }

  private:

    std::vector<std::shared_ptr<const ggo::object3d_abc>> _objects;
    std::vector<std::shared_ptr<const ggo::object3d_abc>> _lights;
    std::shared_ptr<const ggo::background3d_abc>          _background;
    ggo::color_32f                                        _ambient_color = ggo::black<ggo::color_32f>();
    std::shared_ptr<const ggo::fog_abc>                   _fog;
  };
}

#endif

