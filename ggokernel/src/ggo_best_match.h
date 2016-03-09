#ifndef __GGO_BEST_MATCH__
#define __GGO_BEST_MATCH__

#include <functional>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_type, typename create_func, typename compare_func>
  data_type best_match(create_func create, compare_func compare, int tries_count)
  {
    data_type candidate = create();
    --tries_count;
    
    while (tries_count > 0)
    {
      data_type new_candidate = create();
      
      if (compare(new_candidate, candidate) == true)
      {
        candidate = new_candidate;
      }
    
      --tries_count;
    }
    
    return candidate;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_type, typename create_func, typename evaluate_func, typename compare_func>
  data_type best_match(create_func create, evaluate_func evaluate, compare_func compare, int tries_count)
  {
    data_type candidate = create();
    auto best_eval = evaluate(candidate);
    --tries_count;
    
    while (tries_count > 0)
    {
      data_type new_candidate = create();
      auto new_eval = evaluate(new_candidate);

      if (compare(new_eval, best_eval) == true)
      {
        candidate = new_candidate;
        best_eval = new_eval;
      }
      
      --tries_count;
    }
    
    return candidate;
  }
  
  //////////////////////////////////////////////////////////////
  template <typename data_type, typename create_func, typename evaluate_func>
  data_type best_match_min(create_func create, evaluate_func evaluate, int tries_count)
  {
    return best_match<data_type>(create, evaluate, std::less<data_type>(), tries_count);
  }
  
  //////////////////////////////////////////////////////////////
  template <typename data_type, typename create_func, typename evaluate_func>
  data_type best_match_max(create_func create, evaluate_func evaluate, int tries_count)
  {
    return best_match<data_type>(create, evaluate, std::greater<data_type>(), tries_count);
  }
}

#endif
