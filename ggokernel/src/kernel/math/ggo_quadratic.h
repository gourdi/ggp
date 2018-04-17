#ifndef __GGO_QUADRATIC__
#define __GGO_QUADRATIC__

namespace ggo
{
  template <typename data_t>
  bool quadratic_solve(data_t deg2, data_t deg1, data_t deg0, data_t & sol1, data_t & sol2)
  {
    data_t d = deg1 * deg1 - 4 * deg2 * deg0;
    if (d < 0)
    {
      return false;
    }
    
    data_t e = std::sqrt(d);
    data_t f = -1 / (2 * deg2);
    sol1 = (deg1 - e) * f;
    sol2 = (deg1 + e) * f;
    
    return true;
  }
}

#endif
