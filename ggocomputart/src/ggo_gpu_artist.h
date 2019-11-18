#ifndef __GGO_GPU_ARTIST_ABC__
#define __GGO_GPU_ARTIST_ABC__

#include <ggo_artist_ids.h>
#include <kernel/ggo_kernel.h>
#include <kernel/ggo_value.h>
#include <string>
#include <map>
#include <chrono>

namespace ggo
{
  class gpu_artist
  {
  public:

    static gpu_artist * create(gpu_artist_id artist_id);

    virtual std::string get_fragment_shader() const = 0;
    virtual std::map<std::string, ggo::value> update(bool & finished) = 0;
  };
}

namespace ggo
{
  class progress_gpu_artist : public gpu_artist
  {
  public:

    progress_gpu_artist(ggo::ratio duration);

  private:

    virtual std::map<std::string, ggo::value> update(float progress) = 0;

    std::map<std::string, ggo::value> update(bool & finished) override;

  private:

    const std::chrono::time_point<std::chrono::steady_clock> _start_time = std::chrono::steady_clock::now();
    const ggo::ratio _duration;
  };
}

#endif
