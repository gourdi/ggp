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
std::map<std::string, ggo::uniform> ggo::poupette_gpu_artist::update(float progress)
{
  auto colors = _artist.interpolate_colors(ggo::ease_inout(progress));

  return {
    { "c1", colors[0]._coefs },
    { "c2", colors[1]._coefs },
    { "c3", colors[2]._coefs },
    { "c4", colors[3]._coefs } };
}


