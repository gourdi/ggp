#ifndef __GGO_BEST_MATCH__
#define __GGO_BEST_MATCH__

#include <functional>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename T, typename CandidateFunc, typename CompareFunc>
  T best_match(CandidateFunc candidate_func, CompareFunc compare_func, int tries_count)
  {
    T candidate = candidate_func();
    --tries_count;
    
    while (tries_count > 0)
    {
      T new_candidate = candidate_func();
      
      if (compare_func(new_candidate, candidate) == true)
      {
        candidate = new_candidate;
      }
    
      --tries_count;
    }
    
    return candidate;
  }

  //////////////////////////////////////////////////////////////
  template <typename T, typename CandidateFunc, typename EvaluateFunc, typename CompareFunc>
  T best_match(CandidateFunc candidate_func, EvaluateFunc evaluate_func, CompareFunc compare_func, int tries_count)
  {
    T candidate = candidate_func();
    auto best_eval = evaluate_func(candidate);
    --tries_count;
    
    while (tries_count > 0)
    {
      T new_candidate = candidate_func();
      auto new_eval = evaluate_func(new_candidate);

      if (compare_func(new_eval, best_eval) == true)
      {
        candidate = new_candidate;
        best_eval = new_eval;
      }
      
      --tries_count;
    }
    
    return candidate;
  }
  
  //////////////////////////////////////////////////////////////
  template <typename T, typename CandidateFunc, typename EvaluateFunc>
  T best_match_min(CandidateFunc candidate_func, EvaluateFunc evaluate_func, int tries_count)
  {
    return best_match<T>(candidate_func, evaluate_func, std::less<T>(), tries_count);
  }
  
  //////////////////////////////////////////////////////////////
  template <typename T, typename CandidateFunc, typename EvaluateFunc>
  T best_match_max(CandidateFunc candidate_func, EvaluateFunc evaluate_func, int tries_count)
  {
    return best_match<T>(candidate_func, evaluate_func, std::greater<T>(), tries_count);
  }
}

#endif
