#ifndef __GGO_POUPETTE_GPU_ARTIST__
#define __GGO_POUPETTE_GPU_ARTIST__

#include <ggo_gpu_artist.h>
#include <chrono>
#include "ggo_poupette_artist.h"

namespace ggo
{
  class poupette_gpu_artist : public progress_gpu_artist
  {
  public:

    poupette_gpu_artist() : progress_gpu_artist({ 10, 1 }), _artist(8.f) {}

  private:

    std::string get_fragment_shader() const override;
    std::map<std::string, ggo::value> update(float progress) override;

  private:

    ggo::poupette_artist _artist;
  };
}

#endif