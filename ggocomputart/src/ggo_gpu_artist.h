#ifndef __GGO_GPU_ARTIST_ABC__
#define __GGO_GPU_ARTIST_ABC__

#include <ggo_artist_ids.h>
#include <kernel/ggo_kernel.h>
#include <string>
#include <map>
#include <vector>
#include <variant>

namespace ggo
{
  class uniform final
  {
  public:

    template <typename data_t>
    uniform(const std::vector<data_t> & v, int n) : _variant(v), _n(n) { GGO_ASSERT(v.size() % n == 0); }

    template<typename data_t, size_t s>
    uniform(const data_t(&a)[s]) : uniform(std::vector<data_t>(a, a + s), s) { }

    uniform(float v) : uniform(std::vector<float>(1, v), 1) { }
    uniform(int v) : uniform(std::vector<int>(1, v), 1) { }

    template <typename func_int_t, typename func_float_t>
    void visit(func_int_t && func_int, func_float_t && func_float) const
    {
      switch (_variant.index())
      {
      case 0:
        func_int(std::get<std::vector<int>>(_variant).data(), std::get<std::vector<int>>(_variant).size() / _n, _n);
        break;
      case 1:
        func_float(std::get<std::vector<float>>(_variant).data(), std::get<std::vector<float>>(_variant).size() / _n, _n);
        break;
      }
    }

  private:

    const std::variant<std::vector<int>, std::vector<float>> _variant;
    const int _n;
  };

  class gpu_artist
  {
  public:

    static gpu_artist * create(gpu_artist_id artist_id);

    virtual std::string get_fragment_shader() const = 0;
    virtual std::map<std::string, ggo::uniform> update(bool & finished) = 0;
  };
}

namespace ggo
{
  class progress_gpu_artist : public gpu_artist
  {
  public:

    progress_gpu_artist(ggo::ratio duration);

  private:

    virtual std::map<std::string, ggo::uniform> update(float progress) = 0;

    std::map<std::string, ggo::uniform> update(bool & finished) override;

  private:

    const std::chrono::time_point<std::chrono::steady_clock> _start_time = std::chrono::steady_clock::now();
    const ggo::ratio _duration;
  };
}

#endif
