#include <kernel/ggo_reduce.h>
#include <kernel/nonreg/ggo_nonreg.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(reduce, variadic)
{
  static_assert(ggo::max(3, 5, 2, 4) == 5);
  static_assert(ggo::min(3, 5, 2, 4) == 2);

  static_assert(ggo::sum(1_u8, 4_u8, 3_u8) == 8_u8);
  static_assert(ggo::sum(1, 4, 3) == 8);

  static_assert(ggo::mul(1, 4, 3) == 12);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(reduce, fixed_size_array)
{
  constexpr int b[4] = { 3, 5, 2, 4 };
  static_assert(ggo::max(b) == 5);
  static_assert(ggo::min(b) == 2);
  static_assert(ggo::sum(b) == 14);
  static_assert(ggo::mul(b) == 120);
}
