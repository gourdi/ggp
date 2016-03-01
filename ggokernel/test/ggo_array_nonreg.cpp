#include <ggo_nonreg.h>
#include <ggo_array.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(array, range_loop)
{
  auto test_range_loop_aux = [&](const ggo::array_int & array, int ref_sum, int ref_count)
  {
    int sum = 0;
    int count = 0;
    for (const auto & v : array)
    {
      sum += v;
      count += 1;
    }

    GGO_CHECK(sum == ref_sum);
    GGO_CHECK(count == ref_count);
  };

  ggo::array_int array(3);
  array[0] = 2;
  array[1] = 5;
  array[2] = 7;

  test_range_loop_aux(array, 14, 3);

  for (auto & v : array)
  {
    v += 2;
  }

  test_range_loop_aux(array, 20, 3);
}
