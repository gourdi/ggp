#include <kernel/ggo_kernel.h>
#include <kernel/nonreg/ggo_nonreg.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(log2_fract, multiplication)
{
  static_assert(ggo::one_log2_fract * 5 == 5);

  static_assert(21 * ggo::log2_fract<3>(3) == 8); // 21 * 3 / 8 = 7.875
  static_assert(22 * ggo::log2_fract<3>(3) == 8); // 22 * 3 / 8 = 8.25
  static_assert(23 * ggo::log2_fract<3>(3) == 9); // 23 * 3 / 8 = 8.625
}