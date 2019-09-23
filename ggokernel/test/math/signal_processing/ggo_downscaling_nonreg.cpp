#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/signal_proc/ggo_downscaling.h>
#include <array>

/////////////////////////////////////////////////////////////////////
GGO_TEST(downscaling, uint8_2x2)
{
  std::array<uint8_t, 16> in{ 255, 0, 255, 0, 0, 0, 1, 0,
                              255, 0, 255, 0, 0, 0, 1, 0 };
  std::array<uint8_t, 2> out{ 0, 0 };

  ggo::downscale2x2<2, 1>(in.data(), out.data(), 4, 2);
  GGO_CHECK_EQ(out[0], 255);
  GGO_CHECK_EQ(out[1], 1);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(downscaling, uint8_4x4)
{
  std::array<uint8_t, 32> in{ 0, 0, 0, 0, 4, 4, 4, 4,
                              0, 0, 0, 0, 4, 4, 4, 4,
                              2, 2, 2, 2, 8, 9, 7, 8,
                              2, 2, 2, 2, 8, 9, 7, 8, };
  std::array<uint8_t, 2> out{ 0, 0 };

  ggo::downscale4x4(in.data(), out.data(), 8, 4);
  GGO_CHECK_EQ(out[0], 1);
  GGO_CHECK_EQ(out[1], 6);
}

