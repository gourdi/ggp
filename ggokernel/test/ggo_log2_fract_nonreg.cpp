#include <kernel/ggo_kernel.h>
#include <kernel/nonreg/ggo_nonreg.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(log2_fract, multiplication)
{
  static_assert(ggo::one_log2_fract * 5 == 5);

  uint32_t r1 = 21 * ggo::log2_fract<3>(3); // 21 * 3 / 8
  GGO_CHECK_EQ(r1, 8);

  uint32_t r2 = ggo::log2_fract<3>(3) * 22; // 22 * 3 / 8
  GGO_CHECK_EQ(r1, 8);
}