#include "ggo_scene.h"
#include <ggo_point_light.h>
#include <ggo_shape_light.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  scene::scene(std::shared_ptr<const ggo::background3d_abc> background)
  :
  _background(background)
  {

  }

  //////////////////////////////////////////////////////////////
  void scene::add_point_light(const ggo::color_32f & color, const ggo::pos3f & pos)
  {
    auto light = std::make_shared<point_light>(pos, color);

    _lights.push_back(light);
  }

  //////////////////////////////////////////////////////////////
  void scene::add_sphere_light(const ggo::color_32f & color, const ggo::pos3f & center, float radius)
  {
    using object3d_t = shape_light<ggo::discard_roughness | ggo::discard_phong | ggo::discard_reflection, ggo::centered_sphere3d_float>;

    auto light = std::make_shared<object3d_t>(ggo::centered_sphere3d_float(radius), color);
    light->basis().pos() = center;

    _objects.push_back(light);
    _lights.push_back(light);
  }
}

