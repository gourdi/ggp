#ifndef __GGO_SONSON_GPU_ARTIST__
#define __GGO_SONSON_GPU_ARTIST__

#include <ggo_gpu_artist.h>
#include <sonson/ggo_sonson_artist.h>

namespace ggo
{
  class sonson_gpu_artist : public progress_gpu_artist
  {
  public:

    sonson_gpu_artist();

    std::string get_fragment_shader() const override;
    std::map<std::string, ggo::uniform> update(float progress, ggo::size render_size) override;

  private:

    std::chrono::steady_clock::time_point _last_frame_time_point = std::chrono::steady_clock::now();
    ggo::sonson_artist _artist;
  };
}

#endif