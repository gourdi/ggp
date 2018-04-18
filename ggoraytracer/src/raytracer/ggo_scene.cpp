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
  glow & scene::add_glow(const ggo::pos3f & pos, float opacity, const ggo::color_32f & color, float inner_radius, float outter_radius)
  {
    auto object = std::make_shared<glow>(pos, opacity, color, inner_radius, outter_radius);

    _visible_objects.push_back(object);

    return *object;
  }

  //////////////////////////////////////////////////////////////
  point_light & scene::add_point_light(const ggo::color_32f & color, const ggo::pos3f & pos)
  {
    auto light = std::make_shared<point_light>(pos, color);

    _lights.push_back(light);

    return *light;
  }
}

