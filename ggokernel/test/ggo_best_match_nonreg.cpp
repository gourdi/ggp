#include <ggo_nonreg.h>
#include <ggo_best_match.h>
#include <vector>

/////////////////////////////////////////////////////////////////////
// Find the lowest coef.
GGO_TEST(best_match, best_match1)
{
  const std::vector<int> data {9, 4, 8, 2, 3, 4, 5};

  {
    int index = 0;
    
    auto candidate_func = [&]() { return data[index++]; };
    auto evaluate_func = [](int v) { return v; };
    int result = ggo::best_match_min<int>(candidate_func, evaluate_func, static_cast<int>(data.size()));
    GGO_CHECK(result == 2);
  }
  
  {
    int index = 0;
    
    auto candidate_func = [&]() { return data[index++]; };
    int result = ggo::best_match<int>(candidate_func, std::less<int>(), static_cast<int>(data.size()));
    GGO_CHECK(result == 2);
  }

  {
    int index = 0;
    
    auto candidate_func = [&]() { return data[index++]; };
    auto evaluate_func = [](int v) { return v; };
    auto compare_func = [](int v1, int v2) { return v1 < v2; };
    int result = ggo::best_match<int>(candidate_func, evaluate_func, compare_func, static_cast<int>(data.size()));
    GGO_CHECK(result == 2);
  }
}

/////////////////////////////////////////////////////////////////////
// Find the greater coef.
GGO_TEST(best_match, best_match2)
{
  const std::vector<int> data {9, 4, 8, 2, 3, 4, 5};

  {
    int index = 0;
    
    auto candidate_func = [&]() { return data[index++]; };
    auto evaluate_func = [](int v) { return v; };
    int result = ggo::best_match_max<int>(candidate_func, evaluate_func, static_cast<int>(data.size()));
    GGO_CHECK(result == 9);
  }

  {
    int index = 0;
    
    auto candidate_func = [&]() { return data[index++]; };
    int result = ggo::best_match<int>(candidate_func, std::greater<int>(), static_cast<int>(data.size()));
    GGO_CHECK(result == 9);
  }
  
  {
    int index = 0;
    
    auto candidate_func = [&]() { return data[index++]; };
    auto compare_func = [](int v1, int v2) { return v1 > v2; };
    int result = ggo::best_match<int>(candidate_func, compare_func, static_cast<int>(data.size()));
    GGO_CHECK(result == 9);
  }
}

/////////////////////////////////////////////////////////////////////
// Find the closest coef to 7.
GGO_TEST(best_match, best_match3)
{
  const std::vector<int> data {9, 4, 2, 3, 4, 5, 8};

  {
    int index = 0;
    
    auto candidate_func = [&]() { return data[index++]; };
    auto compare_func = [](int v1, int v2) { return std::abs(v1 - 7) < std::abs(v2 - 7); };
    int result = ggo::best_match<int>(candidate_func, compare_func, static_cast<int>(data.size()));
    GGO_CHECK(result == 8);
  }
  
  {
    int index = 0;
    
    auto candidate_func = [&]() { return data[index++]; };
    auto evaluate_func = [](int v) { return v - 7; };
    auto compare_func = [](int v1, int v2) { return std::abs(v1) < std::abs(v2); };
    int result = ggo::best_match<int>(candidate_func, evaluate_func, compare_func, static_cast<int>(data.size()));
    GGO_CHECK(result == 8);
  }
}
