#include "ggo_gpu_artist.h"
#include "poupette/ggo_poupette_gpu_artist.h"
#include "plastic/ggo_plastic_gpu_artist.h"
#include "distorsion/ggo_distorsion_gpu_artist.h"
#include "sonson/ggo_sonson_gpu_artist.h"

namespace ggo
{
  //////////////////////////////////////////////////////////////
  gpu_artist * gpu_artist::create(gpu_artist_id artist_id)
  {
    switch (artist_id)
    {
    case gpu_artist_id::poupette:
      return new poupette_gpu_artist();
    case gpu_artist_id::plastic:
      return new plastic_gpu_artist();
    case gpu_artist_id::distorsion:
      return new distorsion_gpu_artist();
    case gpu_artist_id::sonson:
      return new sonson_gpu_artist();
    default:
      GGO_FAIL();
      return nullptr;
    };

    return nullptr;
  }
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  progress_gpu_artist::progress_gpu_artist(ggo::ratio duration)
  :
  _duration(duration)
  {

  }

  //////////////////////////////////////////////////////////////
  std::map<std::string, ggo::uniform> progress_gpu_artist::update(bool & finished, ggo::size render_size)
  {
    auto now = std::chrono::steady_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(now - _start_time);

    float progress = 0.001f * to<float>(elapsed_time.count() / _duration);

    if (progress > 1.f)
    {
      finished = true;
      return {};
    }
    finished = false;

    return update(progress, render_size);
  }
}
