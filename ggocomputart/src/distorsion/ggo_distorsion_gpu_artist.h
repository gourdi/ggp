#ifndef __GGO_DISTORSION_GPU_ARTIST__
#define __GGO_DISTORSION_GPU_ARTIST__

#include <ggo_gpu_artist.h>
#include <2d/ggo_color.h>
#include <distorsion/ggo_distorsion_artist.h>

namespace ggo
{
  class distorsion_gpu_artist : public progress_gpu_artist
  {
  public:

    distorsion_gpu_artist();

  private:

    std::string                         get_fragment_shader() const override;
    std::map<std::string, ggo::uniform> update(float progress, ggo::size render_size) override;

  private:

    distorsion_artist _artist;
  };
}

#endif
