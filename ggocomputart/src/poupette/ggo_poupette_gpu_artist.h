#ifndef __GGO_POUPETTE_GPU_ARTIST__
#define __GGO_POUPETTE_GPU_ARTIST__

#include <ggo_gpu_artist_abc.h>
#include <chrono>
#include "ggo_poupette_artist.h"

namespace ggo
{
  class poupette_gpu_artist : public gpu_artist_abc
  {
  public:

    poupette_gpu_artist() : _artist(8.f) {}

  private:

    std::string get_fragment_shader() const override;
    std::map<std::string, ggo::value> update(bool & finished, std::chrono::milliseconds elapsed_time) override;

  private:

    ggo::poupette_artist _artist;
  };
}

#endif