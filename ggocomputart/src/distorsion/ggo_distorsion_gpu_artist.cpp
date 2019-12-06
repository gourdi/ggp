#include "ggo_distorsion_gpu_artist.h"

//////////////////////////////////////////////////////////////
ggo::distorsion_gpu_artist::distorsion_gpu_artist() : progress_gpu_artist(12)
{
}

//////////////////////////////////////////////////////////////
std::string ggo::distorsion_gpu_artist::get_fragment_shader() const
{
  std::string source{
#include "distorsion.glsl"
  };

  return source;
}

//////////////////////////////////////////////////////////////
std::map<std::string, ggo::uniform> ggo::distorsion_gpu_artist::update(float progress)
{
  std::vector<float> stripes;
  _artist.for_each_stripe([&](const distorsion_artist::stripe& stripe)
    {
      stripes.push_back(stripe._x_sup);
      stripes.push_back(stripe._color.r());
      stripes.push_back(stripe._color.g());
      stripes.push_back(stripe._color.b());
    });

  std::vector<float> transforms;
  _artist.for_each_transform(progress, [&](ggo::pos2_f center, float disp, float variance)
    {
      transforms.push_back(center.x());
      transforms.push_back(center.y());
      transforms.push_back(disp);
      transforms.push_back(variance);
    });

  return { { "stripes", { stripes, 1 } }, { "transforms", { transforms, 1 } } };
}

