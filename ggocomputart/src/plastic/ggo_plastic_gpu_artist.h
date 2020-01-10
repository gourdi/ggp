#ifndef __GGO_PLASTIC_GPU_ARTIST__
#define __GGO_PLASTIC_GPU_ARTIST__

#include <ggo_gpu_artist.h>
#include "ggo_plastic_artist.h"

namespace ggo
{
  class plastic_gpu_artist : public progress_gpu_artist
  {
  public:

    plastic_gpu_artist();

  private:

    std::string                         get_fragment_shader() const override;
    std::map<std::string, ggo::uniform> update(float progress, ggo::size render_size) override;

  private:

    plastic_artist _artist;
  };
}

#endif
