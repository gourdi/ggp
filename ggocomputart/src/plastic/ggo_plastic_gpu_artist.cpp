#include "ggo_plastic_gpu_artist.h"
#include <kernel/ggo_ease.h>

//////////////////////////////////////////////////////////////
ggo::plastic_gpu_artist::plastic_gpu_artist() : progress_gpu_artist(10)
{
}

//////////////////////////////////////////////////////////////
std::string ggo::plastic_gpu_artist::get_fragment_shader() const
{
  std::string source{
#include "plastic.glsl"
  };

  return source;
}

//////////////////////////////////////////////////////////////
std::map<std::string, ggo::uniform> ggo::plastic_gpu_artist::update(float progress)
{
  std::vector<float> params;

  _artist.for_each_param(progress, [&](float den, float mult, float power, ggo::pos2_f pos)
  {
    params.push_back(den);
    params.push_back(mult);
    params.push_back(power);
    params.push_back(pos.x());
    params.push_back(pos.y());
  });

  return { { "params", { params, 1 } }, { "color", _artist.color()._coefs }, { "altitude_factor", _artist.altitude_factor() } };
}

