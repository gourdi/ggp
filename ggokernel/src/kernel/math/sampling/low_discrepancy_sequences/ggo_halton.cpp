#include "ggo_halton.h"

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  std::vector<std::pair<int, int>>  halton_1d(int base, int count)
  {
    std::vector<std::pair<int, int>> result;

    for (int i = 1; i < base; ++i)
    {
      result.push_back(std::make_pair(i, base));
      
      if (result.size() == count)
      {
        return result;
      }
    }
    
    int first = 0;
    int last = static_cast<int>(result.size()) - 1;
    int prv_den = base;
    
    while (true)
    {
      for (int i = first; i <= last; ++i)
      {
        for (int j = 0; j < base; ++j)
        {
          result.push_back(std::make_pair(result[i].first + j * prv_den, prv_den * base));
          
          if (result.size() == count)
          {
            return result;
          }
        }
      }

      prv_den *= base;
      first = last + 1;
      last = static_cast<int>(result.size()) - 1;
    }
    
    return result;
  }
  
  /////////////////////////////////////////////////////////////////////
  std::vector<ggo::pos2_f> halton_2d(int base1, int base2, int count)
  {
    std::vector<ggo::pos2_f> result;
    
    auto result1 = halton_1d(base1, count);
    auto result2 = halton_1d(base2, count);
    
    for (int i = 0; i < count; ++i)
    {
      float x = static_cast<float>(result1[i].first) / static_cast<float>(result1[i].second);
      float y = static_cast<float>(result2[i].first) / static_cast<float>(result2[i].second);
      
      result.emplace_back(x, y);
    }
    
    return result;
  }
}
