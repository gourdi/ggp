#include "ggo_scene.h"
#include <raytracer/objects3d/ggo_point_light.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  scene::scene(std::shared_ptr<const ggo::background3d_abc> background, std::shared_ptr<const ggo::indirect_lighting_abc> indirect_lighting)
  :
  _background(background), _indirect_lighting(indirect_lighting)
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

