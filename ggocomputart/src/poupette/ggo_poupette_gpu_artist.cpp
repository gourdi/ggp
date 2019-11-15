#include "ggo_poupette_gpu_artist.h"

//////////////////////////////////////////////////////////////
std::string ggo::poupette_gpu_artist::get_fragment_shader() const
{
  std::string source{
#include "poupette.glsl"
  };

  return source;
}

//////////////////////////////////////////////////////////////
std::map<std::string, ggo::value> ggo::poupette_gpu_artist::update(bool & finished, std::chrono::milliseconds elapsed_time)
{
  float t = static_cast<float>(elapsed_time.count()) / 10000.f;
  if (t >= 1.f)
  {
    finished = true;
    return {};
  }

  finished = false;
  auto colors = _artist.interpolate_colors(t);

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


