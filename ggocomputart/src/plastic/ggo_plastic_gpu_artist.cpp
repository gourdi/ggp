#include "ggo_plastic_gpu_artist.h"
#include <kernel/ggo_ease.h>

//////////////////////////////////////////////////////////////
ggo::plastic_gpu_artist::plastic_gpu_artist() : progress_gpu_artist({ 10, 1 })
{
  for (int i = 0; i < 50; ++i)
  {
    plastic_gpu_artist::params params;

    params._center = { ggo::rand(-1.5f, 1.5f), ggo::rand(-1.5f, 1.5f) };
    params._radius = ggo::rand(0.1f, 0.5f);
    params._angle_start = ggo::rand(0.f, 2 * ggo::pi<float>());
    params._angle_end = params._angle_start + ggo::rand(4 * ggo::pi<float>(), 6 * ggo::pi<float>());
    params._den = ggo::rand(0.5f, 2.f);
    params._mult = ggo::rand(0.5f, 2.f);
    params._power = ggo::rand(2.f, 3.f);

    _params.push_back(params);
  }

  _color = ggo::from_hsv<ggo::rgb_32f>(ggo::rand<float>(), ggo::rand<float>(), 1);
  _altitude_factor = ggo::rand<float>(0.04f, 0.08f);
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

  float k = ggo::ease_inout(progress);

  for (const auto & param : _params)
  {
    float angle = ggo::linerp(param._angle_start, param._angle_end, k);
    ggo::pos2_f p = param._center + param._radius * ggo::vec2_f::from_angle(angle);

    params.push_back(param._den);
    params.push_back(param._mult);
    params.push_back(param._power);
    params.push_back(p.x());
    params.push_back(p.y());
  }

  return { { "params", { params, 1 } }, { "color", _color._coefs }, { "altitude_factor", _altitude_factor } };
}

