#ifndef __GGO_RLE__
#define __GGO_RLE__

#include <vector>
#include <initializer_list>
#include <ggo_array.h>

/////////////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename data_type, typename read, typename done>
  std::vector<std::pair<data_type, int>> rle_encode(read read, done done);

  template <typename data_type, typename write>
  void rle_decode(const std::vector<std::pair<data_type, int>> & rle_data, write write);
}

/////////////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename data_type>
  std::vector<std::pair<data_type, int>> rle_encode(const std::initializer_list<data_type> & input)
  {
    auto it = input.begin();
    return rle_encode<data_type>([&]() { return *it++; }, [&]() { return it == input.end(); });
  }

  template <typename data_type>
  std::vector<std::pair<data_type, int>> rle_encode(const ggo::array<data_type> & input)
  {
    auto it = input.begin();
    return rle_encode<data_type>([&]() { return *it++; }, [&]() { return it == input.end(); });
  }

  template <typename data_type>
  void rle_decode(const std::vector<std::pair<data_type, int>> & rle_data, ggo::array<data_type> & output)
  {
    auto it = output.begin();
    rle_decode<data_type>(rle_data, [&](const data_type & value, int count) { std::fill(it, it + count, value); it += count; });
  }
}

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename data_type, typename read, typename done>
  std::vector<std::pair<data_type, int>> rle_encode(read read, done done)
  {
    std::vector<std::pair<data_type, int>> rle_data;

    data_type cur_value = read();
    int count = 1;
    while (done() == false)
    {
      const data_type & nxt_value = read();
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
  template <typename data_type, typename write>
  void rle_decode(const std::vector<std::pair<data_type, int>> & rle_data, write write)
  {
    for (const auto & rle_item : rle_data)
    {
      write(rle_item.first, rle_item.second);
    }
  }
}

#endif