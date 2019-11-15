#include "ggo_gpu_artist_abc.h"
#include "poupette/ggo_poupette_gpu_artist.h"

namespace ggo
{
  //////////////////////////////////////////////////////////////
  std::map<std::string, ggo::value> gpu_artist_abc::update(bool & finished)
  {
    auto now = std::chrono::steady_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(now - _start);

    return update(finished, elapsed_time);
  }

  //////////////////////////////////////////////////////////////
  gpu_artist_abc * gpu_artist_abc::create(gpu_artist_id artist_id)
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
