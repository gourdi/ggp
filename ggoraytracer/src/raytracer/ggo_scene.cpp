#include "ggo_scene.h"
#include <raytracer/objects3d/ggo_point_light.h>
#include <raytracer/objects3d/ggo_shape_light.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  scene::scene(std::shared_ptr<const ggo::background3d_abc> background)
  :
  _background(background)
  {

  }

  //////////////////////////////////////////////////////////////
  point_light & scene::add_point_light(const ggo::color_32f & color, const ggo::pos3f & pos)
  {
    auto light = std::make_shared<point_light>(pos, color);

    _lights.push_back(light);

    return *light;
  }
}

