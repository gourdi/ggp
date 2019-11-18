#include "ggo_poupette_gpu_artist.h"
#include <kernel/ggo_ease.h>

//////////////////////////////////////////////////////////////
std::string ggo::poupette_gpu_artist::get_fragment_shader() const
{
  std::string source{
#include "poupette.glsl"
  };

  return source;
}

//////////////////////////////////////////////////////////////
std::map<std::string, ggo::value> ggo::poupette_gpu_artist::update(float progress)
{
  auto colors = _artist.interpolate_colors(ggo::ease_inout(progress));

  auto to_vec3 = [](ggo::rgb_32f c)
  {
    return vec3(c.r(), c.g(), c.b());
  };

  return {
    { "c1", to_vec3(colors[0]) },
    { "c2", to_vec3(colors[1]) },
    { "c3", to_vec3(colors[2]) },
    { "c4", to_vec3(colors[3]) } };
}


