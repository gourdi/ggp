#include "ggo_gpu_artist.h"
#include "poupette/ggo_poupette_gpu_artist.h"

namespace ggo
{
  //////////////////////////////////////////////////////////////
  gpu_artist * gpu_artist::create(gpu_artist_id artist_id)
  {
    switch (artist_id)
    {
    case gpu_artist_id::poupette:
      return new poupette_gpu_artist();

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
  progress_gpu_artist::progress_gpu_artist(ggo::ratio duration) :
    _duration(duration)
  {

  }

  //////////////////////////////////////////////////////////////
  std::map<std::string, ggo::value> progress_gpu_artist::update(bool & finished)
  {
    auto now = std::chrono::steady_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(now - _start_time);

    float progress = (0.001f * static_cast<float>(elapsed_time.count()) * static_cast<float>(_duration._den))  / static_cast<float>(_duration._num);

    if (progress > 1.f)
    {
      finished = true;
      return {};
    }
    finished = false;

    return update(progress);
  }
}
