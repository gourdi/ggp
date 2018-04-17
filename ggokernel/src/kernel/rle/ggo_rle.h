#ifndef __GGO_RLE__
#define __GGO_RLE__

#include <vector>
#include <initializer_list>
#include <kernel/memory/ggo_array.h>

/////////////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename data_t, typename read_func, typename done_func>
  std::vector<std::pair<data_t, int>> rle_encode(read_func read, done_func done);

  template <typename data_t, typename write_func>
  void rle_decode(const std::vector<std::pair<data_t, int>> & rle_data, write_func write);
}

/////////////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename data_t>
  std::vector<std::pair<data_t, int>> rle_encode(const std::initializer_list<data_t> & input)
  {
    auto it = input.begin();
    return rle_encode<data_t>([&]() { return *it++; }, [&]() { return it == input.end(); });
  }

  template <typename data_t>
  std::vector<std::pair<data_t, int>> rle_encode(const ggo::array<data_t, 1> & input)
  {
    auto it = input.begin();
    return rle_encode<data_t>([&]() { return *it++; }, [&]() { return it == input.end(); });
  }

  template <typename data_t>
  void rle_decode(const std::vector<std::pair<data_t, int>> & rle_data, ggo::array<data_t, 1> & output)
  {
    auto it = output.begin();
    rle_decode<data_t>(rle_data, [&](const data_t & value, int count) { std::fill(it, it + count, value); it += count; });
  }
}

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename data_t, typename read_func, typename done_func>
  std::vector<std::pair<data_t, int>> rle_encode(read_func read, done_func done)
  {
    std::vector<std::pair<data_t, int>> rle_data;

    data_t cur_value = read();
    int count = 1;
    while (done() == false)
    {
      const data_t & nxt_value = read();
      if (cur_value == nxt_value)
      {
        ++count;
      }
      else
      {
        rle_data.push_back(std::make_pair(cur_value, count));
        count = 1;
        cur_value = nxt_value;
      }
    }

    rle_data.push_back(std::make_pair(cur_value, count)); // Don't forget the last element.

    return rle_data;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t, typename write_func>
  void rle_decode(const std::vector<std::pair<data_t, int>> & rle_data, write_func write)
  {
    for (const auto & rle_item : rle_data)
    {
      write(rle_item.first, rle_item.second);
    }
  }
}

#endif
