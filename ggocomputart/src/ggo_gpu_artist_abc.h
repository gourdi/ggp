#ifndef __GGO_GPU_ARTIST_ABC__
#define __GGO_GPU_ARTIST_ABC__

#include <ggo_artist_ids.h>
#include <kernel/ggo_value.h>
#include <string>
#include <map>
#include <chrono>

namespace ggo
{
  class gpu_artist_abc
  {
  public:

    static gpu_artist_abc * create(gpu_artist_id artist_id);

    virtual std::string get_fragment_shader() const = 0;

    std::map<std::string, ggo::value> update(bool & finished);

  private:

    virtual std::map<std::string, ggo::value> update(bool & finished, std::chrono::milliseconds elapsed_time) = 0;

  private:

    const std::chrono::time_point<std::chrono::steady_clock> _start = std::chrono::steady_clock::now();
  };
}

#endif
